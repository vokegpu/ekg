/**
 * MIT License
 * 
 * Copyright (c) 2022-2025 Rina Wilk / vokegpu@gmail.com
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

#include "ekg/draw/allocator.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/io/log.hpp"
#include "ekg/core/context.hpp"

bool ekg::draw::allocator::enable_high_priority {};
bool ekg::draw::allocator::is_simple_shape {};

void ekg::draw::allocator::init() {
  ekg::log() << "Initializing GPU allocator";
}

void ekg::draw::allocator::quit() {
  ekg::log() << "Quitting GPU allocator";
}

void ekg::draw::allocator::invoke() {
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

  this->stride_instance.x += this->stride_instance.y;
  this->stride_instance.y = 0;
}

void ekg::draw::allocator::bind_texture(ekg::sampler_t &sampler) {
  if (sampler == ekg::sampler_t::not_found) {
    return;
  }

  ekg::gpu::data_t &data {this->gpu_data_buffer.at(this->data_instance)};
  data.sampler_at = ekg::p_core->p_gpu_api->bind_sampler(sampler);
}

void ekg::draw::allocator::dispatch() {
  ekg::gpu::data_t *p_data {/* stupid */};

  if (ekg::draw::allocator::enable_high_priority) {
    this->high_priority_gpu_data_buffer.push_back(this->gpu_data_buffer.at(this->data_instance));

    p_data = (
      &this->high_priority_gpu_data_buffer.at(this->high_priority_data_instance)
    );

    this->data_instance -= this->data_instance > 0;
    this->high_priority_data_instance++;
  } else {
    p_data = &this->gpu_data_buffer.at(this->data_instance);
  }

  /**
   * Scissor must be externally synchned.
   **/
  p_data->buffer[8] = this->scissor_instance.x;
  p_data->buffer[9] = this->scissor_instance.y;
  p_data->buffer[10] = this->scissor_instance.w;
  p_data->buffer[11] = this->scissor_instance.h;

  /**
   * the point of re-using a simple shape stride makes performance a little better,
   * due the index rendering, with only one triangle for rectangles.
   **/

  if (ekg::draw::allocator::is_simple_shape) {
    this->stride_instance.y = 0;

    /**
     * Simple shade contains only 4 vertices because it is indexed-rendered.
     **/
    p_data->begin_stride = this->simple_shape_instance;
    p_data->end_stride = 4;
  } else {
    /**
     * Peek `ekg/gpu/gl/shaders.cpp`.
     * Any value less than -1.0 is considered a concave by the vertex shader.
     **/
    p_data->buffer[3] = -1.1f;

    p_data->begin_stride = this->stride_instance.x;
    p_data->end_stride = this->stride_instance.y;
  }

  if (!this->was_hash_changed) {
    this->was_hash_changed = (
      this->previous_hash != p_data->hash
    );
  }

  this->stride_instance.x += this->stride_instance.y;
  this->stride_instance.y = 0;
  this->data_instance++;
}

void ekg::draw::allocator::revoke() {
  this->data_instance -= this->data_instance > 0;

  if (!this->high_priority_gpu_data_buffer.empty()) {
    uint64_t high_priority_gpu_data_buffer_size {
      this->high_priority_gpu_data_buffer.size()
    };

    this->gpu_data_buffer.insert(
      this->gpu_data_buffer.begin() + this->data_instance + 1,
      this->high_priority_gpu_data_buffer.begin(),
      this->high_priority_gpu_data_buffer.end()
    );

    this->data_instance += high_priority_gpu_data_buffer_size;
    this->high_priority_gpu_data_buffer.clear();
    this->high_priority_data_instance = 0;
  }

  if (
      this->last_geometry_buffer_size != this->geometry_instance
      ||
      this->was_hash_changed
    ) {
    ekg::p_core->p_gpu_api->pass_geometry_buffer_to_gpu(
      this->geometry_buffer.data(),
      this->geometry_buffer.size()
    );
  }

  this->last_geometry_buffer_size = this->geometry_instance;
  this->was_hash_changed = false;
  ekg::metrics.gpu_data_count = this->data_instance;
}

