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

void ekg::ui::recursive_self_destroy_abs_popup(
  ekg::popup_t &popup
) {
  if (popup == ekg::popup_t::not_found) {
    return;
  }

  ekg::property_t &property {ekg::query<ekg::property_t>(popup.property_at)};
  ekg::ui::set_visible(
    property,
    false
  );

  property.widget.should_buffering = false;

  ekg::ui::recursive_self_destroy_abs_popup(
    ekg::query<ekg::popup_t>(popup.parent_popup_at)
  );
}

void ekg::ui::recursive_assert_set_current_open(
  ekg::property_t &property,
  ekg::at_t &opened,
  ekg::at_t &to_open
) {
  if (
    opened != ekg::at_t::not_found
    &&
    opened != to_open
  ) {
    ekg::ui::recursive_children_set_visible(
      ekg::query<ekg::popup_t>(opened),
      false
    );
  }

  opened = to_open;
  ekg::ui::set_visible(
    property,
    true
  );
}

void ekg::ui::set_visible(
  ekg::property_t &property,
  bool visible
) {
  property.states.is_visible = visible;
  property.widget.should_buffering = true;
  ekg::gui.ui.redraw = true;
}

void ekg::ui::recursive_children_set_visible(
  ekg::popup_t &popup,
  bool visible
) {
  if (popup == ekg::popup_t::not_found) {
    return;
  }

  ekg::property_t &property {ekg::query<ekg::property_t>(popup.property_at)};
  ekg::ui::set_visible(property, visible);

  for (ekg::popup_t::link_t &link : popup.links) {
    ekg::ui::recursive_children_set_visible(
      ekg::query<ekg::popup_t>(link.popup_at),
      visible
    );
  }
}

