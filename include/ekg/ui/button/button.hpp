#ifndef EKG_UI_BUTTON_HPP
#define EKG_UI_BUTTON_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/memory.hpp"
#include "ekg/ui/abstract.hpp"
#include <string>

namespace ekg {
  struct button_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> string {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> highlight {};
  };

  struct button_t {
  public:
    std::string tag {};
    ekg::value<bool> value {};
    ekg::flags_t dock {};
    ekg::type type {ekg::type::button};
    std::string text {};
    ekg::flags_t text_dock {};
    ekg::rect_t<float> rect {};
  };
}

#endif
