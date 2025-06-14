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
#include "ekg/ui/scrollbar/widget.hpp"
#include "ekg/draw/allocator.hpp"
#include "ekg/draw/typography/font.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/math/floating_point.hpp"

float ekg::ui::get_horizontal_scrollbar_normalized(
  ekg::property_t &property,
  ekg::rect_t<float> &rect_parent
) {
  return (property.scroll.position.x / rect_parent.w);
}

float ekg::ui::get_vertical_scrollbar_normalized(
  ekg::property_t &property,
  ekg::rect_t<float> &rect_parent
) {
  return (property.scroll.position.y / rect_parent.h);
}

void ekg::ui::check_scrollbar(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar,
  ekg::rect_t<float> &rect_parent
) {
  property.scroll.is_enabled.x = scrollbar.widget.rect_scrollable_area.w > rect_parent.w;
  property.scroll.is_enabled.y = scrollbar.widget.rect_scrollable_area.h > rect_parent.h;
}

void ekg::ui::reset_scrollbar(
  ekg::property_t &property
) {
  property.scroll.position.x = property.scroll.position.z;
  property.scroll.position.y = property.scroll.position.w;
}

void ekg::ui::clamp_scrollbar(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar,
  ekg::rect_t<float> &rect_parent
) {
  ekg::vec2_t<float> h_limit {0.0f, rect_parent.w - scrollbar.widget.rect_scrollable_area.w};
  if (scrollbar.widget.rect_scrollable_area.w < rect_parent.w) {
    property.scroll.position.x = 0.0f;
    property.scroll.position.z = 0.0f;
  } else if (property.scroll.position.x < h_limit.y) {
    property.scroll.position.x = h_limit.y;
    property.scroll.position.z = h_limit.y;
  } else if (property.scroll.position.x > h_limit.x) {
    property.scroll.position.x = h_limit.x;
    property.scroll.position.z = h_limit.x;
  }

  ekg::vec2_t<float> v_limit {0.0f, rect_parent.h - scrollbar.widget.rect_scrollable_area.h};
  if (scrollbar.widget.rect_scrollable_area.h < rect_parent.h) {
    property.scroll.position.y = 0.0f;
    property.scroll.position.w = 0.0f;
  } else if (property.scroll.position.y < v_limit.y) {
    property.scroll.position.y = h_limit.y;
    property.scroll.position.w = h_limit.y;
  } else if (property.scroll.position.y > v_limit.x) {
    property.scroll.position.y = h_limit.x;
    property.scroll.position.w = h_limit.x;
  }
}

bool ekg::ui::is_scrollbar_scrolling(
  ekg::property_t &property,
  bool state
) {
  if (
    !(
      state = (
        state
        ||
        (
          (
            static_cast<int32_t>(roundf(property.scroll.position.x))
            !=
            static_cast<int32_t>(roundf(property.scroll.position.z))
          )
          ||
          (
            static_cast<int32_t>(roundf(property.scroll.position.y))
            !=
            static_cast<int32_t>(roundf(property.scroll.position.w))
          )
        )
      )
    )
  ) {
    property.scroll.position.x = property.scroll.position.z;
    property.scroll.position.y = property.scroll.position.w;
  }

  return state;
}

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar,
  ekg::rect_t<float> &rect_parent,
  std::vector<ekg::at_t> &children,
  bool should_childnizate_metrics
) {
  ekg::ui::check_scrollbar(property, scrollbar, rect_parent);

  if (!should_childnizate_metrics) {
    return;
  }

  scrollbar.widget.rect_scrollable_area = {};
  scrollbar.acceleration.x = ekg::draw::get_font_renderer(ekg::font::medium).get_text_height();

  float place {};
  for (ekg::at_t &at : children) {
    ekg_core_abstract_todo(
      at.flags,
      at,

      scrollbar.widget.rect_scrollable_area.w = (
        ekg::max(scrollbar.widget.rect_scrollable_area.w, descriptor.rect.x + descriptor.rect.w)
      );

      scrollbar.widget.rect_scrollable_area.h = (
        ekg::max(scrollbar.widget.rect_scrollable_area.h, descriptor.rect.y + descriptor.rect.h)
      );


      scrollbar.acceleration.y = ekg::min(scrollbar.acceleration.y, descriptor.rect.h);
    );
  }

  scrollbar.acceleration.y += scrollbar.acceleration.x + (scrollbar.acceleration.x * 0.5f);
  ekg::ui::check_scrollbar(property, scrollbar, rect_parent);
  ekg::ui::clamp_scrollbar(property, scrollbar, rect_parent);
}

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar
) {
  ekg::property_t &parent {ekg::query<ekg::property_t>(property.parent_at)};
  if (parent == ekg::property_t::not_found) {
    return;
  }

  bool should_childnizate_metrics {true};
  ekg::ui::reload(
    property,
    scrollbar,
    parent.widget.rect,
    parent.children,
    should_childnizate_metrics
  );
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar,
  const ekg::io::stage &stage
) {
  switch (stage) {
    case ekg::io::stage::pre: {
      ekg::input_info_t &input {ekg::p_core->handler_input.input};
      ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};

      if (
        input.was_pressed
        ||
        input.was_released
        ||
        input.has_motion
        ||
        input.was_wheel
      ) {
        ekg::rect_t<float> h_bar {scrollbar.widget.rect_horizontal};
        h_bar.x += property.widget.rect.x;

        ekg::rect_t<float> v_bar {scrollbar.widget.rect_vertical};
        v_bar.y += property.widget.rect.y;
        
        bool is_visible {
          ekg::rect_collide_vec2<float>(property.widget.rect_scissor, interact)
        };

        property.states.is_active = (
          is_visible
          &&
          (property.scroll.is_scrolling.x || property.scroll.is_scrolling.y)
          &&
          ekg::fire("scrollbar-scroll")
        );         
      }

      break;
    }

    case ekg::io::stage::post: {
      break;
    }

    case ekg::io::stage::process: {
      break;
    }
  }
}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar
) {

}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar
) {

}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::scrollbar_t &scrollbar
) {

}

void unmap(
  ekg::scrollbar_t &scrollbar
) {

}
