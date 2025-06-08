/**
 * MIT License
 * 
 * Copyright (c) 2022-2025 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
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
#include "ekg/handler/theme/handler.hpp"
#include "ekg/io/log.hpp"

void ekg::handler::theme::init() {
  ekg::log() << "Initializing default themes";

  ekg::theme_t light_pinky_theme {
    .tag = "light-pinky",
    .author = "Rina Wilk",
    .description = "Pasted light-theme... moow",
  };

  light_pinky_theme.frame_color_scheme.background = {242, 242, 242, 255};
  light_pinky_theme.frame_color_scheme.highlight = {242, 242, 242, 0};
  light_pinky_theme.frame_color_scheme.outline = {190, 190, 190, 0};
  light_pinky_theme.frame_color_scheme.active = {242, 242, 242, 0};
  light_pinky_theme.frame_color_scheme.focused_background = {242, 242, 242, 0};
  light_pinky_theme.frame_color_scheme.focused_outline = {242, 242, 242, 0};
  light_pinky_theme.frame_color_scheme.warning_outline = {242, 242, 0, 100};
  light_pinky_theme.frame_color_scheme.actions_margin_pixel_thickness = 18;

  light_pinky_theme.button_color_scheme.text_foreground = {141, 141, 141, 255};
  light_pinky_theme.button_color_scheme.background = {204, 204, 204, 50};
  light_pinky_theme.button_color_scheme.active = {245, 169, 184, 100};
  light_pinky_theme.button_color_scheme.outline = {202, 207, 222, 0};
  light_pinky_theme.button_color_scheme.highlight = {245, 169, 184, 50};
  light_pinky_theme.button_color_scheme.text_foreground = {141, 141, 141, 255};
  light_pinky_theme.button_color_scheme.box_outline = light_pinky_theme.button_color_scheme.outline;
  light_pinky_theme.button_color_scheme.box_active = {202, 207, 222, 50};
  light_pinky_theme.button_color_scheme.box_highlight = light_pinky_theme.button_color_scheme.highlight;
  light_pinky_theme.button_color_scheme.box_background = light_pinky_theme.button_color_scheme.background;

  this->registry(light_pinky_theme.tag) = light_pinky_theme;
  this->set_current_theme(light_pinky_theme.tag);
}

void ekg::handler::theme::quit() {
}

ekg::theme_t &ekg::handler::theme::registry(const std::string_view &tag) {
  return this->themes[tag];
}

ekg::theme_t &ekg::handler::theme::set_current_theme(const std::string_view &tag) {
  this->current_theme_tag = tag;
  return this->themes[tag];
}

ekg::theme_t &ekg::handler::theme::get_current_theme() {
  return this->themes[this->current_theme_tag];
}
