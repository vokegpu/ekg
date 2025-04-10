#ifndef EKG_UI_TYPES_HPP
#define EKG_UI_TYPES_HPP

#include "ekg/math/geometry.hpp"

namespace ekg {
  enum type {
    abstract,
    button,
    checkbox,
    combobox,
    frame,
    label,
    listbox,
    menu,
    popup,
    scrollbar,
    slider,
    textbox
  };

  enum class level {
    bottom,
    top
  };
}

namespace ekg::ui {
  struct states_t {
  public:
    bool is_hovering {};
    bool is_highlighting {};

    /**
     * X if horizontal is scrolling;
     * Y if vertical is scrolling;
     * Z if horizontal is scrollable;
     * W if vertical is scrollable;
     **/
    ekg::vec4_t<bool> is_scrolling {};

    /**
     * The nearest-bar pixel-thicnkess;
     **/
    ekg::pixel_thickness_t nearest_scroll_bar_thickness {};

    bool is_active {};
    bool is_focused {};

    /**
     * Generic use-case reserved
     **/
    bool is[4] {};

    bool is_high_frequency {};
    bool is_targeting_absolute_parent {};
    bool is_absolute {};

    bool was_reloaded {};
    bool was_layout_docknized {};
    bool was_just_created {};
  };
}

#endif
