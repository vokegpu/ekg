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
#ifndef EKG_CORE_CONTEXT_HPP
#define EKG_CORE_CONTEXT_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/descriptor.hpp"

namespace ekg {
  extern struct metrics_t {
  public:
    size_t gpu_data_count {};
  } metrics;

  extern struct dpi_t {
  public:
    ekg::rect_t<float> viewport {};
    ekg::rect_t<float> scale {0.0f, 0.0f, 1920.0f, 1080.0f};
    bool auto_scale {true};
    float scale_interval {25.0f};
    float factor_scale {};
    float font_scale {18.0f};
    float min_sizes {10.0f};
    ekg::vec2_t<uint32_t> font_offset {4, 6};
  } dpi;

  extern struct gui_t {
  public:
    struct bind_t {
    public:
      ekg::at_t stack_at {ekg::at_t::not_found};
      ekg::at_t swap_at {ekg::at_t::not_found};
      ekg::at_t parent_at {ekg::at_t::not_found};
    };

    // @TODO: add last for press, release and hover

    struct ui_t {
    public:
      ekg::at_t abs_widget_at {ekg::at_t::not_found};
      ekg::type hovered_type {};
      ekg::at_t hovered_at {ekg::at_t::not_found};
      ekg::at_t last_hovered_at {ekg::at_t::not_found};
      ekg::type pressed_type {};
      ekg::at_t pressed_at {ekg::at_t::not_found};
      ekg::type released_type {};
      ekg::at_t released_at {ekg::at_t::not_found};
      int64_t frequency {500};
      float dt {};
      bool redraw {};
    };
  public:
    ekg::gui_t::bind_t bind {};
    ekg::gui_t::ui_t ui {};
  } gui;

  constexpr uint32_t minimum_small_font_height {4};
  constexpr uint32_t minimum_font_height {8};
  constexpr uint32_t minimum_big_font_height {12};
}

#endif
