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
#ifndef EKG_GPU_SAMPLER_HPP
#define EKG_GPU_SAMPLER_HPP

#include "ekg/io/memory.hpp"
#include "ekg/io/descriptor.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg {
  struct sampler_t {
  public:
    static constexpr ekg::type type {ekg::type::sampler};
    static ekg::sampler_t not_found;
  public:
    std::string tag {};
    uint32_t w {};
    uint32_t h {};
    uint32_t channel {};
    uint32_t gl_id {};
    int8_t gl_protected_active_index {-1};
    bool is_protected {};
  public:
    ekg_descriptor(ekg::sampler_t);
  };

  struct sampler_info_t {
  public:
    std::string tag {};
    int32_t offset[2] {};
    int32_t w {};
    int32_t h {};
    int32_t gl_parameter_filter[2] {};
    int32_t gl_wrap_modes[2] {};
    int32_t gl_internal_format {};
    uint32_t gl_format {};
    uint32_t gl_type {};
    bool gl_unpack_alignment {};
    bool gl_generate_mipmap {};
    void *pv_data {};
  };

  typedef sampler_info_t sampler_allocate_info_t;
  typedef sampler_info_t sampler_fill_info_t;

  ekg::flags_t sampler_src_r8_to_r8g8b8a8(
    const ekg::vec2_t<int32_t> &size,
    const unsigned char *p_src,
    std::vector<unsigned char> &dst
  );
}

#endif
