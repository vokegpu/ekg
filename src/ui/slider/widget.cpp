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
#include "ekg/ui/slider/widget.hpp"
#include "ekg/draw/typography/font.hpp"
#include "ekg/draw/allocator.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/io/utf.hpp"
#include "ekg/core/context.hpp"
#include "ekg/layout/scalenize.hpp"
#include "ekg/ui/abstract.hpp"
#include "ekg/core/pools.hpp"

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::slider_t &slider
) {
  constexpr ekg::pixel_t min_bar_width_factor {ekg::one_pixel * 4};
  ekg::pixel_t min_bar_width {slider.rect.w / min_bar_width};

  ekg::theme_t &global_theme {ekg::p_core->handler_theme.get_current_theme()};
  ekg::aligned_t aligned_dimension {};
  ekg::axis pick_axis {ekg::axis::horizontal};

  slider.rect.scaled_height = ekg::max(slider.rect.scaled_height, ekg::one_pixel);
  for (ekg::slider_t::range_t &range : slider.ranges) {
    ekg::draw::font &draw_font {ekg::draw::get_font_renderer(range.font_size)};

    ekg_ui_slider_range_task(
      range,
      {
        {
          ekg::utf8_number_precision(
            range.widget.text,
            min,
            range.precision
          );

          range.widget.rect_text.w = ekg::max(
            range.widget.rect_text.w,
            draw_font.get_text_width(range.widget.text)
          );
        }

        {
          ekg::utf8_number_precision(
            range.widget.text,
            max,
            range.precision
          );

          range.widget.rect_text.w = ekg::max(
            range.widget.rect_text.w,
            draw_font.get_text_width(range.widget.text)
          );
        }

        ekg::utf8_number_precision(
          range.widget.text,
          value,
          range.precision
        );

        range.widget.rect_text.h = draw_font.get_text_height();
      }
    );

    aligned_dimension = {};
    ekg::align_rect_dimension(
      pick_axis,
      range.widget.rect_text,
      ekg::dpi.min_sizes,
      aligned_dimension
    );

    aligned_dimension.h *= slider.rect.scaled_height;
    slider.rect.h = ekg::max(slider.rect.h, aligned_dimension.h);

    range.widget.rect_bar.h = (
      aligned_dimension.h * (static_cast<float>(slider.color_scheme.bar_thickness) / 100.0f)
    );

    range.widget.rect_target.h = (
      aligned_dimension.h * (static_cast<float>(slider.color_scheme.bar_target_thickness) / 100.0f)
    );

    range.widget.rect_target.w = range.widget.rect_text.h;
    range.widget.rect_bar.w = ekg::max(range.widget.rect_bar.w, min_bar_width);
  }

  ekg::layout::mask mask {};
  mask.preset(
    {
      aligned_dimension.offset,
      aligned_dimension.offset,
      slider.rect.h
    },
    pick_axis,
    slider.rect.w
  );

  for (ekg::slider_t::range_t &range : slider.ranges) {
    /**
     * @TODO: a specialized issue for mask-reordering may with a new dock value.
     * I will explicit set the order here, a temp solution while no dock-priority is done.
     **/
    if (ekg::has(range.dock_text, ekg::dock::left)) {
      mask.insert(
        {.p_rect = &range.widget.rect_text, .dock = range.dock_text}
      );

      mask.insert(
        {.p_rect = &range.widget.rect_bar, .dock = range.dock}
      );
    } else if (ekg::has(range.dock_text, ekg::dock::right)) {
      mask.insert(
        {.p_rect = &range.widget.rect_bar, .dock = range.dock}
      );

      mask.insert(
        {.p_rect = &range.widget.rect_text, .dock = range.dock_text}
      );
    } else {
      mask.insert(
        {.p_rect = &range.widget.rect_bar, .dock = range.dock}
      );
    }
  }

  mask.docknize();

  if (property.widget.should_refresh_size) {
    slider.rect.w = ekg::min(slider.rect.w, mask.get_rect().w);
    property.widget.should_refresh_size = false;
  }

  property.widget.should_buffering = true;
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::slider_t &slider,
  const ekg::io::stage &stage
) {

}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::slider_t &slider
) {

}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::slider_t &slider
) {
  ekg_draw_allocator_bind_local(
    &property.widget.geometry_buffer,
    &property.widget.gpu_data_buffer
  );

  if (property.widget.should_buffering) {
    return;
  }

  ekg_draw_allocator_pass();
}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::slider_t &slider
) {
  ekg::rect_t<float> &rect {
    ekg::ui::get_abs_rect(
      property, slider.rect
    )
  };

  ekg_draw_allocator_assert_scissor(
    property.widget.rect_scissor,
    rect,
    ekg::query<ekg::property_t>(property.parent_at).widget.rect_scissor,
    ekg::always_parented
  );

  ekg_draw_allocator_pass();
}

void ekg::ui::unmap(
  ekg::slider_t &slider
) {
  for (ekg::slider_t::range_t &range : slider.ranges) {
    range.value.ownership(nullptr);
    range.min.ownership(nullptr);
    range.max.ownership(nullptr);
  }
}
