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

#ifndef EKG_IO_SAFETY_HPP
#define EKG_IO_SAFETY_HPP

#include "ekg/ekg.hpp"

namespace ekg::io {
  template<typename t>
  t *new_widget_instance() {
    return dynamic_cast<t*>(
      ekg::core->emplace_back_new_widget_safety(
        dynamic_cast<ekg::ui::abstract*>(new t {})
      ).get()
    );
  }

  template<typename t>
  ekg::ui::abstract *ekg::make(t descriptor) {
    ekg::ui::abstract *p_created_widget {
      nullptr
    };

    ekg::properties_t properties {
      .tag = descriptor.tag,
      .type = descriptor.type,
      .unique_id = ekg::core->generate_unique_id()
      .is_alive = true
    };

    switch (descriptor.type) {
      case ekg::type::button: {
        ekg::button_t &button {
          ekg::io::any_static_cast<ekg::button_t>(
            descriptor
          )
        };

        ekg::ui::button *p_button {
          ekg::io::new_widget_instance<ekg::ui::button>()
        };

        p_button->descriptor = button;
        p_created_widget = p_button;

        properties.descriptor = &p_button->descriptor;
        properties.p_widget = &p_button;
        properties.dock = button.dock; // i mean im dumb idk

        break;
      }

      case ekg::type::frame: {
        ekg::frame_t &frame {
          ekg::io::any_static_cast<ekg::frame_t>(
            descriptor
          )
        };

        ekg::ui::frame *p_frame {
          ekg::io::new_widget_instance<ekg::ui::frame>()
        };

        p_frame->descriptor = frame;
        p_frame->descriptor.__unsafe_p_properties = p_frame->properties;

        p_created_widget = p_frame;

        properties.descriptor = &p_frame->descriptor;
        properties.p_widget = &p_frame;
        properties.dock = frame.dock;
        properties.is_docknizable = true;

        break;
      }
    }

    p_created_widget->properties = properties;
    
    ekg::properties_t *p_current_parent_proprerties {
      ekg::core->get_current_parent_properties()
    };

    if (
      p_created_widget->properties.is_parentable
      &&
      p_current_parent_properties != nullptr
    ) {
      ekg::add_child_to_parent(
        &p_current_parent_properties,
        &p_created_widget
      );
    }

    return p_created_widget;
  }
}

#endif
