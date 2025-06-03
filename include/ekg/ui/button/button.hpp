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
#ifndef EKG_UI_BUTTON_HPP
#define EKG_UI_BUTTON_HPP

#include "ekg/io/memory.hpp"
#include "ekg/io/descriptor.hpp"

namespace ekg {
  struct button_color_scheme_t {
  public:
    ekg::rgba_t<uint8_t> text_foreground {};

    ekg::rgba_t<uint8_t> background {};
    ekg::rgba_t<uint8_t> outline {};
    ekg::rgba_t<uint8_t> highlight {};

    ekg::rgba_t<uint8_t> box_background {};
    ekg::rgba_t<uint8_t> box_outline {};
    ekg::rgba_t<uint8_t> box_highlight {};
    ekg::rgba_t<uint8_t> box_active {};
  };

  struct button_t {
  public:
    struct check_t {
    public:
      ekg::value<std::string> text {};
      bool is_check_box {};
      ekg::flags_t dock {};
    public:
      ekg::rect_t<float> rect_text {};
      ekg::rect_t<float> rect_box {};
    };

    static ekg::button_t not_found;
    static constexpr ekg::type type {ekg::type::button};
  public:
    ekg::at_t property_at {};
  public:
    ekg::rect_t<float> rect {};
    std::vector<ekg::button_t::check_t> checks {};
    ekg::button_color_scheme_t color_scheme {};
  public:
    ekg_descriptor(ekg::button_t);
  };
}

#endif
