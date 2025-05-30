#ifndef EKG_CORE_CONTEXT_HPP
#define EKG_CORE_CONTEXT_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct context_t {
  public:
    ekg::rect_t<float> viewport {};
    float dt {};
  };  
}

#endif