void ekg::draw::allocator::to_gpu() {
  ekg::p_core->p_gpu_api->pass_gpu_data_buffer_to_gpu(
    this->gpu_data_buffer
  );
}

void ekg::draw::allocator::clear_current_data() {
  if (!ekg::draw::allocator::enable_high_priority && this->data_instance >= this->gpu_data_buffer.size()) {
    this->gpu_data_buffer.emplace_back();
  }

  /* allocator handle automatically the size of data */
  ekg::gpu::data_t &data {this->gpu_data_buffer.at(this->data_instance)};
  data.line_thickness = 0;
  data.sampler_at = ekg::sampler_t::not_found;

  this->previous_hash = data.hash;
}

ekg::gpu::data_t &ekg::draw::allocator::bind_current_data() {
  this->clear_current_data();
  return this->gpu_data_buffer.at(this->data_instance);
}

size_t ekg::draw::allocator::get_current_data_id() {
  return this->data_instance;
}

ekg::gpu::data_t &ekg::draw::allocator::get_data_by_index(size_t index) {
  if (index >= this->gpu_data_buffer.size()) {
    return ekg::gpu::data_t::not_found;
  }

  return this->gpu_data_buffer[index];
}

bool ekg::draw::allocator::sync_scissor(
  ekg::rect_t<float> &rect_scissor,
  ekg::rect_t<float> &rect_child,
  ekg::rect_t<float> &rect_parent_scissor,
  bool is_parented
) {
  rect_scissor.x = rect_child.x;
  rect_scissor.y = rect_child.y;
  rect_scissor.w = rect_child.w;
  rect_scissor.h = rect_child.h;

  if (is_parented) {
    bool only_if {};

    only_if = rect_scissor.x < rect_parent_scissor.x;
    rect_scissor.w -= only_if * (rect_parent_scissor.x - rect_scissor.x);
    rect_scissor.x = (
      (only_if * rect_parent_scissor.x) + (rect_scissor.x * !only_if)
    );

    only_if = rect_scissor.y < rect_parent_scissor.y;
    rect_scissor.h -= only_if * (rect_parent_scissor.y - rect_scissor.y);
    rect_scissor.y = (only_if * rect_parent_scissor.y) + (rect_scissor.y * !only_if);

    only_if = rect_scissor.x + rect_scissor.w > rect_parent_scissor.x + rect_parent_scissor.w;
    rect_scissor.w -= only_if * ((rect_scissor.x + rect_scissor.w) - (rect_parent_scissor.x + rect_parent_scissor.w));

    only_if = rect_scissor.y + rect_scissor.h > rect_parent_scissor.y + rect_parent_scissor.h;
    rect_scissor.h -= only_if * ((rect_scissor.y + rect_scissor.h) - (rect_parent_scissor.y + rect_parent_scissor.h));

    this->scissor_instance.x = rect_scissor.x;
    this->scissor_instance.y = rect_scissor.y;
    this->scissor_instance.w = rect_scissor.w;
    this->scissor_instance.h = rect_scissor.h;

    return (
      rect_scissor.x < rect_parent_scissor.x + rect_parent_scissor.w
      &&
      rect_scissor.x + rect_scissor.w > rect_parent_scissor.x
      &&
      rect_scissor.y < rect_parent_scissor.y + rect_parent_scissor.h
      &&
      rect_scissor.y + rect_scissor.h > rect_parent_scissor.y
    );
  }

  this->scissor_instance.x = rect_scissor.x;
  this->scissor_instance.y = rect_scissor.y;
  this->scissor_instance.w = rect_scissor.w;
  this->scissor_instance.h = rect_scissor.h;

  return true;
}

void ekg::draw::allocator::unsafe_set_scissor_rect(
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

void ekg::draw::allocator::push_back_geometry(
  float x, float y,
  float u, float v
) {
  this->stride_instance.y++;
  this->geometry_buffer.push_back(x);
  this->geometry_buffer.push_back(y);
  this->geometry_buffer.push_back(u);
  this->geometry_buffer.push_back(v);
  this->geometry_instance += 4;
}
