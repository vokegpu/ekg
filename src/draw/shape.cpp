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

#include "ekg/draw/shape.hpp"
#include "ekg/core/runtime.hpp"

void ekg::draw::rect(
  const ekg::rect_t<float> &rect,
  const ekg::vec4_t<float> &color,
  int32_t line_thickness,
  ekg::sampler_t *p_sampler
) {
  ekg::draw::rect(
    rect.x, rect.y, rect.w, rect.h,
    color,
    line_thickness,
    p_sampler
  );
}

void ekg::draw::rect(
  float x, float y, float w, float h,
  const ekg::vec4_t<float> &color,
  int32_t line_thickness,
  ekg::sampler_t *p_sampler
) {
  if (color.w < 0.1f) {
    return;
  }

  ekg::io::gpu_data_t &data {ekg::p_core->gpu_allocator.bind_current_data()};

  data.buffer_content[0] = x;
  data.buffer_content[1] = y;
  data.buffer_content[2] = w;
  data.buffer_content[3] = h;
  data.buffer_content[4] = color.x;
  data.buffer_content[5] = color.y;
  data.buffer_content[6] = color.z;
  data.buffer_content[7] = color.w;

  data.line_thickness = static_cast<int8_t>(line_thickness);
  data.factor = 1;

  ekg::p_core->gpu_allocator.bind_texture(p_sampler);
  ekg::p_core->gpu_allocator.dispatch();
}

void ekg::draw::sync_scissor(
  ekg::rect_t<float> &scissor,
  ekg::rect_t<float> &rect_child,
  ekg::rect_t<float> *p_parent_scissor
) {
  ekg::p_core->gpu_allocator.sync_scissor(scissor, rect_child, p_parent_scissor);
}

void ekg::draw::scissor(
  float x,
  float y,
  float w,
  float h
) {
  ekg::p_core->gpu_allocator.unsafe_set_scissor_placement(
    x, y, w, h
  );  
}

void ekg::draw::enable_high_priority() {
  ekg::gpu::allocator::high_priority = true;
}

void ekg::draw::disable_high_priority() {
  ekg::gpu::allocator::high_priority = false;
}
