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
#include "ekg/ui/abstract.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/core/runtime.hpp"

ekg::rect_t<float> &ekg::ui::get_abs_rect(
  ekg::property_t &property,
  ekg::rect_t<float> &descriptor_rect
) {
  ekg::property_t &parent {
    ekg::query<ekg::property_t>(property.parent_at)
  };

  return (
    property.widget.rect = (
      descriptor_rect + parent.widget.rect + parent.scroll.position
    )
  );
}


void ekg::ui::pre_event(
  ekg::property_t &property,
  ekg::rect_t<float> &descriptor_rect,
  bool is_top_level
) {
  ekg::input_info_t &input {ekg::p_core->handler_input.input};
  if (
    input.was_pressed || input.was_released ||
    input.has_motion  || input.was_wheel
  ) {
    ekg::rect_t<float> &abs {ekg::ui::get_abs_rect(property, descriptor_rect)};
    ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};

    property.widget.is_hovering = (
      ekg::rect_collide_vec2<float>(abs, interact)
      &&
      (
        is_top_level
        ||
        property.parent_at == ekg::at_t::not_found
        ||
        ekg::rect_collide_vec2<float>(property.widget.rect_scissor, interact)
      )
    );
  }
}

void ekg::ui::post_event(
  ekg::property_t &property
) {
  property.widget.is_hovering = false;

  #if defined(__ANDROID__)
    property.widget.is_highlight = (
      !(!property.widget.is_hovering && ekg::p_core->handler_input.input.was_released)
      &&
      property.widget.is_highlight
    );
  #endif
}
