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

#ifndef EKG_GPU_ALLOCATOR_HPP
#define EKG_GPU_ALLOCATOR_HPP

#include <array>
#include <vector>

#include "ekg/gpu/api.hpp"

#define EKG_ASSERT_SCISSOR(scissor, rect_child, p_parent_scissor) if (!ekg::p_core->gpu_allocator.sync_scissor(scissor, rect_child, p_parent_scissor)) return

namespace ekg::gpu {
  class allocator {
  public:
    static float concave;
    static bool high_priority;
    static uint64_t current_rendering_data_count;
  protected:
    std::vector<ekg::io::gpu_data_t> data_list {};
    std::vector<float> geometry_resource_list {};

    std::vector<ekg::io::gpu_data_t> high_priority_data_list {};
    std::vector<float> high_priority_geometry_resource_list {};

    uint64_t high_priority_data_instance_index {};
    uint64_t data_instance_index {};
    uint64_t geometry_resource_index {};
    uint64_t previous_geometry_resource_list_size {};

    int32_t simple_shape_index {-1};
    int32_t previous_factor {};

    int32_t begin_stride_count {};
    int32_t end_stride_count {};

    bool factor_changed {};
    bool simple_shape {};
    bool out_of_scissor_rect {};
    ekg::rect_t<float> scissor_instance {};
  public:
    /*
     * Init gpu allocator.
     */
    void init();

    /*
     * Delete all GPU buffers & GL stuf.
     */
    void quit();

    /*
     * Bind a new gpu data.
     */
    ekg::io::gpu_data_t &bind_current_data();

    /*
     * Clear current gpu data active.
     */
    void clear_current_data();

    /*
     * Find registered gpu data in allocator's batch.
     */
    ekg::io::gpu_data_t *get_data_by_id(int32_t id);

    /*
     * Get current gpu data.
     */
    uint32_t get_current_data_id();

    /*
     * Bind texture for send in data.
     */
    void bind_texture(ekg::sampler_t *p_sampler);

    /*
     * Insert geometry positions here:
     * vertex x, y and texture coords u, and v.
     */
    void push_back_geometry(float, float, float, float);

    /*
     * Update animations.
     */
    void on_update();

    /*
     * Invoke allocator data reader.
     */
    void invoke();

    /*
     * Dispatch current data.
     */
    void dispatch();

    /*
     * Revoke data to GPU.
     */
    void revoke();

    /*
     * Draw all data from the gpu allocator's batch.
     */
    void draw();

    /*
     * Sync active scissor position.
     */
    bool sync_scissor(
      ekg::rect_t<float> &scissor,
      ekg::rect_t<float> &rect_child,
      ekg::rect_t<float> *p_parent_scissor
    );

    /**
     * Set scissor position and size, unsafe due the stack GPU-data order in allocator,
     * which affect the rendering.
     * 
     * When a GPU-data is created it receives the current scissor placement (position and size),
     * and the next GPU-data stacked may not receive the same placements.
     **/
    void unsafe_set_scissor_placement(
      float x,
      float y,
      float w,
      float h
    );
  };
}

#endif
