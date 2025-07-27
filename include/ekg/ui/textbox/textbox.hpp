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
#ifndef EKG_UI_TEXTBOX_HPP
#define EKG_UI_TEXTBOX_HPP

#include "ekg/ui/scrollbar/scrollbar.hpp"
#include "ekg/io/descriptor.hpp"
#include "ekg/io/utf.hpp"
#include "ekg/io/font.hpp"

namespace ekg {
  struct textbox_color_scheme_t {
  public:
    ekg::rgba_t<float> background {};
    ekg::rgba_t<float> outline {};
    ekg::rgba_t<float> text_foreground {};
    ekg::rgba_t<float> text_select_foreground {};
    ekg::rgba_t<float> text_cursor_foreground {};
    bool caret_cursor {};
  };

  struct textbox_t {
  public:
    struct widget_t {
    public:
      ekg::rect_t<float> rect_text_size {};
      ekg::scrollbar_t scrollbar {};
    };

    static constexpr ekg::type type {ekg::type::textbox};
    static ekg::textbox_t not_found;
  public:
    ekg::at_t property_at {};
  public:
    std::string tag {};
    ekg::text text {};
    ekg::font font_size {ekg::font::medium};
    ekg::flags_t dock {};
    ekg::rect_t<float> rect {.w = 100.0f};
    ekg::textbox_color_scheme_t color_scheme {};
    ekg::textbox_t::widget_t widget {};
    ekg::at_array_t<ekg::layer, ekg::enum_layer_size> layers {};
  public:
    ekg_descriptor(ekg::textbox_t);
  };
}

#endif
