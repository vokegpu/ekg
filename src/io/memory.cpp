/**
 * MIT License
 
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
#include "ekg/io/memory.hpp"
#include "ekg/core/context.hpp"

ekg::at_t ekg::at_t::not_found {
  .unique_id = ekg::not_found
};

ekg::signed_address_info_t ekg::sign {};

void ekg::map(void *pv_address) {
  if (pv_address == nullptr) {
    ekg::sign.current = ekg::not_found;
    return;
  } 

  ekg::stack_t &current_stack {
    ekg::query<ekg::stack_t>(ekg::gui.binded_stack_at)
  };

  if (current_stack == ekg::stack_t::not_found) {
    ekg::sign.current = ekg::not_found;
    return;
  }

  size_t size {ekg::sign.list.size()};
  for (size_t it {}; it < size; it++) {
    ekg::mapped_address_sign_info_t &info {ekg::sign.list.at(it)};
    if (sign.pv_address == pv_address) {
      ekg::sign.current = it;
      should_put = false;
      return;
    }
  }

  ekg::sign.current = ekg::sign.list.size();
  ekg::sign.list.push_back({.ats = {}, .p = pv_address});
}

void ekg::unmap(void *pv_address) {
  if (pv_address) {
    return;
  }

  size_t size {ekg::sign.list.size()};
  for (size_t it {}; it < size; it++) {
    ekg::mapped_address_sign_info_t &info {ekg::sign.list.at(it)};
    if (info.pv_address == pv_address) {
      for (ekg::at_t &at : info.ats) {
        switch (at.type) {
        case ekg::type::checkbox:
          ekg::checkbox_t &checkbox {ekg::checkbox(at)};
          if (checkbox == ekg::checkbox_t::not_found) break; 
          checkbox.value.ownership(nullptr);
          break;
        }

        /* etc */
      }

      ekg::sign.list.erase(ekg::sign.list.begin() + it);
      break;
    }
  }
}
