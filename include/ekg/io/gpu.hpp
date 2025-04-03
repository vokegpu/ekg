#ifndef EKG_IO_GPU_HPP
#define EKG_IO_GPU_HPP

#include <vector>
#include <array>

#include "ekg/io/memory.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg {
  enum class gpu_api {
    vulkan,
    opengl,
    opengles,
    webgpu
  };
  
  struct sampler_info_t {
  public:
    const char *p_tag {};
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
    void *p_data {};
  };

  typedef sampler_info_t sampler_allocate_info_t;
  typedef sampler_info_t sampler_fill_info_t;

  struct sampler_t {
  public:
    const char *p_tag {};
    uint32_t w {};
    uint32_t h {};
    uint32_t channel {};
    uint32_t gl_id {};
    int8_t gl_protected_active_index {-1};
    bool is_protected {};
  };

  enum class layer {
    background,
    highlight,
    active
  };

  struct layer_group_t {
  protected:
    std::array<ekg::sampler_t*, 3> samplers {};
  public:
    ekg::sampler_t *&operator[](ekg::layer layer) {
      return this->samplers[static_cast<size_t>(layer)];
    }
  };

  template<size_t size>
  struct layer_t {
  protected:
    std::array<ekg::layer_group_t, size> layer_groups {};
  public:
    ekg::sampler_t *&operator[](ekg::layer layer) {
      return this->layer_groups[0][layer];
    }

    ekg::layer_group_t &operator[](size_t group) {
      return this->layer_groups[group];
    }
  };

  ekg::flags_t gpu_allocate_sampler(
    ekg::sampler_allocate_info_t *p_sampler_allocate_info,
    ekg::sampler_t *p_sampler
  );

  ekg::flags_t gpu_fill_sampler(
    ekg::sampler_fill_info_t *p_sampler_fill_info,
    ekg::sampler_t *p_sampler
  );

  ekg::flags_t image_src_r8_convert_to_r8g8b8a8(
    ekg::vec2_t<int32_t> size,
    const unsigned char *p_src,
    std::vector<unsigned char> &dst
  );
}

namespace ekg::io {
  struct gpu_data_t {
  public:
    float buffer_content[12] {};
    int32_t sampler_index {-1};
    int8_t line_thickness {};
    int32_t begin_stride {};
    int32_t end_stride {};
    int32_t factor {};
    int32_t scissor_id {-1};
  };
}

#endif
