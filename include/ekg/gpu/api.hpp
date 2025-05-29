#ifndef EKG_GPU_API_HPP
#define EKG_GPU_API_HPP

#include <iostream>
#include <cstdint>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "sampler.hpp"
#include "data.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg {
  enum class which_gpu_api {
    opengl,
    opengles,
    vulkan
  };
}

namespace ekg::gpu {
  class api {
  protected:
    float mat4x4_proj_matrix[16] {}; 
  public:
    ekg::which_gpu_api which_gpu_api {};
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
      std::vector<ekg::gpu::data_t> &loaded_gpu_data_list
    ) {};

    virtual ekg::flags_t gen_font_atlas_and_map_glyph(
      ekg::sampler_t &sampler,
      ekg::io::font_face_t *p_font_face_text,
      ekg::io::font_face_t *p_font_face_emoji,
      ekg::io::font_face_t *p_font_face_kanjis,
      ekg::rect_t<int32_t> &atlas_rect,
      std::vector<char32_t> &char_to_gen_sampler_list,
      std::unordered_map<char32_t, ekg::io::glyph_char_t> &mapped_gpu_data_char_glyph,
      float &non_swizzlable_range
    ) { return ekg::result::not_implemented; };

    virtual ekg::flags_t allocate_sampler(
      ekg::sampler_allocate_info_t &sampler_allocate_info,
      ekg::sampler_t &sampler
    )  { return ekg::result::not_implemented; }

    virtual ekg::flags_t fill_sampler(
      ekg::sampler_fill_info_t &sampler_fill_info,
      ekg::sampler_t &sampler
    ) { return ekg::result::not_implemented; };

    virtual ekg::flags_t bind_sampler(
      ekg::sampler_t &sampler
    ) { return ekg::result::not_implemented; };
  };
  };
}

#endif
