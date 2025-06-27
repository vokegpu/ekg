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
#ifndef EKG_UI_SLIDER_WIDGET_HPP
#define EKG_UI_SLIDER_WIDGET_HPP

#include "slider.hpp"
#include "ekg/ui/property.hpp"
#include "ekg/io/event.hpp"

namespace ekg::ui {
  static const size_t f64 {typeid(double).hash_code()};
  static const size_t f32 {typeid(float).hash_code()};
  static const size_t i64 {typeid(int64_t).hash_code()};
  static const size_t u64 {typeid(uint64_t).hash_code()};
  static const size_t i32 {typeid(int32_t).hash_code()};
  static const size_t u32 {typeid(uint32_t).hash_code()};
  static const size_t i16 {typeid(int16_t).hash_code()};
  static const size_t u16 {typeid(uint16_t).hash_code()};
  static const size_t i8 {typeid(int8_t).hash_code()};
  static const size_t u8 {typeid(uint8_t).hash_code()};
}

/**
 * This make easily the part of adding tasks like: drag, draw, etc.
 **/
#define ekg_ui_slider_range_task_impl(ekg_ui_slider_range, number_t_hash, number_t, todo) \
  if (hash == number_t_hash) { \
    number_t &value {ekg_ui_slider_range.value.as<number_t>()}; \
    number_t &min {ekg_ui_slider_range.min.as<number_t>()}; \
    number_t &max {ekg_ui_slider_range.max.as<number_t>()}; \
    todo; \
  }

/**
 * This provides a map for most all number-types.
 **/
#define ekg_ui_slider_range_task(ekg_ui_slider_range, todo) \
  const size_t hash {ekg_ui_slider_range.value.get_type_info_hash()}; \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::f64, double, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::f32, float, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::i64, int64_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::u64, uint64_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::i32, int32_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::u32, uint32_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::i16, int16_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::u16, uint16_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::i8, int8_t, todo); \
  ekg_ui_slider_range_task_impl(ekg_ui_slider_range, ekg::ui::u8, uint8_t, todo);

namespace ekg::ui {
  template<typename t>
  void calculate_value_by_factor(
    ekg::slider_t::range_t &range,
    ekg::vec2_t<float> &factor,
    const ekg::axis &axis,
    t &min, t &max, t &value
  ) {
    value = ekg::clamp<t>(value, min, max);
    switch (axis) {
    case ekg::axis::horizontal:
      if (static_cast<ekg::pixel_t>(factor.x) == 0) {
        value = min;
        break;
      }

      value = (factor.x / range.widget.rect_bar.w) * (max - min) + min;
      break;
    case ekg::axis::vertical:
      if (static_cast<ekg::pixel_t>(factor.y) == 0) {
        value = min;
        break;
      }

      value = (factor.y / range.widget.rect_bar.h) * (max - min) + min;
      break;
    }
  }

  template<typename t>
  void calculate_bar_progress_by_value(
    ekg::slider_t::range_t &range,
    const ekg::axis &axis,
    t &min, t &max, t &value
  ) {
    value = ekg::clamp<t>(value, min, max);
    switch (axis) {
    case ekg::axis::horizontal:
      range.widget.rect_bar_progress.w = range.widget.rect_bar.w * (value - min) / (max - min);
      range.widget.rect_target.x = range.widget.rect_bar_progress.w - (range.widget.rect_target.w * 0.5f);
      break;
    case ekg::axis::vertical:
      range.widget.rect_bar_progress.h = range.widget.rect_bar.h * (value - min) / (max - min);
      range.widget.rect_target.y = range.widget.rect_bar_progress.h - (range.widget.rect_target.h * 0.5f);
      break;
    }
  }

  void reload(
    ekg::property_t &property,
    ekg::slider_t &slider
  );

  void event(
    ekg::property_t &property,
    ekg::slider_t &slider,
    const ekg::io::stage &stage
  );

  void high_frequency(
    ekg::property_t &property,
    ekg::slider_t &slider
  );

  void pass(
    ekg::property_t &property,
    ekg::slider_t &slider
  );

  void buffering(
    ekg::property_t &property,
    ekg::slider_t &slider
  );

  void unmap(
    ekg::slider_t &slider
  );
}

#endif
