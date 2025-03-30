#ifndef EKG_UI_BUTTON_HPP
#define EKG_UI_BUTTON_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/typography.hpp"
#include "ekg/io/gpu.hpp"
#include "ekg/ui/types.hpp"

namespace ekg {
  struct button_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> active_outline {};
    ekg::vec4_t<float> highlight {};

    /**
     * [0] ekg::button_t
     **/
    ekg::layer_t<1> layers {};
  };

  struct button_t {
  public:
    std::string tag {};
    ekg::flags_t dock {ekg::dock::left};
    ekg::value<std::string> text {};
    ekg::flags_t text_dock {ekg::dock::left};
    ekg::value<bool> value {};
    ekg::font text_font_size {ekg::font::normal};
    ekg::rect_t<float> rect {};
    ekg::type type {ekg::type::button};
    ekg::button_theme_t theme {};
  };
}

#endif
