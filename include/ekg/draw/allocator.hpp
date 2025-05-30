#ifndef EKG_DRAW_ALLOCATOR_HPP
#define EKG_DRAW_ALLOCATOR_HPP

#include <vector>

#include "ekg/math/geometry.hpp"
#include "ekg/gpu/data.hpp"
#include "ekg/gpu/sampler.hpp"

namespace ekg {
  class allocator {
  protected:
    size_t data_instance {};
    ekg::vec2_t<size_t> stride_instance {};
    size_t simple_shape_instance {};
    size_t geometry_instance {};
    ekg::rect_t<float> scissor_instance {};

    std::vector<ekg::gpu::data_t> gpu_data_buffer {};
    std::vector<ekg::gpu::data_t> loaded_high_priority_data_list {};
    std::vector<float> geometry_resource_list {};
  public:
    void invoke();
    void revoke();

    void push_back_geometry(
      const ekg::vec2_t<float> &position,
      const ekg::vec2_t<float> &uv
    );
  };
}

#endif
