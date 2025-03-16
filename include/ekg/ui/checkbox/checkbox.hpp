#ifndef EKG_UI_CHECKBOX_HPP
#define EKG_UI_CHECKBOX_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct checkbox_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> highlight {};
  };

  struct checkbox_t {
  public:
  };
}

#endif
