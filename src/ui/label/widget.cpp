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
#include "ekg/ui/label/widget.hpp"

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::label_t &label
) {
  ekg::ui::get_abs_rect(
    property,
    button.rect
  );

  ekg::axis pick_axis {
    ekg::axis::horizontal
  };

  label.rect.scaled_height = ekg::max<ekg::pixel_thickness_t>(1, label.rect.scaled_height);

  if (property.widget.should_refresh_size) {
    label.rect.h = ekg::min();
    property.widget.should_refresh_size = false;
  }

  ekg::layout::mask mask {};
  mask.preset(
    {
      aligned_dimension.offset,
      aligned_dimension.offset,
      label.rect.h
    },
    pick_axis,
    label.rect.w
  );

  mask.docknize();
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::label_t &label,
  const ekg::io::stage &stage
) {

}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::label_t &label
) {

}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::label_t &label
) {

}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::label_t &label
) {

}

void ekg::ui::unmap(
  ekg::label_t &label
) {
  label.text.ownership(nullptr);
}
