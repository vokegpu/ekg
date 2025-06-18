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
  this->global_data_instance = 0;

  this->gpu_data_buffer.clear();
  this->geometry_buffer.clear();

  /**
   * inserting a simple triangle mesh,
   * is necessary to make work the simple-shape rendering.
   **/
  this->p_local_geometry_buffer = &this->geometry_buffer;
  this->push_back_geometry(0.0f, 0.0f, 0.0f, 0.0f);
  this->push_back_geometry(0.0f, 1.0f, 0.0f, 1.0f);
  this->push_back_geometry(1.0f, 0.0f, 1.0f, 0.0f);
  this->push_back_geometry(1.0f, 1.0f, 1.0f, 1.0f);
  this->p_local_geometry_buffer = nullptr;

  this->stride_instance.x += this->stride_instance.y;
  this->stride_instance.y = 0;
}

void ekg::draw::allocator::bind_texture(ekg::at_t &sampler_at) {
  ekg::gpu::data_t &data {this->p_local_gpu_data_buffer->at(this->data_instance)};
  data.sampler_at = ekg::p_core->p_gpu_api->bind_sampler(sampler_at);
}

void ekg::draw::allocator::revoke() {
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
  ekg::metrics.gpu_data_count = this->gpu_data_buffer.size();
}

void ekg::draw::allocator::to_gpu() {
  ekg::p_core->p_gpu_api->pass_gpu_data_buffer_to_gpu(
    this->gpu_data_buffer
  );
}

void ekg::draw::allocator::bind_local(
  std::vector<float> *p_geometry_buffer,
  std::vector<ekg::gpu::data_t> *p_gpu_data_buffer
) {
  this->p_local_geometry_buffer = p_geometry_buffer;
  this->p_local_gpu_data_buffer = p_gpu_data_buffer;
  this->p_local_geometry_buffer->clear();
  this->data_instance = 0;
}

ekg::gpu::data_t &ekg::draw::allocator::bind_current_data() {
  if (this->data_instance >= this->p_local_gpu_data_buffer->size()) {
    this->p_local_gpu_data_buffer->emplace_back().is_new = true;
  }

  ekg::gpu::data_t &data {this->p_local_gpu_data_buffer->at(this->data_instance)};
  data.line_thickness = 0;
  data.sampler_at = ekg::sampler_t::not_found;
  this->previous_hash = data.hash;  
  return data;
}

void ekg::draw::allocator::dispatch() {  
  ekg::gpu::data_t &data {this->p_local_gpu_data_buffer->at(this->data_instance)};

  /**
   * Scissor must be externally synchned.
   **/
  data.buffer[8] = this->scissor_instance.x;
  data.buffer[9] = this->scissor_instance.y;
  data.buffer[10] = this->scissor_instance.w;
  data.buffer[11] = this->scissor_instance.h;

  /**
   * the point of re-using a simple shape stride makes performance a little better,
   * due the index rendering, with only one triangle for rectangles.
   **/

  if (ekg::draw::allocator::is_simple_shape) {
    this->stride_instance.y = 0;

    /**
     * Simple shade contains only 4 vertices because it is indexed-rendered.
     **/
    data.begin_stride = this->simple_shape_instance;
    data.end_stride = 4;
  } else {
    /**
     * Peek `ekg/gpu/gl/shaders.cpp`.
     * Any value less than -1.0 is considered a concave by the vertex shader.
     **/
    data.buffer[3] = -1.1f;

    data.begin_stride = this->stride_instance.x;
    data.end_stride = this->stride_instance.y;
  }

  if (!this->was_hash_changed) {
    this->was_hash_changed = (
      this->previous_hash != data.hash
    );
  }

  this->stride_instance.x += this->stride_instance.y;
  this->stride_instance.y = 0;
  this->data_instance++;
}

void ekg::draw::allocator::pass() {
  /*if (!this->p_local_gpu_data_buffer->empty()) {
    ekg::gpu::data_t &data {this->p_local_gpu_data_buffer->at(0)};
    if (
      !data.is_new
    ) {
      data.mem_block_offset = this->p_local_geometry_buffer->size() - data.prev_mem_block_size;
    }

    data.prev_mem_block_size = this->p_local_geometry_buffer->size();
  }*/

  this->gpu_data_buffer.insert(
    this->gpu_data_buffer.end(),
    this->p_local_gpu_data_buffer->begin(),
    this->p_local_gpu_data_buffer->begin() + this->data_instance
  );

  this->geometry_buffer.insert(
    this->geometry_buffer.end(),
    this->p_local_geometry_buffer->begin(),
    this->p_local_geometry_buffer->end()
  );

  this->global_data_instance += this->gpu_data_buffer.size();
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
  this->p_local_geometry_buffer->push_back(x);
  this->p_local_geometry_buffer->push_back(y);
  this->p_local_geometry_buffer->push_back(u);
  this->p_local_geometry_buffer->push_back(v);
  this->geometry_instance += 4;
}
