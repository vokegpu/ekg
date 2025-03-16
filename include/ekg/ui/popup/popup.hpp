#ifndef EKG_UI_POPUP_HPP
#define EKG_UI_POPUP_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct popup_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> highlight {};
    ekg::vec4_t<float> separator {};
    int64_t drop_animation_delay {};
  };

  struct popup_t {
  public:
  };
}

#endif
