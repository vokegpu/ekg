#ifndef EKG_IO_DESIGN_HPP
#define EKG_IO_DESIGN_HPP

#include <map>

#include "ekg/ui/button/button.hpp"
#include "ekg/ui/checkbox/checkbox.hpp"
#include "ekg/ui/combobox/combobox.hpp"
#include "ekg/ui/frame/frame.hpp"
#include "ekg/ui/label/label.hpp"
#include "ekg/ui/listbox/listbox.hpp"
#include "ekg/ui/menu/menu.hpp"
#include "ekg/ui/popup/popup.hpp"
#include "ekg/ui/scrollbar/scrollbar.hpp"
#include "ekg/ui/slider/slider.hpp"
#include "ekg/ui/textbox/textbox.hpp"

namespace ekg {
  struct theme_t {
  public:
    std::string_view name {};
    std::string_view author {};
    std::string_view description {};
    std::string_view path {};
  public:
    ekg::pixel_t layout_offset {2.0f};
    ekg::pixel_thickness_t layout_margin_thickness {2};

    ekg::button_theme_t button {};
    ekg::checkbox_theme_t checkbox {};
    ekg::combobox_theme_t combobox {};
    ekg::frame_theme_t frame {};
    ekg::label_theme_t label {};
    ekg::listbox_theme_t listbox {};
    ekg::menu_theme_t menu {};
    ekg::popup_theme_t popup {};
    ekg::scrollbar_theme_t scrollbar {};
    ekg::slider_theme_t slider {};
    ekg::textbox_theme_t textbox {};
  };

  std::map<std::string_view, ekg::theme_t> &themes();
  ekg::theme_t &theme(std::string_view name = "");
  void theme(ekg::theme_t theme);
  ekg::flags_t set_current_theme(std::string_view name);
}

#endif
