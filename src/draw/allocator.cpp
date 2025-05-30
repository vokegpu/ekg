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
  this->data_instance = 0;
  this->stride_instance.x = 0;
  this->stride_instance.y = 0;
  this->simple_shape_instance = 0;
  this->geometry_instance = 0;

  /**
   * inserting a simple triangle mesh,
   * is necessary to make work the simple-shape rendering.
   **/
  this->push_back_geometry(0.0f, 0.0f, 0.0f, 0.0f);
  this->push_back_geometry(0.0f, 1.0f, 0.0f, 1.0f);
  this->push_back_geometry(1.0f, 0.0f, 1.0f, 0.0f);
  this->push_back_geometry(1.0f, 1.0f, 1.0f, 1.0f);

  if (this->data_instance >= this->gpu_data_buffer.size()) {
    this->gpu_data_buffer.emplace_back();
  }

  //this->gpu_data_buffer.at(this->data_instance).begin_stride = this->stride_instance.y;
  this->stride_instance.x += this->stride_instance.y;
  this->stride_instance.y = 0;
}

void ekg::gpu::allocator::bind_texture(ekg::sampler_t &sampler) {
  if (sampler == ekg::sampler_t::not_found) {
    return;
  }

  ekg::gpu::data_t &data {this->gpu_data_buffer.at(this->data_instance)};
  data.sampler_at = ekg::p_core->p_gpu_api->bind_sampler(sampler);
}

void ekg::gpu::allocator::dispatch() {
  ekg::gpu::data_t *p_data {};

  if (ekg::gpu::allocator::high_priority) {
    if (this->high_priority_data_instance >= this->loaded_high_priority_data_list.size()) {
      this->loaded_high_priority_data_list.emplace_back();
    }

    p_data = &(
      this->loaded_high_priority_data_list.at(this->high_priority_data_instance++) = (
        ekg::gpu::data_t {this->gpu_data_buffer.at(this->data_instance)}
      )
    );

    this->data_instance -= this->data_instance > 0;
  } else {
    p_data = &this->gpu_data_buffer.at(this->data_instance);
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
    p_data->begin_stride = this->simple_shape_instance;
    p_data->end_stride = 4; // simple shape contains 4 vertices.
    this->stride_instance.y = 0;
  } else {    
    p_data->begin_stride = this->stride_instance.x;
    p_data->end_stride = this->stride_instance.y;
  }

  /* flag re alloc buffers if factor changed */

  if (!this->factor_changed) {
    this->factor_changed = (
      this->previous_factor != p_data->factor
    );
  }

  this->stride_instance.x += this->stride_instance.y;
  this->stride_instance.y = 0;
  this->data_instance++;
}

void ekg::gpu::allocator::revoke() {
  this->data_instance -= this->data_instance > 0;

  if (!this->loaded_high_priority_data_list.empty()) {
    uint64_t loaded_high_priority_data_list_size {this->loaded_high_priority_data_list.size()};

    /**
     * So, I do not trust this code by the performanceless,
     * usually I would use a refill, but if the refill necessary part
     * is larger than the data list size, then it requires a re-allocation;
     * and so I do not know how code it performanceness.
     * 
     * Rina.
     **/
    this->gpu_data_buffer.erase(
      this->gpu_data_buffer.begin() + this->data_instance + 1,
      this->gpu_data_buffer.end()
    );

    this->gpu_data_buffer.insert(
      this->gpu_data_buffer.end(),
      this->loaded_high_priority_data_list.begin(),
      this->loaded_high_priority_data_list.end()
    );

    this->data_instance += loaded_high_priority_data_list_size;
    this->loaded_high_priority_data_list.clear();
    this->high_priority_data_instance = 0;
  }

  uint64_t geometry_buffer_size {this->geometry_instance};
  bool should_re_alloc_buffers {this->previous_geometry_buffer_size != geometry_buffer_size};

  if (this->data_instance < this->gpu_data_buffer.size()) {
    this->gpu_data_buffer.erase(
      this->gpu_data_buffer.begin() + this->data_instance + 1,
      this->gpu_data_buffer.end()
    );
  }

  if (should_re_alloc_buffers || this->factor_changed) {
    ekg::p_core->p_gpu_api->re_alloc_geometry_resources(
      this->geometry_buffer.data(),
      this->geometry_buffer.size()
    );
  }

  this->factor_changed = false;

  if (this->geometry_buffer.size() < this->previous_geometry_buffer_size) {
    this->geometry_buffer.erase(
      this->geometry_buffer.begin() + geometry_buffer_size + 1,
      this->geometry_buffer.end()
    );
  }

  this->previous_geometry_buffer_size = geometry_buffer_size;
  ekg::gpu::allocator::current_rendering_data_count = this->gpu_data_buffer.size();
}

void ekg::gpu::allocator::on_update() {
}

void ekg::gpu::allocator::draw() {
  ekg::p_core->p_gpu_api->draw(
    this->gpu_data_buffer
  );
}

void ekg::gpu::allocator::init() {
  ekg::log() << "Initializing GPU allocator";
}

void ekg::gpu::allocator::clear_current_data() {  
  if (!ekg::gpu::allocator::high_priority && this->data_instance >= this->gpu_data_buffer.size()) {
    this->gpu_data_buffer.emplace_back();
  }

  /* allocator handle automatically the size of data */
  ekg::gpu::data_t &data {this->gpu_data_buffer.at(this->data_instance)};
  data.line_thickness = 0;
  data.sampler_at = ekg::sampler_t::not_found;

  this->previous_factor = data.factor;
}

ekg::gpu::data_t &ekg::gpu::allocator::bind_current_data() {
  this->clear_current_data();
  return this->gpu_data_buffer.at(this->data_instance);
}

uint32_t ekg::gpu::allocator::get_current_data_id() {
  return this->data_instance;
}

ekg::gpu::data_t *ekg::gpu::allocator::get_data_by_id(int32_t id) {
  if (id < 0 || static_cast<uint64_t>(id) > this->data_instance) {
    return nullptr;
  }

  return &this->gpu_data_buffer[id];
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
  const ekg::vec2_t<float> &position,
  const ekg::vec2_t<float> &uv
) {
  this->stride_instance.y++;
  this->geometry_buffer.push_back(position.x);
  this->geometry_buffer.push_back(position.y);
  this->geometry_buffer.push_back(uv.x);
  this->geometry_buffer.push_back(uv.y);
  this->geometry_instance += 4;
}
