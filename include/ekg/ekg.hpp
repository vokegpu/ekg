#ifndef EKG_HPP
#define EKG_HPP

#include "ekg/io/memory.hpp"
#include "ekg/core/runtime.hpp"

namespace ekg {
  extern ekg::runtime_t *p_core;

  ekg::flags_t init(
    ekg::ekg_runtime_properties_info_t &runtime_properties_info,
    ekg::runtime_t *p_runtime
  );

  void quit();
  void update();
  void render();
}

#endif
