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
#include "ekg/ui/label/widget.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/draw/shape/shape.hpp"
#include "ekg/draw/typography/font.hpp"
#include "ekg/layout/docknize.hpp"
#include "ekg/ui/abstract.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/draw/allocator.hpp"

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::label_t &label
) {
  ekg::ui::get_abs_rect(
    property,
    label.rect
  );

  ekg::axis pick_axis {
    ekg::axis::horizontal
  };

  ekg::draw::font &draw_font {
    ekg::draw::get_font_renderer(label.font_size)
  };

  label.widget.rect_text.w = draw_font.get_text_width(label.text.get());
  label.widget.rect_text.h = draw_font.get_text_height();

  ekg::aligned_t aligned_dimension {};
  ekg::align_rect_dimension(
    pick_axis,
    label.widget.rect_text,
    ekg::dpi.min_sizes,
    aligned_dimension
  );

  label.rect.scaled_height = ekg::max<ekg::pixel_thickness_t>(1, label.rect.scaled_height);
  label.rect.h = aligned_dimension.h * label.rect.scaled_height;

  ekg::layout::mask mask {};
  mask.preset(
    {
      aligned_dimension.offset,
      aligned_dimension.offset,
      label.rect.h
    },
    pick_axis,
    label.rect.w
  );

  mask.insert(
    {
      .p_rect = &label.widget.rect_text,
      .dock = label.dock_text
    }
  );

  mask.docknize();

  if (property.widget.should_refresh_size) {
    label.rect.w = ekg::max(ekg::dpi.min_sizes, mask.get_rect().w);
    property.widget.should_refresh_size = false;
  }
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::label_t &label,
  const ekg::io::stage &stage
) {
  switch (stage) {
  case ekg::io::stage::pre:
    ekg::ui::pre_event(property, label.rect, false);
    break;
  case ekg::io::stage::post:
    ekg::ui::post_event(property);
    break;
  default:
    break;
  }
}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::label_t &label
) {

}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::label_t &label
) {
  ekg_draw_allocator_bind_local(
    &property.widget.geometry_buffer,
    &property.widget.gpu_data_buffer
  );

  if (property.widget.should_buffering) {
    return;
  }

  if (label.text.was_changed()) {
    property.widget.should_buffering = true;
    return;
  }

  ekg_draw_allocator_pass();
}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::label_t &label
) {
  ekg::rect_t<float> &rect_abs {ekg::ui::get_abs_rect(property, label.rect)};

  ekg_draw_allocator_assert_scissor(
    property.widget.rect_scissor,
    rect_abs,
    ekg::query<ekg::property_t>(property.parent_at).widget.rect,
    true
  );

  ekg::draw::rect(
    rect_abs,
    label.color_scheme.background,
    ekg::draw::mode::fill,
    label.layers[ekg::layer::bg]
  );

  ekg::draw::get_font_renderer(label.font_size)
    .blit(
      label.text.get(),
      rect_abs.x + label.widget.rect_text.x, rect_abs.y + label.widget.rect_text.y,
      label.color_scheme.text_foreground
    );

  ekg::draw::rect(
    rect_abs,
    label.color_scheme.outline,
    ekg::draw::mode::outline,
    label.layers[ekg::layer::outline]
  );

  ekg_draw_allocator_pass();
}

void ekg::ui::unmap(
  ekg::label_t &label
) {
  label.text.ownership(nullptr);
}
