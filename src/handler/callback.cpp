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
#include "ekg/handler/callback.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/core/context.hpp"

ekg::callback_t ekg::callback_t::not_found {
  .at = ekg::at_t::not_found
};

void ekg::io::dispatch(
  ekg::io::operation op,
  ekg::at_t &property_at
) {
  ekg::property_t &property {
    ekg::query<ekg::property_t>(property_at)
  };

  if (
    op != ekg::io::operation::swap
    &&
    op != ekg::io::operation::scalenize
    &&
    property == ekg::property_t::not_found
  ) {
    return;
  }

  switch (op) {
  case ekg::io::operation::swap:
    if (property != ekg::property_t::not_found) {
      ekg::gui.bind.swap_at = property_at;
    }

    ekg::p_core->handler_callback.dispatch(
      static_cast<size_t>(op)
    );
    break;
  case ekg::io::operation::reload:
    if (property.operation.should_docknize) return;
    property.operation.should_reload = true;
    ekg::p_core->reload.push_back(property_at);
    ekg::p_core->handler_callback.dispatch(
      static_cast<size_t>(op)
    );
    break;
  case ekg::io::operation::docknize:
    if (property.operation.should_docknize) return;
    property.operation.should_docknize = true;
    ekg::p_core->docknize.push_back(property_at);
    ekg::p_core->handler_callback.dispatch(
      static_cast<size_t>(op)
    );
    break;
  case ekg::io::operation::scalenize:
    ekg::p_core->handler_callback.dispatch(
      static_cast<size_t>(op)
    );
    break;
  case ekg::io::operation::high_frequency:
    property.is_high_frequency = true; // make sure this tick is true
    if (property.operation.should_enable_high_frequency) return;
    property.operation.should_enable_high_frequency = true;
    ekg::p_core->high_frequency.push_back(property_at);
    break;
  }
}
