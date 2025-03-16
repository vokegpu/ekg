#ifndef EKG_UI_BUTTON_HPP
#define EKG_UI_BUTTON_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/memory.hpp"
#include "ekg/ui/abstract.hpp"
#include "ekg/io/typography.hpp"
#include <string>

namespace ekg {
  struct button_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> active_outline {};
    ekg::vec4_t<float> highlight {};
  };

  struct button_t {
  public:
    std::string tag {};
    ekg::value<bool> value {};
    ekg::flags_t dock {};
    ekg::rect_t<float> rect {};
    ekg::type type {ekg::type::button};
    std::string text {};
    ekg::flags_t text_dock {};
    ekg::font text_font_size {ekg::font::normal};
    ekg::button_theme_t theme {};
  };
}

#endif
