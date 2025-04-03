#ifndef EKG_UI_SCROLLBAR_HPP
#define EKG_UI_SCROLLBAR_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/ui/properties.hpp"

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
    std::string tag {};
    ekg::vec2_t<bool> axis {true, true};
    std::vector<ekg::properties_t*> *p_binded_children {};
    ekg::rect_t<float> *p_binded_rect {};
    ekg::type type {ekg::type::scrollbar};
  };
}

#endif
