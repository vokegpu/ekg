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
#ifndef EKG_UI_POPUP_HPP
#define EKG_UI_POPUP_HPP

#include "ekg/ui/frame/frame.hpp"

namespace ekg {
  typedef ekg::frame_color_scheme_t popup_color_scheme_t;

  struct popup_t {
  public:
    struct widget_t {
    public:
      ekg::frame_t frame {};
      ekg::at_t popup_opened_at {ekg::at_t::not_found};
      bool was_visible {};
      bool just_opened {};
      bool should_self_recursive_destroy {};
    };

    struct link_t {
    public:
      std::string tag {};
      ekg::at_t popup_at {};
      ekg::at_t focused_widget_at {};
      std::vector<ekg::at_t> widget_ats {};
    };
  public:
    static ekg::popup_t not_found;
    static constexpr ekg::type type {ekg::type::popup};
    static constexpr ekg::flags_t auto_kill {2 << 2};
  public:
    ekg::at_t parent_popup_at {};
    ekg::at_t property_at {};
  public:
    std::string tag {};
    ekg::flags_t mode {};
    std::vector<ekg::popup_t::link_t> links {};
    ekg::rect_t<float> rect {.w = 200.0f};
    ekg::flags_t dock {};
    ekg::popup_color_scheme_t color_scheme {};
    ekg::popup_t::widget_t widget {};
  public:
    ekg_descriptor(ekg::popup_t);
  };

  void show(
    ekg::at_t &popup_at,
    const ekg::vec2_t<float> &pos,
    bool should_if = true
  );
}

#endif
