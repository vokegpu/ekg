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
#ifndef EKG_UI_PROPERTY_HPP
#define EKG_UI_PROPERTY_HPP

#include "ekg/io/descriptor.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg {
  struct operation_t {
  public:
    bool should_reload {};
    bool should_docknize {};
    bool should_enable_high_frequency {};
  };

  struct scroll_t {
  public:
    ekg::vec2_t<bool> is_enabled {};
    ekg::vec2_t<bool> is_scrolling {};
    ekg::pixel_thickness_t nearest_scroll_bar_thickness {};
  };

  struct property_t {
  public:
    static ekg::property_t not_found;
    static constexpr ekg::type type {ekg::type::property};
  public:
    ekg::at_t parent_at {ekg::at_t::not_found};
    ekg::at_t abs_parent_at {ekg::at_t::not_found};
    ekg::at_t descriptor_at {};
    std::vector<ekg::at_t> children {};
  public:
    ekg::rect_t<float> rect {};

    bool is_childnizate {};
    bool is_children_docknizable {};
    bool is_targeting_absolute_parent {};
    bool is_absolute {};
    bool is_hovering {};
    bool is_visible {};
    bool is_enabled {};
    bool is_high_frequency {};
    bool should_refresh_size {};
    bool should_buffering {};

    ekg::scroll_t scroll {};
    ekg::operation_t operation {};
  public:
    ekg_descriptor(ekg::property_t);
  };
}

#endif
