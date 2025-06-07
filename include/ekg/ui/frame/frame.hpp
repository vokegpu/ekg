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
#ifndef EKG_UI_FRAME_HPP
#define EKG_UI_FRAME_HPP

#include "ekg/io/descriptor.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg {
  struct frame_color_scheme_t {
  public:
    ekg::pixel_thickness_t actions_margin_pixel_thickness {5};
    ekg::rgba_t<uint8_t> background {};
    ekg::rgba_t<uint8_t> highlight {};
    ekg::rgba_t<uint8_t> outline {};
    ekg::rgba_t<uint8_t> active {};
    ekg::rgba_t<uint8_t> focused_background {};
    ekg::rgba_t<uint8_t> focused_outline {};
    ekg::rgba_t<uint8_t> warning_outline {};
  };

  struct frame_t {
  public:
    struct widget_t {
    public:
      ekg::flags_t target_dock_drag {};
      ekg::flags_t target_dock_resize {};
      ekg::docker_t<float> docker_drag {};
      ekg::docker_t<float> docker_resize {};
      ekg::rect_t<float> rect_delta {};
      ekg::rect_t<float> rect_cache {};
    };
  public:
    static ekg::frame_t not_found;
    static constexpr ekg::type type {ekg::type::frame};
  public:
    ekg::at_t top_level_at {};
    ekg::at_t property_at {};
  public:
    std::string tag {};
    ekg::dock dock {ekg::dock::none};
    ekg::rect_t<float> rect {};
    ekg::flags_t drag {};
    ekg::flags_t resize {};
    bool set_top_level {};
    ekg::frame_color_scheme_t color_scheme {};
    ekg::frame_t::widget_t widget {};
  public:
    ekg_descriptor(ekg::frame_t);
  };
}

#endif
