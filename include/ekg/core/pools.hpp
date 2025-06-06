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
#include "ekg/ui/stack.hpp"

#include "ekg/ui/button/button.hpp"
#include "ekg/ui/button/widget.hpp"

#include "ekg/ui/frame/frame.hpp"
#include "ekg/ui/frame/widget.hpp"

namespace ekg::core {
  void registry(ekg::property_t &property);
}

namespace ekg {
  extern struct pools_t {
  public:
    ekg::pool<ekg::stack_t> stack {};
    ekg::pool<ekg::callback_t> callback {};
    ekg::pool<ekg::sampler_t> sampler {};

    ekg::pool<ekg::property_t> button_property {};
    ekg::pool<ekg::button_t> button {};

    ekg::pool<ekg::property_t> frame_property {};
    ekg::pool<ekg::frame_t> frame {};
  } pools;

  template<typename t>
  t &make(
    t descriptor
  ) {
    switch (t::type) {
    case ekg::type::stack:
      return ekg::io::any_static_cast<t>( 
        &ekg::pools.stack.push_back(
          ekg::io::any_static_cast<ekg::stack_t>(&descriptor)
        )
      );
    case ekg::type::callback:
      return ekg::io::any_static_cast<t>( 
        &ekg::pools.callback.push_back(
          ekg::io::any_static_cast<ekg::callback_t>(&descriptor)
        )
      );
    case ekg::type::sampler:
      return ekg::io::any_static_cast<t>( 
        &ekg::pools.sampler.push_back(
          ekg::io::any_static_cast<ekg::sampler_t>(&descriptor)
        )
      );
    case ekg::type::button: {
        ekg::button_t &button {
          ekg::pools.button.push_back(
            ekg::io::any_static_cast<ekg::button_t>(&descriptor)
          )
        };

        ekg::property_t &property {
          ekg::pools.button_property.push_back({})
        };

        property.widget.is_childnizate = false;
        property.widget.is_children_docknizable = false;

        button.at.flags = t::type;
        property.descriptor_at = button.at;

        property.at.flags = t::type;
        button.property_at = property.at;

        ekg::core::registry(property);
        return ekg::io::any_static_cast<t>(&button);
      }
    case ekg::type::frame: {
        ekg::frame_t &frame {
          ekg::pools.frame.push_back(
            ekg::io::any_static_cast<ekg::frame_t>(&descriptor)
          )
        };

        ekg::property_t &property {
          ekg::pools.frame_property.push_back({})
        };

        property.widget.is_childnizate = true;
        property.widget.is_children_docknizable = true;

        frame.at.flags = t::type;
        property.descriptor_at = frame.at;

        property.at.flags = t::type;
        frame.property_at = property.at;

        ekg::core::registry(property);
        return ekg::io::any_static_cast<t>(&frame);
      }
    }

    return t::not_found;
  }

  template<typename t>
  t &query(
    ekg::at_t &at
  ) {
    ekg::pool<ekg::property_t> *p_property_pool {nullptr};
    switch (t::type) {
    case ekg::type::stack:
      return ekg::io::any_static_cast<t>(
        &ekg::pools.sampler.query(at)
      );
    case ekg::type::callback:
      return ekg::io::any_static_cast<t>(
        &ekg::pools.callback.query(at)
      );
    case ekg::type::sampler:
      return ekg::io::any_static_cast<t>(
        &ekg::pools.sampler.query(at)
      );
    case ekg::type::property:
      switch (at.flags) {
      case ekg::type::button:
        return ekg::io::any_static_cast<t>(
          &ekg::pools.button_property.query(at)
        );
      case ekg::type::frame:
        return ekg::io::any_static_cast<t>(
          &ekg::pools.frame_property.query(at)
        );
      }
    case ekg::type::button:
      return ekg::io::any_static_cast<t>(
        &ekg::pools.button.query(at)
      );
    case ekg::type::frame:
      return ekg::io::any_static_cast<t>(
        &ekg::pools.frame.query(at)
      );
    }

    return t::not_found;
  }
}

#define ekg_abstract_todo(ekg_abstract_todo_at_type, ekg_abstract_todo_at, ekg_abstract_todo_todo) \
  switch (ekg_abstract_todo_at_type) { \
    case ekg::type::button: { \
      ekg::button_t &descriptor { \
        ekg::query<ekg::button_t>(ekg_abstract_todo_at) \
      }; \
      if (descriptor == ekg::button_t::not_found) { \
        break; \
      } \
      ekg_abstract_todo_todo \
      break; \
    } \
    case ekg::type::frame: { \
      ekg::frame_t &descriptor { \
        ekg::query<ekg::frame_t>(ekg_abstract_todo_at) \
      }; \
      if (descriptor == ekg::frame_t::not_found) { \
        break; \
      } \
      ekg_abstract_todo_todo \
      break; \
    } \
  }

#endif
