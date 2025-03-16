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
    top,
    bottom
  };
}

namespace ekg::ui {
  struct states_t {
  public:
    /**
     * Generic use-case reserved
     **/
    bool is[4] {};

    bool is_hover {};
    bool is_active {};
    bool is_absolute {};
    bool is_highlighting {};
    bool is_focusing {};
    ekg::vec2_t<bool> is_scrolling {};
    bool is_high_frequency {};
    bool is_targeting_absolute_parent {};

    bool was_reloaded {};
    bool was_layout_docknized {};
    bool was_just_created {};
  };
}

#endif
