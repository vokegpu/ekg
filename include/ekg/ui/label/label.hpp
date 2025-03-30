#ifndef EKG_UI_LABEL_HPP
#define EKG_UI_LABEL_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/typography.hpp"
#include "ekg/io/gpu.hpp"
#include "ekg/ui/types.hpp"

namespace ekg {
  struct label_theme_t {
  public:
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> background {};

    /**
     * [0] ekg::label_t
     **/
    ekg::layer_t<1> layers {};
  };

  struct label_t {
  public:
    std::string tag {};
    ekg::flags_t dock {};
    ekg::value<std::string> text {};
    ekg::flags_t text_dock {ekg::dock::left};
    ekg::font text_font_size {ekg::font::normal};
    ekg::rect_t<float> rect {};
    ekg::type type {ekg::type::label};
    ekg::label_theme_t theme {};
  };
}

#endif
