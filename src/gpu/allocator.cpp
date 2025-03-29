/**
 * MIT License
 * 
 * Copyright (c) 2022-2024 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ekg/gpu/allocator.hpp"
#include "ekg/core/runtime.hpp"

// TODO: add capacity mapped-gbuffer

bool ekg::gpu::allocator::high_priority {};
float ekg::gpu::allocator::concave {-2.0f};
uint64_t ekg::gpu::allocator::current_rendering_data_count {};

void ekg::gpu::allocator::invoke() {
  this->data_instance_index = 0;
  this->begin_stride_count = 0;
  this->end_stride_count = 0;
  this->simple_shape_index = 0;
  this->geometry_resource_index = 0;

  /**
   * inserting a simple triangle mesh,
   * is necessary to make work the simple-shape rendering.
   **/
  this->push_back_geometry(0.0f, 0.0f, 0.0f, 0.0f);
  this->push_back_geometry(0.0f, 1.0f, 0.0f, 1.0f);
  this->push_back_geometry(1.0f, 0.0f, 1.0f, 0.0f);
  this->push_back_geometry(1.0f, 1.0f, 1.0f, 1.0f);

  if (this->data_instance_index >= this->data_list.size()) {
    this->data_list.emplace_back();
  }

  //this->data_list.at(this->data_instance_index).begin_stride = this->end_stride_count;
  this->begin_stride_count += this->end_stride_count;
  this->end_stride_count = 0;
}

void ekg::gpu::allocator::bind_texture(ekg::sampler_t *p_sampler) {
  if (p_sampler == nullptr) {
    return;
  }

  ekg::io::gpu_data_t &data {this->data_list.at(this->data_instance_index)};
  data.sampler_index = ekg::p_core->p_gpu_api->bind_sampler(p_sampler);
}

void ekg::gpu::allocator::dispatch() {
  ekg::io::gpu_data_t *p_data {};

  if (ekg::gpu::allocator::high_priority) {
    if (this->high_priority_data_instance_index >= this->high_priority_data_list.size()) {
      this->high_priority_data_list.emplace_back();
    }

    p_data = &(
      this->high_priority_data_list.at(this->high_priority_data_instance_index++) = (
        ekg::io::gpu_data_t {this->data_list.at(this->data_instance_index)}
      )
    );

    this->data_instance_index -= this->data_instance_index > 0;
  } else {
    p_data = &this->data_list.at(this->data_instance_index);
  }

  /**
   * Scissor must be synchned externally to update the scissor context.
   **/

  p_data->buffer_content[8] = this->scissor_instance.x;
  p_data->buffer_content[9] = this->scissor_instance.y;
  p_data->buffer_content[10] = this->scissor_instance.w;
  p_data->buffer_content[11] = this->scissor_instance.h;

  /**
   * the point of re-using a simple shape stride makes performance a little better,
   * due the index rendering, with only one triangle for rectangles.
   **/

  this->simple_shape = (
    static_cast<int32_t>(p_data->buffer_content[2]) != static_cast<int32_t>(ekg::gpu::allocator::concave)
    &&
    static_cast<int32_t>(p_data->buffer_content[3]) != static_cast<int32_t>(ekg::gpu::allocator::concave)
  );

  if (this->simple_shape) {
    p_data->begin_stride = this->simple_shape_index;
    p_data->end_stride = 4; // simple shape contains 4 vertices.
    this->end_stride_count = 0;
  } else {    
    p_data->begin_stride = this->begin_stride_count;
    p_data->end_stride = this->end_stride_count;
  }

  /* flag re alloc buffers if factor changed */

  if (!this->factor_changed) {
    this->factor_changed = (
      this->previous_factor != p_data->factor
    );
  }

  this->begin_stride_count += this->end_stride_count;
  this->end_stride_count = 0;
  this->data_instance_index++;
}

void ekg::gpu::allocator::revoke() {
  this->data_instance_index -= this->data_instance_index > 0;

  if (!this->high_priority_data_list.empty()) {
    uint64_t high_priority_data_list_size {this->high_priority_data_list.size()};

    /**
     * So, I do not trust this code by the performanceless,
     * usually I would use a refill, but if the refill necessary part
     * is larger than the data list size, then it requires a re-allocation;
     * and so I do not know how code it performanceness.
     * 
     * Rina.
     **/
    this->data_list.erase(
      this->data_list.begin() + this->data_instance_index + 1,
      this->data_list.end()
    );

    this->data_list.insert(
      this->data_list.end(),
      this->high_priority_data_list.begin(),
      this->high_priority_data_list.end()
    );

    this->data_instance_index += high_priority_data_list_size;
    this->high_priority_data_list.clear();
    this->high_priority_data_instance_index = 0;
  }

  uint64_t geometry_resource_list_size {this->geometry_resource_index};
  bool should_re_alloc_buffers {this->previous_geometry_resource_list_size != geometry_resource_list_size};

  if (this->data_instance_index < this->data_list.size()) {
    this->data_list.erase(
      this->data_list.begin() + this->data_instance_index + 1,
      this->data_list.end()
    );
  }

  if (should_re_alloc_buffers || this->factor_changed) {
    ekg::p_core->p_gpu_api->re_alloc_geometry_resources(
      this->geometry_resource_list.data(),
      this->geometry_resource_list.size()
    );
  }

  this->factor_changed = false;

  if (this->geometry_resource_list.size() < this->previous_geometry_resource_list_size) {
    this->geometry_resource_list.erase(
      this->geometry_resource_list.begin() + geometry_resource_list_size + 1,
      this->geometry_resource_list.end()
    );
  }

  this->previous_geometry_resource_list_size = geometry_resource_list_size;
  ekg::gpu::allocator::current_rendering_data_count = this->data_list.size();
}

