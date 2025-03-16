#ifndef EKG_UI_SLIDER_HPP
#define EKG_UI_SLIDER_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct slider_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> bar_background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> bar {};
    ekg::vec4_t<float> bar_outline {};
    ekg::vec4_t<float> highlight {};
    int32_t bar_thickness {};
    int32_t target_thickness {};
  };

  struct slider_t {
  public:
  };
}

#endif