void ekg::ui::splash_popup_just_opened(
  ekg::popup_t &popup,
  const ekg::vec2_t<float> &pos
) {
  ekg::property_t &property {ekg::query<ekg::property_t>(popup.property_at)};
  if (
    popup == ekg::popup_t::not_found
    ||
    property == ekg::property_t::not_found
  ) {
    return;
  }

  ekg::ui::recursive_children_set_visible(
    popup,
    false
  );

  property.states.is_visible = true;
  popup.widget.was_visible = true;
  ekg::gui.ui.redraw = true;
  property.widget.should_buffering = true;
  popup.widget.just_opened = true;

  ekg::vec2_t<float> preview {};
  ekg::rect_t<float> &rect {popup.widget.frame.rect};

  preview.x = pos.x + rect.w;
  rect.x = pos.x;

  if (preview.x > ekg::dpi.viewport.w) {
    rect.x = pos.x - rect.w;
  }

  preview.y = pos.y + rect.h;
  rect.y = pos.y;

  if (preview.y > ekg::dpi.viewport.h) {
    rect.y = ekg::dpi.viewport.h - rect.h;
  }
}

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
  popup.widget.frame.color_scheme = popup.color_scheme;
  ekg::ui::reload(property, popup.widget.frame);
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::popup_t &popup,
  const ekg::io::stage &stage
) {
  ekg::ui::event(property, popup.widget.frame, stage);
  popup.rect = popup.widget.frame.rect;
  popup.widget.frame.drag = ekg::dock::none;
  popup.widget.frame.resize = ekg::dock::none;

  switch (stage) {
    case ekg::io::stage::pre: {
      ekg::input_info_t &input {ekg::p_core->handler_input.input};
      ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};

      bool skip_this_tick_self_destruction {};
      if (
        popup.widget.just_opened
        &&
        input.was_released
      ) {
        skip_this_tick_self_destruction = true;
        popup.widget.just_opened = false;
      }

      if (
        (
          !input.has_motion
          &&
          !input.was_pressed
          &&
          !input.was_released
        )
        ||
        !property.states.is_visible
        ||
        popup.widget.just_opened
      ) {
        break;
      }

      bool exists {};
      bool is_hovering_a_popup {};
      bool should_set_visibility {};
      bool should_unset_visibility {};
      bool is_linked_hovering {};
      bool is_this_popup_being_hovered {};
      bool is_hovering_any_linked_widget {};

      ekg::rect_t<float> rect_position {};
      ekg::at_t hovering_popup {};

      ekg_core_widget_call(
        ekg::gui.ui.hovered_type,
        ekg::gui.ui.hovered_at,
        {
          is_hovering_a_popup = false;
          hovering_popup = ekg::at_t::not_found;

          /**
           * Check if the hovered property is a popup.
           **/
          ekg::property_t &wproperty {ekg::query<ekg::property_t>(descriptor.property_at)};
          if (
            wproperty != ekg::property_t::not_found
            &&
            wproperty.parent_at == ekg::at_t::not_found
            &&
            wproperty.at.flags == ekg::type::popup
          ) {
            is_hovering_a_popup = true;
            hovering_popup = wproperty.descriptor_at;
          }

          /**
           * If not, then, check if the abs parent from hovered is a popup.
           **/
          ekg::property_t &wproperty_abs {ekg::query<ekg::property_t>(wproperty.abs_parent_at)};
          if (
            !is_hovering_a_popup
            &&
            wproperty_abs != ekg::property_t::not_found
            &&
            wproperty_abs.at.flags == ekg::type::popup
          ) {
            is_hovering_a_popup = true;
            hovering_popup = wproperty_abs.descriptor_at;
          }
        }
      );

      is_this_popup_being_hovered = hovering_popup == popup.at;

      for (ekg::popup_t::link_t &link : popup.links) {
        exists = false;
        is_linked_hovering = false;

        ekg_core_widget_call(
          link.widget_at.flags,
          link.widget_at,
          {
            ekg::property_t &wproperty {ekg::query<ekg::property_t>(descriptor.property_at)};
            if (wproperty.states.is_highlight) {
              should_set_visibility = true;
              is_linked_hovering = true;
              is_hovering_any_linked_widget = true;
            }

            rect_position = wproperty.widget.rect;
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

        if (
          /**
           * If the actual popup is being hovered
           * and linked widget is not being hovered
           * visibility can be unset!
           **/
          (
            is_this_popup_being_hovered
            &&
            !is_linked_hovering
            &&
            ekg::gui.ui.hovered_type != ekg::type::popup
          )
        ){
          should_unset_visibility = true;
        }

        ekg_core_unique_widget_call(
          ekg::popup_t,
          link.popup_at.flags,
          link.popup_at,
          {
            ekg::property_t &wproperty {ekg::query<ekg::property_t>(descriptor.property_at)};
            descriptor.parent_popup_at = popup.at;

            if (should_set_visibility) {
              ekg::ui::recursive_assert_set_current_open(
                property,
                popup.widget.popup_opened_at,
                descriptor.at
              );

              wproperty.states.is_visible = true;
              should_set_visibility = false;
            }

            if (should_unset_visibility) {
              ekg::ui::recursive_children_set_visible(
                descriptor,
                false
              );

              popup.widget.popup_opened_at = ekg::at_t::not_found;
              should_unset_visibility = false;
            }

            if (descriptor.widget.was_visible != wproperty.states.is_visible) {
              ekg::ui::splash_popup_but_bounding(
                popup.color_scheme.popup_offset,
                rect_position,
                popup.rect,
                descriptor.widget.frame.rect
              );

              descriptor.widget.was_visible = wproperty.states.is_visible;
              ekg::io::dispatch(ekg::io::operation::swap, wproperty.at);
            }
          }
        );
      }

      if (
        (
          (
            input.was_released
            &&
            is_this_popup_being_hovered
            &&
            !is_hovering_any_linked_widget
          )
          ||
          (
            input.was_released
            &&
            !is_hovering_a_popup
          )
        )
        &&
        !skip_this_tick_self_destruction
      ) {
        popup.widget.should_self_recursive_destroy = true;
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
)  {
  if (popup.widget.should_self_recursive_destroy) {
    ekg::ui::recursive_self_destroy_abs_popup(
      popup
    );

    popup.widget.should_self_recursive_destroy = false;
  }

  ekg::ui::pass(property, popup.widget.frame);
}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::popup_t &popup
) {
  // ekg::draw::allocator::is_scissor_sync_allowed = false;
  popup.widget.frame.color_scheme = popup.color_scheme;
  ekg::ui::buffering(property, popup.widget.frame);
  // ekg::draw::allocator::is_scissor_sync_allowed = true;
}

void ekg::ui::unmap(
  ekg::popup_t &popup
) {

}