void ekg::gpu::allocator::on_update() {
}

void ekg::gpu::allocator::draw() {
  ekg::p_core->p_gpu_api->draw(
    this->data_list
  );
}

void ekg::gpu::allocator::init() {
  ekg::log() << "Initializing GPU allocator";
}

void ekg::gpu::allocator::clear_current_data() {  
  if (!ekg::gpu::allocator::high_priority && this->data_instance_index >= this->data_list.size()) {
    this->data_list.emplace_back();
  }

  /* allocator handle automatically the size of data */
  ekg::io::gpu_data_t &data {this->data_list.at(this->data_instance_index)};
  data.line_thickness = 0;
  data.sampler_index = -1;

  this->previous_factor = data.factor;
}

ekg::io::gpu_data_t &ekg::gpu::allocator::bind_current_data() {
  this->clear_current_data();
  return this->data_list.at(this->data_instance_index);
}

uint32_t ekg::gpu::allocator::get_current_data_id() {
  return this->data_instance_index;
}

ekg::io::gpu_data_t *ekg::gpu::allocator::get_data_by_id(int32_t id) {
  if (id < 0 || static_cast<uint64_t>(id) > this->data_instance_index) {
    return nullptr;
  }

  return &this->data_list[id];
}

void ekg::gpu::allocator::quit() {
  ekg::log() << "Quitting GPU allocator";
}

bool ekg::gpu::allocator::sync_scissor(
  ekg::rect_t<float> &scissor,
  ekg::rect_t<float> &rect_child,
  ekg::rect_t<float> *p_parent_scissor
) {
  scissor.x = rect_child.x;
  scissor.y = rect_child.y;
  scissor.w = rect_child.w;
  scissor.h = rect_child.h;

  if (p_parent_scissor) {
    bool only_if {};

    only_if = scissor.x < p_parent_scissor->x;
    scissor.w -= only_if * (p_parent_scissor->x - scissor.x);
    scissor.x = (only_if * p_parent_scissor->x) + (scissor.x * !only_if);

    only_if = scissor.y < p_parent_scissor->y;
    scissor.h -= only_if * (p_parent_scissor->y - scissor.y);
    scissor.y = (only_if * p_parent_scissor->y) + (scissor.y * !only_if);

    only_if = scissor.x + scissor.w > p_parent_scissor->x + p_parent_scissor->w;
    scissor.w -= only_if * ((scissor.x + scissor.w) - (p_parent_scissor->x + p_parent_scissor->w));

    only_if = scissor.y + scissor.h > p_parent_scissor->y + p_parent_scissor->h;
    scissor.h -= only_if * ((scissor.y + scissor.h) - (p_parent_scissor->y + p_parent_scissor->h));

    this->scissor_instance.x = scissor.x;
    this->scissor_instance.y = scissor.y;
    this->scissor_instance.w = scissor.w;
    this->scissor_instance.h = scissor.h;

    return (
      scissor.x < p_parent_scissor->x + p_parent_scissor->w
      &&
      scissor.x + scissor.w > p_parent_scissor->x
      &&
      scissor.y < p_parent_scissor->y + p_parent_scissor->h
      &&
      scissor.y + scissor.h > p_parent_scissor->y
    );
  }

  this->scissor_instance.x = scissor.x;
  this->scissor_instance.y = scissor.y;
  this->scissor_instance.w = scissor.w;
  this->scissor_instance.h = scissor.h;

  return true;
}

void ekg::gpu::allocator::unsafe_set_scissor_placement(
  float x,
  float y,
  float w,
  float h
) {
  this->scissor_instance.x = x;
  this->scissor_instance.y = y;
  this->scissor_instance.w = w;
  this->scissor_instance.h = h;
}

void ekg::gpu::allocator::push_back_geometry(
  float x,
  float y,
  float u,
  float v
) {
  this->end_stride_count++;

  if (this->geometry_resource_index >= this->geometry_resource_list.size()) {
    this->geometry_resource_index += 4;

    this->geometry_resource_list.emplace_back(x);
    this->geometry_resource_list.emplace_back(y);

    this->geometry_resource_list.emplace_back(u);
    this->geometry_resource_list.emplace_back(v);

    return;
  }

  this->geometry_resource_list[this->geometry_resource_index++] = x;
  this->geometry_resource_list[this->geometry_resource_index++] = y;

  this->geometry_resource_list[this->geometry_resource_index++] = u;
  this->geometry_resource_list[this->geometry_resource_index++] = v;
}
