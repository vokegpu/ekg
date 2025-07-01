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
#include "ekg/ui/popup/widget.hpp"
#include "ekg/ui/frame/widget.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/core/context.hpp"

void ekg::ui::splash_popup_but_bounding(
  float &popup_offset,
  ekg::rect_t<float> &rect_widget,
  ekg::rect_t<float> &rect_parent,
  ekg::rect_t<float> &rect_child
) {
  ekg::rect_t<float> preview {};

  preview.w = rect_parent.x + rect_parent.w + popup_offset;
  rect_child.x = preview.w;

  if (preview.w + rect_child.w > ekg::dpi.viewport.w) {
    rect_child.x = rect_parent.x - rect_parent.w - popup_offset;
  }

  preview.h = rect_widget.y;
  rect_child.y = preview.h;

  if (preview.h + rect_child.h > ekg::dpi.viewport.h) {
    preview.h += rect_child.h;
    rect_child.y = rect_widget.y - (preview.h - ekg::dpi.viewport.h);
  }
}

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::popup_t &popup
) {
  ekg::ui::reload(property, popup.widget.frame);
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::popup_t &popup,
  const ekg::io::stage &stage
) {
  ekg::ui::event(property, popup.widget.frame, stage);
  popup.rect = popup.widget.frame.rect;

  switch (stage) {
    case ekg::io::stage::post: {
      ekg::input_info_t &input {ekg::p_core->handler_input.input};
      ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};

      if (
        !input.has_motion
        &&
        !input.was_pressed
        &&
        !input.was_released
      ) {
        break;
      }

      bool exists {};
      bool preview_popup {};
      ekg::rect_t<float> rect_position {};

      for (ekg::popup_t::link_t &link : popup.links) {
        exists = false;

        ekg_core_widget_call(
          link.widget_at.flags,
          link.widget_at,
          {
            ekg::property_t &wproperty {ekg::query<ekg::property_t>(descriptor.property_at)};
            if (wproperty.states.is_highlight) {
              preview_popup = true;
              rect_position = wproperty.widget.rect;
            }

            exists = true;
          }
        );

        // @TODO: ref this thanks
        if (!exists) {
          for (ekg::at_t &at : property.children) {
            ekg_core_widget_call(
              at.flags,
              at,
              {
                if (descriptor.tag == link.tag) {
                  exists = true;
                  link.widget_at = at;
                }
              }
            );

            if (exists) {
              break;
            }
          }

          continue;
        }

        ekg_core_unique_widget_call(
          ekg::popup_t,
          link.popup_at.flags,
          link.popup_at,
          {
            ekg::property_t &wproperty {ekg::query<ekg::property_t>(descriptor.property_at)};

            descriptor.parent_popup_at = popup.at;
            wproperty.states.is_visible = false;

            if (preview_popup) {
              wproperty.states.is_visible = true;

              splash_popup_but_bounding(
                popup.color_scheme.popup_offset,
                rect_position,
                popup.rect,
                descriptor.widget.frame.rect
              );
            }
          }
        );
      }

      break;
    }

  default:
    break;
  }
}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::popup_t &popup
) {
}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::popup_t &popup
) {
  ekg::ui::pass(property, popup.widget.frame);
}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::popup_t &popup
) {
  popup.widget.frame.color_scheme = popup.color_scheme;
  ekg::ui::buffering(property, popup.widget.frame);
}

void ekg::ui::unmap(
  ekg::popup_t &popup
) {

}
