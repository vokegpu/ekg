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
#include "ekg/ui/button/widget.hpp"
#include "ekg/layout/docknize.hpp"
#include "ekg/io/log.hpp"
#include "ekg/ui/abstract.hpp"
#include "ekg/draw/shape/shape.hpp"
#include "ekg/draw/typography/font.hpp"
#include "ekg/core/context.hpp"

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::button_t &button
) {
  ekg_assert_low_level(
    property == ekg::property_t::not_found,
    ekg::log() << "warn: invalid property on reload",
    return
  );

  ekg_assert_low_level(
    button == ekg::button_t::not_found,
    ekg::log() << "warn: invalid button on reload",
    return
  );

  ekg::ui::get_abs_rect(
    property,
    button.rect
  );

  ekg::axis pick_axis {
    ekg::axis::horizontal
  };

  if (button.checks.empty()) {
    button.checks.push_back(
      {}
    );
  }

  ekg::aligned_t aligned_dimension {};
  for (ekg::button_t::check_t &check : button.checks) {
    ekg::draw::font &draw_font {
      ekg::draw::get_font_renderer(check.font_size)
    };

    check.widget.rect_text.w = draw_font.get_text_width(check.text.get());
    check.widget.rect_text.h = draw_font.get_text_height();

    aligned_dimension = {};
    ekg::align_rect_dimension(
      pick_axis,
      check.widget.rect_text,
      ekg::dpi.min_sizes,
      aligned_dimension
    );
  }

  button.rect.h = aligned_dimension.h * button.rect.scaled_height;

  ekg::layout::mask mask {};
  mask.preset(
    {
      aligned_dimension.offset,
      aligned_dimension.offset,
      button.rect.h
    },
    pick_axis,
    button.rect.w
  );

  for (ekg::button_t::check_t &check : button.checks) {
    ekg::draw::font &draw_font {
      ekg::draw::get_font_renderer(check.font_size)
    };

    if (check.box != ekg::dock::none) {
      check.widget.rect_box.w = check.widget.rect_text.h;
      check.widget.rect_box.h = check.widget.rect_text.h;

      mask.insert(
        {
          .p_rect = &check.widget.rect_box,
          .dock = check.box
        }
      );
    }

    mask.insert(
      {
        .p_rect = &check.widget.rect_text,
        .dock = check.dock
      }
    );
  }

  mask.docknize();

  if (property.widget.should_refresh_size) {
    button.rect.w = ekg::max(ekg::dpi.min_sizes, mask.get_rect().w);
    property.widget.should_refresh_size = false;
  }
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::button_t &button,
  const ekg::io::stage &stage
) {
  switch (stage) {
    case ekg::io::stage::process: {
      ekg::rect_t<float> &abs {ekg::ui::get_abs_rect(property, button.rect)};
      ekg::input_info_t &input {ekg::p_core->handler_input.info};

      if (property.widget.is_hovering && input.has_motion) {
        ekg_action(
          button.actions,
          ekg::action::hover,
          ekg::timing_t::second > ekg::gui.ui.frequency
        );
      }

      if (!property.widget.is_active && property.widget.is_hovering && input.was_pressed) {
      }

      break;
    }
  case ekg::io::stage::pre:
    break;
  case ekg::io::stage::post:
    break;
  }
}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::button_t &button
) {

}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::button_t &button
) {
  
}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::button_t &button
) {
  
}

void ekg::ui::unmap(
  ekg::button_t &button
) {

}
