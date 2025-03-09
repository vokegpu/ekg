#ifndef EKG_UI_SCROLLBAR_HPP
#define EKG_UI_SCROLLBAR_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  struct scrollbar_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> bar {};
    ekg::vec4_t<float> highlight {};
    int32_t pixel_thickness {};
    float min_bar_size {};
  };

  struct scrollbar_t {
  public:
  };
}

#endif
