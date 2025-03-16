/**
 * MIT License
 * 
 * Copyright (c) 2022-2024 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"); to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ekg/service/theme.hpp"
#include "ekg/io/log.hpp"

std::map<std::string_view, ekg::theme_t> &ekg::service::theme::get_theme_map() {
  return this->theme_map;
}

void ekg::service::theme::add(ekg::theme_t theme) {
  this->theme_map[theme.name] = theme;
}

ekg::flags_t ekg::service::theme::set_current_theme(std::string_view name) {
  if (this->current_theme.name == name) {
    return ekg::result::success;
  }

  if (!this->theme_map.count(name)) {
    ekg::log() << "Could not to find theme named '" << name << "'!";
    return ekg::result::could_not_find;
  }

  this->current_theme = this->theme_map[name];
  return ekg::result::success;
}

ekg::theme_t &ekg::service::theme::get_current_theme() {
  return this->current_theme;
}

void ekg::service::theme::init() {
  ekg::log() << "Initialising theme-service theme-scheme based!";

  ekg::theme_t dark_theme {
    .name = "dark",
    .author = "Rina Wilk",
    .description = "Pasted dark-theme... mwm",
  };

  dark_theme.symmetric_layout = true;
  dark_theme.frame.background = ekg::color<int32_t>(43, 43, 43, 255);
  dark_theme.frame.border = ekg::color<int32_t>(190, 190, 190, 0);
  dark_theme.frame.outline = ekg::color<int32_t>(30, 40, 60, 100);
  dark_theme.frame.margin_actions_offset = 18;
  dark_theme.button.background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_theme.button.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.button.outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_theme.button.active = ekg::color<int32_t>(44, 166, 255, 100);
  dark_theme.button.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.checkbox.background = ekg::color<int32_t>(85, 85, 85, 0);
  dark_theme.checkbox.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.checkbox.outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_theme.checkbox.active = ekg::color<int32_t>(44, 166, 255, 200);
  dark_theme.checkbox.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.slider.background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_theme.slider.bar_background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_theme.slider.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.slider.outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_theme.slider.active = ekg::color<int32_t>(44, 166, 255, 200);
  dark_theme.slider.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.slider.bar = ekg::color<int32_t>(44, 166, 255, 200);
  dark_theme.slider.bar_outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_theme.slider.bar_thickness = 100;
  dark_theme.slider.target_thickness = 0;
  dark_theme.label.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.popup.background = ekg::color<int32_t>(43, 43, 43, 255);
  dark_theme.popup.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.popup.outline = ekg::color<int32_t>(43, 43, 43, 0);
  dark_theme.popup.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.popup.separator = ekg::color<int32_t>(141, 141, 141, 50);
  dark_theme.popup.drop_animation_delay = 120;
  dark_theme.textbox.background = ekg::color<int32_t>(242, 242, 242, 255);
  dark_theme.textbox.text = ekg::color<int32_t>(141, 141, 141, 255);
  dark_theme.textbox.outline = ekg::color<int32_t>(141, 141, 141, 50);
  dark_theme.textbox.cursor = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.textbox.select = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.scrollbar.background = ekg::color<int32_t>(85, 85, 85, 255);
  dark_theme.scrollbar.outline = ekg::color<int32_t>(202, 207, 222, 150);
  dark_theme.scrollbar.bar = ekg::color<int32_t>(44, 166, 255, 200);
  dark_theme.scrollbar.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.scrollbar.pixel_thickness = 4;
  dark_theme.scrollbar.min_bar_size = 30.0f;
  dark_theme.listbox.header_background = ekg::color<int32_t>(85, 85, 85, 255);
  dark_theme.listbox.item_highlight_outline = ekg::color<int32_t>(141, 141, 141, 0);
  dark_theme.listbox.header_highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.listbox.header_outline = ekg::color<int32_t>(141, 141, 141, 100);
  dark_theme.listbox.header_string = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.listbox.item_outline = ekg::color<int32_t>(141, 141, 141, 0);
  dark_theme.listbox.item_background = ekg::color<int32_t>(85, 85, 85, 0);
  dark_theme.listbox.item_string = ekg::color<int32_t>(202, 202, 202, 255);
  dark_theme.listbox.item_highlight = ekg::color<int32_t>(44, 166, 255, 50);
  dark_theme.listbox.item_focused_outline = ekg::color<int32_t>(141, 141, 141, 0);
  dark_theme.listbox.item_focused = ekg::color<int32_t>(44, 166, 255, 100);
  dark_theme.listbox.line_separator = ekg::color<int32_t>(141, 141, 141, 100);
  dark_theme.listbox.outline = ekg::color<int32_t>(141, 141, 141, 100);
  dark_theme.listbox.background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_theme.listbox.drag_outline = ekg::color<int32_t>(141, 141, 141, 100);
  dark_theme.listbox.drag_background = ekg::color<int32_t>(85, 85, 85, 50);

  this->add(dark_theme);

  ekg::theme_t light_theme {
    .name = "light",
    .author = "Rina Wilk",
    .description = "Pasted light-theme... moow",
  };

  light_theme.symmetric_layout = true;
  light_theme.frame.background = ekg::color<int32_t>(242, 242, 242, 255);
  light_theme.frame.border = ekg::color<int32_t>(190, 190, 190, 0);
  light_theme.frame.outline = ekg::color<int32_t>(202, 207, 222, 150);
  light_theme.frame.margin_actions_offset = 18;
  light_theme.button.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.button.background = ekg::color<int32_t>(204, 204, 204, 50);
  light_theme.button.active = ekg::color<int32_t>(44, 166, 255, 100);
  light_theme.button.outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_theme.button.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.checkbox.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.checkbox.background = ekg::color<int32_t>(204, 204, 204, 0);
  light_theme.checkbox.active = ekg::color<int32_t>(44, 166, 255, 200);
  light_theme.checkbox.outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_theme.checkbox.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.slider.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.slider.background = ekg::color<int32_t>(204, 204, 204, 50);
  light_theme.slider.bar_background = ekg::color<int32_t>(204, 204, 204, 50);
  light_theme.slider.active = ekg::color<int32_t>(44, 166, 255, 200);
  light_theme.slider.outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_theme.slider.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.slider.bar = ekg::color<int32_t>(44, 166, 255, 200);
  light_theme.slider.bar_outline = ekg::color<int32_t>(44, 166, 255, 200);
  light_theme.slider.bar_thickness = 16;
  light_theme.slider.target_thickness = 0;
  light_theme.label.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.popup.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.popup.background = ekg::color<int32_t>(242, 242, 242, 255);
  light_theme.popup.outline = ekg::color<int32_t>(30, 40, 60, 0);
  light_theme.popup.highlight = ekg::color<int32_t>(206, 225, 239, 255);
  light_theme.popup.separator = ekg::color<int32_t>(202, 207, 222, 150);
  light_theme.popup.drop_animation_delay = 120,
  light_theme.textbox.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.textbox.background = ekg::color<int32_t>(242, 242, 242, 255);
  light_theme.textbox.outline = ekg::color<int32_t>(202, 207, 222, 150);
  light_theme.textbox.select = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.textbox.cursor = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.scrollbar.background = ekg::color<int32_t>(202, 202, 202, 255);
  light_theme.scrollbar.outline = ekg::color<int32_t>(202, 207, 222, 150);
  light_theme.scrollbar.bar = ekg::color<int32_t>(44, 166, 255, 200);
  light_theme.scrollbar.highlight = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.scrollbar.pixel_thickness = 4;
  light_theme.scrollbar.min_bar_size = 30.0f;
  light_theme.listbox.header_background = ekg::color<int32_t>(204, 204, 204, 255);
  light_theme.listbox.header_highlight = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.listbox.header_outline = ekg::color<int32_t>(202, 207, 222, 50);
  light_theme.listbox.header_string = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.listbox.item_background = ekg::color<int32_t>(204, 204, 204, 0);
  light_theme.listbox.item_string = ekg::color<int32_t>(141, 141, 141, 255);
  light_theme.listbox.item_outline = ekg::color<int32_t>(202, 207, 222, 50);
  light_theme.listbox.item_highlight_outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_theme.listbox.item_focused_outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_theme.listbox.item_focused = ekg::color<int32_t>(44, 166, 255, 100);
  light_theme.listbox.item_highlight = ekg::color<int32_t>(44, 166, 255, 50);
  light_theme.listbox.line_separator = ekg::color<int32_t>(202, 207, 222, 100);
  light_theme.listbox.background = ekg::color<int32_t>(204, 204, 204, 50);
  light_theme.listbox.drag_background = ekg::color<int32_t>(204, 204, 204, 50);
  light_theme.listbox.drag_outline = ekg::color<int32_t>(202, 207, 222, 100);
  light_theme.listbox.outline = ekg::color<int32_t>(202, 207, 222, 100);

  this->add(light_theme);
  this->set_current_theme("dark");

  ekg::theme_t light_pinky_theme {
    .name = "light-pinky",
    .author = "Rina Wilk",
    .description = "Pasted light-theme... moow",
  };

  light_pinky_theme.symmetric_layout = true;
  light_pinky_theme.frame.background = ekg::color<int32_t>(242, 242, 242, 255);
  light_pinky_theme.frame.border = ekg::color<int32_t>(190, 190, 190, 0);
  light_pinky_theme.frame.outline = ekg::color<int32_t>(202, 207, 222, 150);
  light_pinky_theme.frame.margin_actions_offset = 18;
  light_pinky_theme.button.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.button.background = ekg::color<int32_t>(204, 204, 204, 50);
  light_pinky_theme.button.active = ekg::color<int32_t>(245, 169, 184, 100);
  light_pinky_theme.button.outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_pinky_theme.button.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.checkbox.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.checkbox.background = ekg::color<int32_t>(204, 204, 204, 0);
  light_pinky_theme.checkbox.active = ekg::color<int32_t>(245, 169, 184, 200);
  light_pinky_theme.checkbox.outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_pinky_theme.checkbox.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.slider.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.slider.background = ekg::color<int32_t>(204, 204, 204, 50);
  light_pinky_theme.slider.bar_background = ekg::color<int32_t>(204, 204, 204, 50);
  light_pinky_theme.slider.active = ekg::color<int32_t>(245, 169, 184, 200);
  light_pinky_theme.slider.outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_pinky_theme.slider.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.slider.bar = ekg::color<int32_t>(245, 169, 184, 200);
  light_pinky_theme.slider.bar_outline = ekg::color<int32_t>(245, 169, 184, 200);
  light_pinky_theme.slider.bar_thickness = 16;
  light_pinky_theme.slider.target_thickness = 0;
  light_pinky_theme.label.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.popup.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.popup.background = ekg::color<int32_t>(242, 242, 242, 255);
  light_pinky_theme.popup.outline = ekg::color<int32_t>(30, 40, 60, 0);
  light_pinky_theme.popup.highlight = ekg::color<int32_t>(206, 225, 239, 255);
  light_pinky_theme.popup.separator = ekg::color<int32_t>(202, 207, 222, 150);
  light_pinky_theme.popup.drop_animation_delay = 120,
  light_pinky_theme.textbox.text = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.textbox.background = ekg::color<int32_t>(242, 242, 242, 255);
  light_pinky_theme.textbox.outline = ekg::color<int32_t>(202, 207, 222, 150);
  light_pinky_theme.textbox.select = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.textbox.cursor = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.scrollbar.background = ekg::color<int32_t>(202, 202, 202, 255);
  light_pinky_theme.scrollbar.outline = ekg::color<int32_t>(202, 207, 222, 150);
  light_pinky_theme.scrollbar.bar = ekg::color<int32_t>(245, 169, 184, 200);
  light_pinky_theme.scrollbar.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.scrollbar.pixel_thickness = 4;
  light_pinky_theme.scrollbar.min_bar_size = 30.0f;
  light_pinky_theme.listbox.header_background = ekg::color<int32_t>(204, 204, 204, 255);
  light_pinky_theme.listbox.header_highlight = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.listbox.header_outline = ekg::color<int32_t>(202, 207, 222, 50);
  light_pinky_theme.listbox.header_string = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.listbox.item_background = ekg::color<int32_t>(204, 204, 204, 0);
  light_pinky_theme.listbox.item_string = ekg::color<int32_t>(141, 141, 141, 255);
  light_pinky_theme.listbox.item_outline = ekg::color<int32_t>(202, 207, 222, 50);
  light_pinky_theme.listbox.item_highlight_outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_pinky_theme.listbox.item_focused_outline = ekg::color<int32_t>(202, 207, 222, 0);
  light_pinky_theme.listbox.item_focused = ekg::color<int32_t>(245, 169, 184, 100);
  light_pinky_theme.listbox.item_highlight = ekg::color<int32_t>(245, 169, 184, 50);
  light_pinky_theme.listbox.line_separator = ekg::color<int32_t>(202, 207, 222, 100);
  light_pinky_theme.listbox.background = ekg::color<int32_t>(204, 204, 204, 50);
  light_pinky_theme.listbox.drag_background = ekg::color<int32_t>(204, 204, 204, 50);
  light_pinky_theme.listbox.drag_outline = ekg::color<int32_t>(202, 207, 222, 100);
  light_pinky_theme.listbox.outline = ekg::color<int32_t>(202, 207, 222, 100);

  this->add(light_pinky_theme);

  ekg::theme_t dark_pinky_theme {
    .name = "dark-pinky",
    .author = "Rina Wilk",
    .description = "Pasted dark-theme... mooo mwm",
  };

  dark_pinky_theme.symmetric_layout = true;
  dark_pinky_theme.frame.background = ekg::color<int32_t>(43, 43, 43, 255);
  dark_pinky_theme.frame.border = ekg::color<int32_t>(190, 190, 190, 0);
  dark_pinky_theme.frame.outline = ekg::color<int32_t>(30, 40, 60, 100);
  dark_pinky_theme.frame.margin_actions_offset = 18;
  dark_pinky_theme.button.background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_pinky_theme.button.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.button.outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_pinky_theme.button.active = ekg::color<int32_t>(245, 169, 184, 100);
  dark_pinky_theme.button.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.checkbox.background = ekg::color<int32_t>(85, 85, 85, 0);
  dark_pinky_theme.checkbox.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.checkbox.outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_pinky_theme.checkbox.active = ekg::color<int32_t>(245, 169, 184, 200);
  dark_pinky_theme.checkbox.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.slider.background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_pinky_theme.slider.bar_background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_pinky_theme.slider.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.slider.outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_pinky_theme.slider.active = ekg::color<int32_t>(245, 169, 184, 200);
  dark_pinky_theme.slider.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.slider.bar = ekg::color<int32_t>(245, 169, 184, 200);
  dark_pinky_theme.slider.bar_outline = ekg::color<int32_t>(202, 207, 222, 0);
  dark_pinky_theme.slider.bar_thickness = 100;
  dark_pinky_theme.slider.target_thickness = 0;
  dark_pinky_theme.label.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.popup.background = ekg::color<int32_t>(43, 43, 43, 255);
  dark_pinky_theme.popup.text = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.popup.outline = ekg::color<int32_t>(43, 43, 43, 0);
  dark_pinky_theme.popup.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.popup.separator = ekg::color<int32_t>(141, 141, 141, 50);
  dark_pinky_theme.popup.drop_animation_delay = 120;
  dark_pinky_theme.textbox.background = ekg::color<int32_t>(242, 242, 242, 255);
  dark_pinky_theme.textbox.text = ekg::color<int32_t>(141, 141, 141, 255);
  dark_pinky_theme.textbox.outline = ekg::color<int32_t>(141, 141, 141, 50);
  dark_pinky_theme.textbox.cursor = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.textbox.select = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.scrollbar.background = ekg::color<int32_t>(85, 85, 85, 255);
  dark_pinky_theme.scrollbar.outline = ekg::color<int32_t>(202, 207, 222, 150);
  dark_pinky_theme.scrollbar.bar = ekg::color<int32_t>(245, 169, 184, 200);
  dark_pinky_theme.scrollbar.highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.scrollbar.pixel_thickness = 4;
  dark_pinky_theme.scrollbar.min_bar_size = 30.0f;
  dark_pinky_theme.listbox.header_background = ekg::color<int32_t>(85, 85, 85, 255);
  dark_pinky_theme.listbox.item_highlight_outline = ekg::color<int32_t>(141, 141, 141, 0);
  dark_pinky_theme.listbox.header_highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.listbox.header_outline = ekg::color<int32_t>(141, 141, 141, 100);
  dark_pinky_theme.listbox.header_string = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.listbox.item_outline = ekg::color<int32_t>(141, 141, 141, 0);
  dark_pinky_theme.listbox.item_background = ekg::color<int32_t>(85, 85, 85, 0);
  dark_pinky_theme.listbox.item_string = ekg::color<int32_t>(202, 202, 202, 255);
  dark_pinky_theme.listbox.item_highlight = ekg::color<int32_t>(245, 169, 184, 50);
  dark_pinky_theme.listbox.item_focused_outline = ekg::color<int32_t>(141, 141, 141, 0);
  dark_pinky_theme.listbox.item_focused = ekg::color<int32_t>(245, 169, 184, 100);
  dark_pinky_theme.listbox.line_separator = ekg::color<int32_t>(141, 141, 141, 100);
  dark_pinky_theme.listbox.outline = ekg::color<int32_t>(141, 141, 141, 100);
  dark_pinky_theme.listbox.background = ekg::color<int32_t>(85, 85, 85, 50);
  dark_pinky_theme.listbox.drag_outline = ekg::color<int32_t>(141, 141, 141, 100);
  dark_pinky_theme.listbox.drag_background = ekg::color<int32_t>(85, 85, 85, 50);

  this->add(dark_pinky_theme);
}

void ekg::service::theme::quit() {
  ekg::log() << "Quitting theme-service";
}
