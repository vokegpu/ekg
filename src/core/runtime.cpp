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
#include "ekg/core/runtime.hpp"

void ekg::core:swap_collector(
  bool &was_found,
  ekg::at_t &property_at
) {
  if (property_at == ekg::at_t::not_found) {
    return;
  }

  ekg::property_t &property {
    ekg::query<ekg::property_t>(property_at)
  };

  if (
    property.at == ekg::p_core->swap_target_at
  ) {
    was_found = true;
  }

  ekg::p_core->collector.push_back(property_at);

  if (property.is_childnizate && property.is_children_docknizable) {
    ekg_abstract_todo(
      property,
      for (ekg::property &property : descriptor.children) {
        if (property == ekg::property_t::not_found) {
          continue;
        }

        ekg::core::swap_collector(was_found, property);
      }
    );
  }
}

void ekg::core::swap(ekg::info_t &info) {
  if (
    ekg::p_core == nullptr
    ||
    ekg::p_core->swap_target_at == ekg::at_t::not_found
  ) {
    return;
  }

  bool was_found {};

  ekg::p_core->top_level_stack.clear();
  for (ekg::at_t &at : ekg::p_core->register) {
    ekg::property_t &property {ekg::query<ekg::property_t>(at)};

    if (
      property == ekg::property_t::not_found
      ||
      property.parent_at != ekg::at_t::not_found
    ) {
      continue;
    }

    // recurse
    ekg::p_core->collector.clear();
    ekg::core::swap_collector(was_found, at);

    if (ekg::p_core->swap_target_collector.was_target_found) {
      ekg::p_core->top_level_stack.insert(
        ekg::p_core->top_level_stack.begin(),
        ekg::p_core->collector.begin(),
        ekg::p_core->collector.end()
      );
    } else {
      ekg::p_core->stack.insert(
        ekg::p_core.stack.begin(),
        ekg::p_core->collector.begin(),
        ekg::p_core->collector.end()
      );
    }
  }

  ekg::p_core->stack.insert(
    ekg::p_core->stack.end(),
    ekg::p_core->top_level_stack.begin(),
    ekg::p_core->top_level_stack.end(),
  );

  ekg::p_core->collector.clear();
  ekg::p_core->swap_target_at = ekg::at_t::not_found;
}

void ekg::core::reload(ekg::info_t &info) {
  if (ekg::p_core == nullptr) {
    return;
  }

  for (ekg::at_t &at : ekg::p_core->reload) {
    ekg::property_t &property {ekg::query<ekg::property_t>(at)};
    if (property == ekg::property_t::not_found) {
      continue;
    }

    ekg_abstract_todo(
      property,
      ekg::ui::reload(property, descriptor);
    );
  }

  ekg::p_core->reload.clear();
}

void ekg::core::docknize(ekg::info_t &info) {
  if (ekg::p_core == nullptr) {
    return;
  }

  for (ekg::at_t &at : ekg::p_core->reload) {
    ekg::property_t &property {ekg::query<ekg::property_t>(at)};
    if (property == ekg::property_t::not_found) {
      continue;
    }

    // add docknize
  }

  ekg::p_core->reload.clear();
}

void ekg::core::scale(ekg::info_t &info) {
  if (ekg::p_core == nullptr) {
    return;
  }

  // add scale
}

void ekg::core::poll_events() {
  if (ekg::p_core == nullptr) {
    return;
  }  
}
