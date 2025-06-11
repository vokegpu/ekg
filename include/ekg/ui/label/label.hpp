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
#ifndef EKG_UI_LABEL_HPP
#define EKG_UI_LABEL_HPP

#include "ekg/io/descriptor.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/io/event.hpp"
#include "ekg/io/font.hpp"

namespace ekg {
  struct label_color_scheme_t {
  public:
    ekg::rgba_t<uint8_t> background {};
    ekg::rgba_t<uint8_t> outline {};
    ekg::rgba_t<uint8_t> text_foreground {};
  };

  struct label_t {
  public:
    static constexpr ekg::type type {ekg::type::label};
    static ekg::label_t not_found;
  public:
    std::string tag {};
    ekg::rect_t<float> rect {};
    ekg::value<std::string> text {};
    ekg::flags_t dock {};
    ekg::flags_t dock_text {};
    ekg::font font_size {};
    ekg::at_array_t<ekg::action, 12> actions {};
  public:
    ekg_descriptor(ekg::label_t);
  };
}

#endif
