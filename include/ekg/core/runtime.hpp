#ifndef EKG_CORE_RUNTIME_HPP
#define EKG_CORE_RUNTIME_HPP

#include "ekg/platform/base.hpp"
#include "ekg/gpu/api.hpp"

namespace ekg {
  struct ekg_runtime_properties_info_t {
  public:
    ekg::platform::base *p_platform_base {};
    ekg::gpu::api *p_gpu_api {};
  };

  struct runtime_t {
  public:
    ekg::platform::base *p_platform_base {};
    ekg::gpu::api *p_gpu_api {};
  public:
    ekg::input::handler input_handler {};
  };
}

namespace ekg::core {
  void poll_events();
}

#endif
