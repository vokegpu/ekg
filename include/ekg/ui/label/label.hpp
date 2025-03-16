#ifndef EKG_UI_LABEL_HPP
#define EKG_UI_LABEL_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct label_theme_t {
  public:
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> background {};
  };

  struct label_t {
  public:
  };
}

#endif
