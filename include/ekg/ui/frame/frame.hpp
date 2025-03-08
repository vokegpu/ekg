#ifndef EKG_UI_FRAME_HPP
#define EKG_UI_FRAME_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/ui/properties.hpp"

namespace ekg {
  struct frame_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> border {};
    ekg::vec4_t<float> outline {};
    int32_t activity_offset {};
  };

  struct frame_t {
  public:
    std::string tag {};
    ekg::flags_t dock {};
    ekg::flags_t drag_dock {};
    ekg::flags_t resize_dock {};
    ekg::type type {ekg::type::frame};
    ekg::rect_t<float> rect {};
    ekg::top_level_t top_level {};
    ekg::frame_theme_t theme {};
  public:
    ekg::properties_t *__unsafe_p_properties {};
  public:
    operator ekg::top_level_t() {
      return this->__unsafe_p_properties;
    }
  };
}

#endif
