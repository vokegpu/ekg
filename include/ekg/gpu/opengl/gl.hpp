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
#ifndef EKG_GPU_OPENGL_GL_HPP
#define EKG_GPU_OPENGL_GL_HPP

#if defined(__ANDROID__)
  #include <GLES3/gl3.h>
#else
  #include <GL/glew.h>
#endif

#include <vector>
#include <unordered_map>

#include "ekg/gpu/api.hpp"

#define EKG_DISABLE_TEXTURE 0
#define EKG_ENABLE_TEXTURE_PROTECTED 1
#define EKG_ENABLE_TEXTURE 2

namespace ekg {
  class opengl : public ekg::gpu::api {
  protected:
    std::vector<ekg::at_t> bound_sampler_list {};
    std::string_view glsl_version {};

    int32_t uniform_active_texture {};
    int32_t uniform_active_tex_slot {};
    int32_t uniform_content {};
    int32_t uniform_rect {};
    int32_t uniform_line_thickness {};
    int32_t uniform_scissor {};
    int32_t uniform_viewport_height {};
    int32_t uniform_projection {};

    uint32_t geometry_buffer {};
    uint32_t vbo_array {};
    uint32_t ebo_simple_shape {};
    uint32_t pipeline_program {};
    uint8_t protected_texture_active_index {};
  public:
    bool create_pipeline_program(
      uint32_t &program,
      const std::unordered_map<std::string_view, uint32_t> &resources
    );
  public:
    /**
     * OpenGL API wrapper abstraction constructor;
     * `set_glsl_version` must be 330 higher, if not, the version is auto-initialized as `450`.
     * OpenGL ES 3 needs explicit set to the GLSL ES version.
     */
    explicit opengl(std::string_view set_glsl_version = "#version 450");
  public:
    void log_vendor_details() override;

    void init() override;
    void quit() override;
    void pre_re_alloc() override;
    void update_viewport(int32_t w, int32_t h) override;
    void pass_geometry_buffer_to_gpu(const float *p_data, uint64_t size) override;
    
    void pass_gpu_data_buffer_to_gpu(
      std::vector<ekg::gpu::data_t> &gpu_data_buffer
    ) override;

    ekg::flags_t allocate_sampler(
      ekg::sampler_allocate_info_t &sampler_allocate_info,
      ekg::sampler_t &sampler
    ) override;

    ekg::flags_t fill_sampler(
      ekg::sampler_fill_info_t &sampler_fill_info,
      ekg::sampler_t &sampler
    ) override;

    ekg::flags_t gen_font_atlas_and_map_glyph(
      ekg::sampler_t &sampler,
      ekg::io::font_face_t *p_font_face_text,
      ekg::io::font_face_t *p_font_face_emoji,
      ekg::io::font_face_t *p_font_face_kanjis,
      ekg::rect_t<int32_t> &atlas_rect,
      std::vector<char32_t> &char_to_gen_sampler_list,
      std::unordered_map<char32_t, ekg::io::glyph_t> &mapped_gpu_data_char_glyph,
      float &non_swizzlable_range
    ) override;

    ekg::at_t &bind_sampler(
      ekg::at_t &sampler_at
    ) override;
  };
}

#endif
