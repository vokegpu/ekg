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
#include "ekg/ui/popup/popup.hpp"
#include "ekg/ui/popup/widget.hpp"
#include "ekg/core/pools.hpp"

ekg::popup_t ekg::popup_t::not_found {
  .at = ekg::at_t::not_found
};

void ekg::show(
  ekg::at_t &popup_at,
  const ekg::vec2_t<float> &pos,
  bool should_if
) {
  ekg::popup_t &popup {
    ekg::query<ekg::popup_t>(popup_at)
  };

  if (!should_if || popup == ekg::popup_t::not_found) {
    return;
  }

  ekg::ui::splash_popup_just_opened(
    popup,
    pos
  );

  ekg::io::dispatch(
    ekg::io::operation::swap,
    popup.property_at
  );
}
