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
#include "ekg/layout/scalenize.hpp"
#include "ekg/io/log.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/core/context.hpp"

void ekg::core::swap_collector(
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
    property.at == ekg::gui.bind.swap_at
  ) {
    was_found = true;
  }

  ekg::p_core->collector.push_back(property_at);

  if (property.is_childnizate && property.is_children_docknizable) {
    for (ekg::property_t &property : property.children) {
      if (property == ekg::property_t::not_found) {
        continue;
      }

      ekg::core::swap_collector(was_found, property);
    }
  }
}

void ekg::core::swap(ekg::info_t &info) {
  if (
    ekg::p_core == nullptr
    ||
    ekg::gui.bind.swap_at == ekg::at_t::not_found
  ) {
    return;
  }

  bool was_found {};

  ekg::p_core->top_level_stack.clear();
  for (ekg::at_t &at : ekg::p_core->registry) {
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
  ekg::gui.bind.swap_at = ekg::at_t::not_found;
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
      property.descriptor_at.flags,
      property.descriptor_at,
      ekg::ui::reload(property, descriptor);
    );

    property.operation.should_reload = false;
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

    ekg::layout::docknize(
      property
    );

    property.operation.should_docknize = false;
  }

  ekg::p_core->reload.clear();
}

void ekg::core::scalenize(ekg::info_t &info) {
  if (ekg::p_core == nullptr) {
    return;
  }

  ekg::layout::scale_calculate();

  ekg::dpi.font_scale = ekg::clamp<float>(
    ekg::dpi.font_scale,
    static_cast<float>(4),
    static_cast<float>(UINT8_MAX)
  );

  uint32_t font_size {
    ekg::clamp<uint32_t>(
      static_cast<uint32_t>(
        ekg::dpi.font_scale
        *
        ekg::dpi.factor_scale
      ),
      0,
      UINT8_MAX
    )
  };

  if (this->draw_font_medium.font_size != font_size) {
    this->draw_font_small.set_size(
      ekg::clamp_min(
        font_size - ekg::dpi.font_offset.x,
        ekg::minimum_small_font_height
      )
    );

    this->draw_font_medium.set_size(
      ekg::clamp_min(
        font_size,
        ekg::minimum_font_height
      )
    );

    this->draw_font_big.set_size(
      ekg::clamp_min(
        font_size + ekg::dpi.font_offset.y,
        ekg::minimum_big_font_height
      )
    );
  }
}

void ekg::core::poll_events() {
  if (ekg::p_core == nullptr) {
    return;
  }

  ekg::input_info_t &input {
    ekg::p_core->handler_input.input
  };

  bool is_on_scrolling_timeout {!ekg::reach(input.ui_scrolling_timing, 100)};
  ekg::gui.ui.hovered_at *= !(input.was_pressed || input.was_released || input.has_motion);

  ekg::property_t &abs_widget {ekg::query<ekg::property_t>(ekg::gui.ui.abs_widget_at)};
  if (
      abs_widget != ekg::property_t::not_found
      &&
      (abs_widget.is_absolute || is_on_scrolling_timeout)
    ) {

    ekg_abstract_todo(
      abs_widget.descriptor_at.flags,
      abs_widget.descriptor_at,

      ekg::ui::event(property, descriptor, ekg::io::stage::pre);
      ekg::ui::event(property, descriptor, ekg::io::stage::process);
    
      if (
        property.scroll.is_scrolling.x
        ||
        property.scroll.is_scrolling.y
      ) {
        ekg::reset(input.ui_scrolling_timing);
      }

      ekg::ui::event(property, descriptor, ekg::io::stage::post);
    );

    return;
  }

  if (is_on_scrolling_timeout) {
    return;
  }

  ekg::gui.ui.abs_widget_at = ekg::at_t::not_found;

  bool hovered {};
  bool first_absolute {};

  ekg::at_t focused_at {ekg::at_t::not_found};
  for (ekg::at_t &at : ekg::p_core->stack) {
    ekg::property_t &property {
      ekg::query<ekg::property_t>(at)
    };

    if (ekg::property_t::not_found || property.is_dead) {
      continue;
    }

    ekg_abstract_todo(
      property.descriptor_at.flags,
      property.descriptor_at,

      ekg::ui::event(property, descriptor, ekg::io::stage::pre);

      hovered = (
        !(
          ekg::p_core->p_platform_base->event.flags == ekg::io::event_type::key_down
          ||
          ekg::p_core->p_platform_base->event.flags == ekg::io::event_type::key_up
          ||
          ekg::p_core->p_platform_base->event.flags == ekg::io::event_type::text_input
        )
        &&
        property.is_hovering
        &&
        property.is_visible
        &&
        property.is_enabled
      );

      if (hovered) {
        ekg::property_t &focused_property {
          ekg::query<ekg::property_t>(focused_at)
        };

        focused_property != ekg::property_t::not_found
          && (focused_property.is_hovering = false);

        focused_at = at;
        first_absolute = false;
      }

      if (property.is_absolute && !first_absolute) {
        focused_at = at;
        first_absolute = true;
      }

      ekg::ui::event(property, descriptor, ekg::io::stage::post);
      if (!hovered && !property.is_absolute) {
        ekg::ui::event(property, descriptor, ekg::io::stage::process);
      }
    );
  }

  ekg::gui.ui.hovered_type = ekg::type::unknown;

  ekg::property_t &focused_property {
    ekg::query<ekg::property_t>(focused_at)
  };

  if (focused_property != ekg::property_t::not_found) {
    ekg_abstract_todo(
      focused_at.at,
      focused_at,

      ekg::gui.ui.hovered_type = focused_at.flags;
      ekg::gui.ui.hovered_at = focused_at;

      if (property.is_absolute) {
        ekg::gui::ui.abs_widget_at = focused_at;
      }

      ekg::ui::event(property, descriptor, ekg::io::stage::pre);
      ekg::ui::event(property, descriptor, ekg::io::stage::process);
      ekg::ui::event(property, descriptor, ekg::io::stage::post);
    );
  }

  // @TODO: ref this later

  if (input.was_pressed) {
    ekg::gui.ui.pressed_at = ekg::gui.ui.hovered_at;
    (
      // if p_widget_focused != nullptr ? focused_at.flags : ekg::type::unknown
      (focused_property != ekg::property_t::not_found && (ekg::gui.ui.pressed_type = focused_at.flags))
      ||
      (ekg::gui.ui.pressed_type = ekg::type::unknown)
    );
  } else if (input.was_released) {
    ekg::gui.ui.released_at = ekg::gui.ui.hovered_at;
    (
      (focused_property != ekg::property_t::not_found && (ekg::gui.ui.released_type = focused_at.flags))
      ||
      (ekg::gui.ui.released_type = ekg::type::unknown)
    );
  }

  if (
      ekg::gui.ui.last_hovered_at != ekg::gui.ui.hovered_at
      &&
      ekg::gui.ui.hovered_at != ekg::at_t::not_found
      &&
      (
        input.was_pressed || input.was_released
      )
  ) {
    ekg::gui.bind.swap_at = ekg::gui.ui.hovered_at;
    ekg::gui.ui.last_hovered_at = ekg::gui.ui.hovered_at;
    ekg::gui.ui.redraw = true;
    ekg::io::dispatch
  }
}
