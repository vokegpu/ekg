#ifndef EKG_UI_TEXTBOX_HPP
#define EKG_UI_TEXTBOX_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct textbox_theme_t {
  public:
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> cursor {};
    ekg::vec4_t<float> select {};
  };

  struct textbox_t {
  public:
  };
}

#endif
