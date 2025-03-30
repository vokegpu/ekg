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

#include "ekg/core/runtime.hpp"
#include "ekg/io/log.hpp"

#include "ekg/ui/button/button_widget.hpp"
#include "ekg/ui/frame/frame_widget.hpp"
#include "ekg/ui/label/label_widget.hpp"
#include "ekg/ui/checkbox/checkbox_widget.hpp"

namespace ekg::io {
  template<typename t>
  t *new_widget_instance() {
    return dynamic_cast<t*>(
      ekg::p_core->emplace_back_new_widget_safety(
        dynamic_cast<ekg::ui::abstract*>(new t {})
      )
    );
  }
}

namespace ekg {
  template<typename t>
  t &make(t descriptor) {
    ekg::ui::abstract *p_created_widget {
      nullptr
    };

    ekg::properties_t properties {
      .tag = descriptor.tag,
      .type = descriptor.type,
      .unique_id = ekg::p_core->generate_unique_id(),
      .is_enabled = true,
      .is_alive = true,
      .is_visible = true
    };

    ekg::theme_t &current_global_theme {ekg::p_core->service_theme.get_current_theme()};

    switch (descriptor.type) {
      case ekg::type::button: {
        ekg::button_t &button {
          ekg::io::any_static_cast<ekg::button_t>(
            &descriptor
          )
        };

        ekg::ui::button *p_button {
          ekg::io::new_widget_instance<ekg::ui::button>()
        };

        p_button->descriptor = button;
        p_button->descriptor.theme = current_global_theme.button;

        properties.p_descriptor = &p_button->descriptor;
        properties.p_widget = p_button;
        properties.dock = button.dock;
        properties.is_docknizable = false;
        properties.is_parentable = true;

        p_created_widget = dynamic_cast<ekg::ui::abstract*>(p_button);
        p_created_widget->p_descriptor_rect = &p_button->descriptor.rect;

        break;
      }

      case ekg::type::frame: {
        ekg::frame_t &frame {
          ekg::io::any_static_cast<ekg::frame_t>(
            &descriptor
          )
        };

        ekg::ui::frame *p_frame {
          ekg::io::new_widget_instance<ekg::ui::frame>()
        };

        p_frame->descriptor = frame;
        p_frame->descriptor.p_properties = &p_frame->properties;
        p_frame->descriptor.theme = current_global_theme.frame;

        properties.p_descriptor = &p_frame->descriptor;
        properties.p_widget = p_frame;
        properties.dock = frame.dock;
        properties.is_docknizable = true;
        properties.is_parentable = frame.dock != 0;

        p_created_widget = dynamic_cast<ekg::ui::abstract*>(p_frame);
        p_created_widget->p_descriptor_rect = &p_frame->descriptor.rect;

        break;
      }

      case ekg::type::label: {
        ekg::label_t &label {
          ekg::io::any_static_cast<ekg::label_t>(
            &descriptor
          )
        };

        ekg::ui::label *p_label {
          ekg::io::new_widget_instance<ekg::ui::label>()
        };

        p_label->descriptor = label;
        p_label->descriptor.theme = current_global_theme.label;

        properties.p_descriptor = &p_label->descriptor;
        properties.p_widget = p_label;
        properties.dock = label.dock;
        properties.is_docknizable = false;
        properties.is_parentable = true;

        p_created_widget = dynamic_cast<ekg::ui::abstract*>(p_label);
        p_created_widget->p_descriptor_rect = &p_label->descriptor.rect;

        break;
      }

      case ekg::type::checkbox: {
        ekg::checkbox_t &checkbox {
          ekg::io::any_static_cast<ekg::checkbox_t>(
            &descriptor
          )
        };

        ekg::ui::checkbox *p_checkbox {
          ekg::io::new_widget_instance<ekg::ui::checkbox>()
        };

        p_checkbox->descriptor = checkbox;
        p_checkbox->descriptor.theme = current_global_theme.checkbox;

        properties.p_descriptor = &p_checkbox->descriptor;
        properties.p_widget = p_checkbox;
        properties.dock = checkbox.dock;
        properties.is_docknizable = false;
        properties.is_parentable = true;

        p_created_widget = dynamic_cast<ekg::ui::abstract*>(p_checkbox);
        p_created_widget->p_descriptor_rect = &p_checkbox->descriptor.rect;

        break;
      }

    default:
      break;
    }

    p_created_widget->properties = properties;

    ekg::properties_t *p_current_parent_properties {
      ekg::p_core->get_current_parent_properties()
    };

    if (
      p_created_widget->properties.is_parentable
      &&
      p_current_parent_properties != nullptr
    ) {
      ekg::add_child_to_parent(
        p_current_parent_properties,
        &p_created_widget->properties
      );
    }

    ekg::p_core->dispatch_widget_op(
      p_created_widget,
      ekg::io::operation::reload
    );

    if (p_created_widget->properties.is_docknizable) {
      ekg::p_core->set_current_parent_properties(
        &p_created_widget->properties
      );

      ekg::p_core->dispatch_widget_op(
        p_created_widget,
        ekg::io::operation::layout_docknize
      );
    }

    p_created_widget->on_create();
    return p_created_widget->properties;
  }

  void pop(ekg::properties_t *p_properties = nullptr);
}

#endif
