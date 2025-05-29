#ifndef EKG_GPU_DATA_HPP
#define EKG_GPU_DATA_HPP

#include "ekg/io/memory.hpp"

namespace ekg::gpu {
  struct data_t {
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
