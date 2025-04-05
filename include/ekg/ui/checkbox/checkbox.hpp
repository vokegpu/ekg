#ifndef EKG_UI_CHECKBOX_HPP
#define EKG_UI_CHECKBOX_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/gpu.hpp"
#include "ekg/io/typography.hpp"
#include "ekg/ui/types.hpp"

namespace ekg {
  struct checkbox_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> highlight {};
    ekg::vec4_t<float> box_background {};
    ekg::vec4_t<float> box_outline {};
    ekg::vec4_t<float> box_highlight {};
    ekg::vec4_t<float> box_active {};

    /**
     * [0] ekg::checkbox_t
     * [1] ekg::checkbox_t::box
     **/
    ekg::layer_t<2> layers {};
  };

  struct checkbox_t {
  public:
    constexpr static ekg::flags_t box {1};
  public:
    std::string tag {};
    ekg::flags_t dock {};
    ekg::value<std::string> text {};
    ekg::flags_t text_dock {ekg::dock::left};
    ekg::value<bool> value {};
    ekg::font text_font_size {ekg::font::normal};
    ekg::flags_t box_dock {ekg::dock::left};
    ekg::rect_t<float> rect {};
    ekg::type type {ekg::type::checkbox};
    ekg::checkbox_theme_t theme {};
  };
}

#endif
