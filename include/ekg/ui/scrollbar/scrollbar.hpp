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
#ifndef EKG_UI_SCROLLBAR_HPP
#define EKG_UI_SCROLLBAR_HPP

#include "ekg/ui/property.hpp"
#include "ekg/io/event.hpp"

namespace ekg {
  struct scrollbar_color_scheme_t {
  public:
    ekg::rgba_t<uint8_t> background {};
    ekg::rgba_t<uint8_t> outline {};

    ekg::rgba_t<uint8_t> bar_background {};
    ekg::rgba_t<uint8_t> bar_highlight {};
    ekg::rgba_t<uint8_t> bar_active {};
    ekg::rgba_t<uint8_t> bar_outline {};

    ekg::pixel_thickness_t bar_thickness {20};
    ekg::pixel_t bar_size_limit {20};
  };

  struct scrollbar_t {
  public:
    struct widget_t {
    public:
      ekg::property_t::states_t states_horizontal_bar {};
      ekg::property_t::states_t states_vertical_bar {};
      ekg::rect_t<float> rect_horizontal {};
      ekg::rect_t<float> rect_vertical {};
      ekg::rect_t<float> rect_delta {};
    };
  public:
    static constexpr ekg::type type {ekg::type::scrollbar};
    static ekg::scrollbar_t not_found;
  public:
    ekg::at_t property_at {};
  public:
    std::string tag {};
    ekg::rect_t<float> rect {};
    ekg::vec2_t<float> acceleration {};
    ekg::scrollbar_color_scheme_t color_scheme {};
    ekg::at_array_t<ekg::layer, ekg::enum_layer_size> layers {};
    ekg::at_array_t<ekg::action, ekg::enum_action_size> actions {};
    ekg::scrollbar_t::widget_t widget {};
    ekg::flags_t dock {}; // useless
  public:
    ekg_descriptor(ekg::scrollbar_t);
  };
}

#endif
