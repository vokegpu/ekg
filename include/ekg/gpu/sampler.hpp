#ifndef EKG_GPU_SAMPLER_HPP
#define EKG_GPU_SAMPLER_HPP

#include "ekg/io/memory.hpp"
#include "ekg/io/descriptor.hpp"

namespace ekg {
  struct sampler_t {
  public:
    static constexpr ekg::type type {ekg::type::sampler};
    static ekg::sampler_t not_found;
  public:
    const char *p_tag {};
    uint32_t w {};
    uint32_t h {};
    uint32_t channel {};
    uint32_t gl_id {};
    int8_t gl_protected_active_index {-1};
    bool is_protected {};
  public:
    EKG_DESCRIPTOR(ekg::sampler_t);
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
    void *pv_data {};
  };

  typedef sampler_info_t sampler_allocate_info_t;
  typedef sampler_info_t sampler_fill_info_t;
}

#endif
