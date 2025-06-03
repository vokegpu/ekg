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
#ifndef EKG_CORE_POOLS_HPP
#define EKG_CORE_POOLS_HPP

#include "ekg/io/memory.hpp"
#include "ekg/handler/callback.hpp"
#include "ekg/gpu/sampler.hpp"
#include "ekg/ui/property.hpp"

namespace ekg::core {
  void registry(ekg::property_t &property);
}

namespace ekg {
  extern struct pools_t {
  public:
    ekg::pool<ekg::stack_t> stack {ekg::stack_t::not_found};
    ekg::pool<ekg::callback_t> callback {ekg::callback_t::not_found};
    ekg::pool<ekg::sampler_t> sampler {ekg::sampler_t::not_found};
    ekg::pool<ekg::property> button_property {ekg::property::not_found};
    ekg::pool<ekg::button_t> button {ekg::button_t::not_found};
  } pools;

  template<typename t>
  t &make(
    t descriptor
  ) {
    switch (t::type) {
    case ekg::type::stack:
      return ekg::pools.stack.push_back(
        ekg::io::any_static_cast<ekg::sampler_t>(&descriptor)
      );
    case ekg::type::callback:
      return ekg::pools.callback.push_back(
        ekg::io::any_static_cast<ekg::callback_t>(&descriptor)
      );
    case ekg::type::sampler:
      return ekg::pools.sampler.push_back(
        ekg::io::any_static_cast<ekg::sampler_t>(&descriptor)
      );
    case ekg::type::button:
      ekg::button_t &button {
        ekg::pools.button.push_back(
          ekg::io::any_static_cast<ekg::button_t>(&descriptor)
        )
      };

      ekg::property_t &property {
        ekg::pools.button_property.push_back({})
      };

      property.is_childnizate = false;
      property.is_children_docknizable = false;

      button.at.flags = t::type;
      property.descriptor_at = button.at;

      property.at.flags = t::type;
      button.property_at = property.at;

      ekg::core::registry(property);
      return button;
    }

    return t::not_found;
  }

  template<typename t>
  t &query(
    ekg::at_t &at
  ) {
    switch (t::type) {`
      return ekg::io::any_static_cast<ekg::sampler_t>(
        &ekg::pools.sampler.query(at)
      );
    case ekg::type::callback:
      return ekg::io::any_static_cast<ekg::callback_t>(
        &ekg::pools.callback.query(at)
      );
    case ekg::type::sampler:
      return ekg::io::any_static_cast<ekg::sampler_t>(
        &ekg::pools.sampler.query(at)
      );
    case ekg::type::property:
      ekg::pool<ekg::property_t> *p_property_pool {nullptr};

      switch (at.type) {
      case ekg::type::button:
        p_property_pool = &ekg::pools.button_property;
        break;
      }

      if (p_property_pool == nullptr) {
        return t::not_found;
      }

      return p_property_pool.query(at);
    case ekg::type::button:
      return ekg::io::any_static_cast<ekg::button_t>(
        &ekg::pools.button.query(at)
      );
    }

    return t::not_found;
  }
}

#define ekg_abstract_todo(type, at, todo) \
  switch (type) { \
    case ekg::type::button: { \
      ekg::button_t &descriptor { \
        ekg::query<ekg::button_t>(at) \
      }; \
      if (descriptor == ekg::button_t::not_found) { \
        break; \
      } \
      todo \
      break; \
    } \
  } \

#endif
