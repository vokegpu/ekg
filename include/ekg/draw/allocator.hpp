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
#ifndef EKG_DRAW_ALLOCATOR_HPP
#define EKG_DRAW_ALLOCATOR_HPP

#include <vector>

#include "ekg/math/geometry.hpp"
#include "ekg/gpu/data.hpp"
#include "ekg/gpu/sampler.hpp"

/**
 * This macro prevent from dispatching any GPU-data under a render section
 * if the content is not visible to the parent rect_scissor.
 **/
#define ekg_assert_scissor(rect_scissor, rect_child, rect_parent, is_parented) \
  if (!ekg::p_core->draw_allocator.sync_scissor(rect_scissor, rect_child, rect_parent, is_parented)) return;

namespace ekg::draw {
  class allocator {
  public:
    static bool enable_high_priority;
    static bool is_simple_shape;
  protected:
    size_t data_instance {};
    ekg::vec2_t<size_t> stride_instance {};
    size_t simple_shape_instance {};
    size_t geometry_instance {};
    size_t high_priority_data_instance {};
    ekg::rect_t<float> scissor_instance {};

    std::vector<ekg::gpu::data_t> gpu_data_buffer {};
    std::vector<ekg::gpu::data_t> high_priority_gpu_data_buffer {};
    std::vector<float> geometry_buffer {};
    size_t last_geometry_buffer_size {};

    bool was_hash_changed {};
    int32_t previous_hash {};
  public:
    void init();
    void quit();

    void invoke();
    void revoke();
    void to_gpu();

    void bind_texture(ekg::at_t &sampler_at);
    ekg::gpu::data_t &bind_current_data();
    size_t get_current_data_id();
    ekg::gpu::data_t &get_data_by_index(size_t index);
    void clear_current_data();
    void dispatch();

    bool sync_scissor(
      ekg::rect_t<float> &rect_scissor,
      ekg::rect_t<float> &rect_child,
      ekg::rect_t<float> &rect_parent_scissor,
      bool is_parented
    );

    void unsafe_set_scissor_rect(
      float x, float y, float w, float h
    );

    void push_back_geometry(
      float x, float y,
      float u, float v
    );
  };
}

#endif
