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

#ifndef EKG_GPU_BASE_IMPL_HPP
#define EKG_GPU_BASE_IMPL_HPP

#include <iostream>
#include <cstdint>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "ekg/math/geometry.hpp"
#include "ekg/io/typography.hpp"
#include "ekg/io/gpu.hpp"

namespace ekg::gpu {
  class api {
  protected:
    std::string_view rendering_shader_fragment_source {};
    float projection_matrix[16] {};
  public:
    ekg::gpu_api gpu_api {};
    ekg::rect_t<float> viewport {};
  public:
    void set_rendering_shader_fragment_source(std::string_view source);
  public:
    virtual void log_vendor_details() {};
    virtual void init() {};
    virtual void quit() {};
    virtual void invoke_pipeline() {};
    virtual void revoke_pipeline() {};
    virtual void pre_re_alloc() {};

    virtual void update_viewport(
      int32_t w,
      int32_t h
    ) {};
    
    virtual void re_alloc_geometry_resources(
      const float *p_data,
      uint64_t size
    ) {};

    virtual void draw(
      std::vector<ekg::io::gpu_data_t> &loaded_gpu_data_list
    ) {};

    virtual ekg::flags_t gen_font_atlas_and_map_glyph(
      ekg::sampler_t *p_sampler,
      ekg::io::font_face_t *p_font_face_text,
      ekg::io::font_face_t *p_font_face_emoji,
      ekg::io::font_face_t *p_font_face_kanjis,
      ekg::rect_t<int32_t> &atlas_rect,
      std::vector<char32_t> &char_to_gen_sampler_list,
      std::unordered_map<char32_t, ekg::io::glyph_char_t> &mapped_gpu_data_char_glyph,
      float &non_swizzlable_range
    ) { return ekg::result::not_implemented; };

    virtual ekg::flags_t allocate_sampler(
      ekg::sampler_allocate_info_t *p_sampler_allocate_info,
      ekg::sampler_t *p_sampler
    )  { return ekg::result::not_implemented; }

    virtual ekg::flags_t fill_sampler(
      ekg::sampler_fill_info_t *p_sampler_fill_info,
      ekg::sampler_t *p_sampler
    ) { return ekg::result::not_implemented; };

    virtual ekg::flags_t bind_sampler(
      ekg::sampler_t *p_sampler
    ) { return ekg::result::not_implemented; };
  };
}

#endif
