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
#ifndef EKG_UI_SCROLLBAR_WIDGET_HPP
#define EKG_UI_SCROLLBAR_WIDGET_HPP

#include "scrollbar.hpp"
#include "ekg/ui/property.hpp"

namespace ekg::ui {
  float get_horizontal_scrollbar_normalized(
    ekg::property_t &property,
    ekg::rect_t<float> &rect_parent
  );

  float get_vertical_scrollbar_normalized(
    ekg::property_t &property,
    ekg::rect_t<float> &rect_parent
  );

  void check_scrollbar(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar,
    ekg::rect_t<float> &rect_parent
  );

  void reset_scrollbar(
    ekg::property_t &property
  );

  void clamp_scrollbar(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar,
    ekg::rect_t<float> &rect_parent
  );

  bool is_scrollbar_scrolling(
    ekg::property_t &property,
    bool state
  );

  void reload(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar,
    ekg::rect_t<float> &rect_parent,
    std::vector<ekg::at_t> &children,
    bool should_childnizate_metrics
  );

  void reload(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar
  );

  void process_event(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar,
    ekg::rect_t<float> &rect_parent
  );

  void event(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar,
    const ekg::io::stage &stage
  );

  void high_frequency(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar,
    ekg::rect_t<float> &rect_parent
  );

  void high_frequency(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar
  );

  void pass(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar
  );

  void buffering(
    ekg::property_t &property,
    ekg::scrollbar_t &scrollbar
  );

  void unmap(
    ekg::scrollbar_t &scrollbar
  );
}

#endif
