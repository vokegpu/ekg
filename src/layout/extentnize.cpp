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
#include "ekg/layout/extentnize.hpp"
#include "ekg/core/runtime.hpp"

ekg::layout::extent_t ekg::layout::extent_t::v_widget {};
ekg::layout::extent_t ekg::layout::extent_t::h_widget {};
ekg::layout::extent_t ekg::layout::extent_t::v_mask {};
ekg::layout::extent_t ekg::layout::extent_t::h_mask {};

void ekg::layout::extentnize_mask(
  std::vector<ekg::layout::mask::component_t> &components,
  ekg::vec3_t<float> offset,
  ekg::flags_t flag_ok,
  ekg::flags_t flag_stop,
  ekg::flags_t flag_axis,
  float &extent,
  int32_t &in_out_count
) {
  extent = 0.0f;
  switch (flag_axis & ekg::axis::horizontal) {
    case ekg::axis::horizontal: {
      int32_t it {in_out_count};

      if (
          it > ekg::layout::extent_t::h_mask.begin_index
          &&
          it < ekg::layout::extent_t::h_mask.end_index
        ) {
        in_out_count = ekg::layout::extent_t::h_mask.count;
        extent = ekg::layout::extent_t::h_mask.extent;
        return;
      }

      ekg::layout::extent_t::h_mask.begin_index = static_cast<float>(it);

      int32_t size {static_cast<int32_t>(components.size())};
      int32_t latest_index {static_cast<int32_t>(size - (!components.empty()))};
      int32_t should_skip_next {};
      int32_t flag_ok_count {};

      bool is_last_index {};
      bool is_ok_flag {};
 
      extent += offset.x;

      for (it = it; it < size; it++) {
        ekg::layout::mask::component_t &component {components.at(it)};
        if (component.p_rect == nullptr) {
          continue;
        }

        is_last_index = it == latest_index;

        if (
            (ekg::has(component.flags, flag_stop) && it != in_out_count)
            ||
            is_last_index
          ) {
          extent -= offset.x;
          flag_ok_count += (
            (is_ok_flag = (!ekg::has(component.flags, flag_stop) && (ekg::has(component.flags, flag_ok)) && is_last_index))
          );

          /**
           * Basically if the container/frame mother ends with any non flag ok (ekg::dock::fill)
           * it MUST add the width size to extend.
           *
           * :blush:
           **/
          extent += ( 
            (component.p_rect->w + offset.x)
            *
            (is_last_index && (!ekg::has(component.flags, flag_ok) && should_skip_next == 0))
          );

          ekg::layout::extent_t::h_mask.end_index = it + is_last_index;
          ekg::layout::extent_t::h_mask.extent = extent;
          ekg::layout::extent_t::h_mask.count = flag_ok_count + (flag_ok_count == 0);
          break;
        }

        should_skip_next += ekg::has(component.flags, ekg::dock::concat);

        if (should_skip_next > 0) {
          should_skip_next = (should_skip_next + 1) * (should_skip_next < 2);
          flag_ok_count += ekg::has(component.flags, flag_ok);
          continue;
        }

        if (ekg::has(component.flags, flag_ok)) {
          flag_ok_count++;
          continue;
        }

        extent += component.p_rect->w + offset.x;
      }

      in_out_count = flag_ok_count + (flag_ok_count == 0);
      break;
    }

    case ekg::axis::vertical: {
      break;
    }
  }
}

void ekg::layout::extentnize_widget(
  ekg::property_t &parent_property,
  ekg::flags_t flag_ok,
  ekg::flags_t flag_stop,
  ekg::flags_t flag_axis,
  float &extent,
  int32_t &in_out_count
) {
  extent = 0.0f;
  if (p_widget == nullptr) {
    return;
  }

  int32_t begin_index {in_out_count};
  switch (flag_axis & ekg::axis::horizontal) {
    case ekg::axis::horizontal: {
      int32_t it {begin_index};

      if (
          it > ekg::layout::extent_t::h_widget.begin_index
          &&
          it < ekg::layout::extent_t::h_widget.end_index
        ) {

        in_out_count = static_cast<int32_t>(ekg::layout::extent_t::h_widget.count);
        extent = ekg::layout::extent_t::h_widget.extent;
        break;
      }

      ekg::layout::extent_t::h_widget.begin_index = static_cast<float>(it);
      ekg::theme_t &current_global_theme {ekg::p_core->handler_theme.get_current_theme()};

      int32_t size {static_cast<int32_t>(parent_property.children.size())};
      int32_t latest_index {size - (!parent_property.children.empty())};
      int32_t flag_ok_count {};

      ekg::flags_t dock {};
      bool is_scrollbar {};
      bool is_last_index {};
      bool is_last_index_but {};
      bool is_ok {};
      bool is_stop {};

      ekg::rect_t<float> rect {};

      for (it = it; it < size; it++) {
        ekg::query<ekg::property_t> &property {
          ekg::query<ekg::property_t>(parent_property.children.at(it))
        };

        if (property == ekg::property_t::not_found) {
          continue;
        }

        is_scrollbar = property.descriptor_at.type == ekg::type::scrollbar;
        is_last_index = it == latest_index;

        ekg_abstract_todo(
          property.descriptor_at.type,
          property.descriptor_at,
          dock = descriptor.dock;
          rect = descriptor.rect;
        );

        is_ok = ekg::has(dock, flag_ok);
        is_stop = ekg::has(dock, flag_stop);

        if (
            (is_stop && it != in_out_count)
            ||
            is_last_index
            ||
            is_scrollbar
          ) {

          extent -= (current_global_theme.layout_offset) * (extent > 0.0f);

          is_last_index_but = (
            is_ok && is_last_index
          );

          flag_ok_count += (
            !is_stop
            &&
            is_last_index_but
          );

          is_last_index_but = (
            is_last_index
            &&
            !is_ok
            &&
            !is_stop
            &&
            !is_scrollbar
          );

          /**
           * Basically if the container/frame mother ends with any non flag ok (ekg::dock::fill)
           * it MUST add the width size to extend.
           * But the point is the scrollbar, he is not docknized here, then just bypass with `!is_scrollbar`.
           *
           * :blush:
           **/
          extent += (
            rect.w
            *
            is_last_index_but
          );

          /**
           * There are two glitches fixed here.
           * 
           * The first one happens when the widget contains `next | fill` flag but the previous row
           * count as one extra index, so the last widget does not fill with 1 but with the previous
           * amount of fill count.
           * 
           * The second one happens when the widget contains `next | fill` and `fill`, making the
           * previous row do not count as 1, so the widget is complete filled.
           * 
           * To fix this, we consider the two options:
           * NOT STOP or OK but NOT STOP.
           * 
           * Both case must fix this, if soon happens, we can already now the context of issue.
           **/
          is_last_index_but = (
            is_last_index
            &&
            (!is_stop || (is_ok && !is_stop))
            &&
            !is_scrollbar
          );

          ekg::layout::extent_t::h_widget.end_index = it + (is_last_index * is_last_index_but);
          ekg::layout::extent_t::h_widget.extent = extent;
          ekg::layout::extent_t::h_widget.count = flag_ok_count + (flag_ok_count == 0);\

          is_stop = true;

          break;
        }

        if (is_ok) {
          flag_ok_count++;
          continue;
        }

        extent += rect.w + current_global_theme.layout_offset;
      }

      in_out_count = flag_ok_count + (flag_ok_count == 0);
      break;
    }

    case ekg::axis::vertical: {
      break;
    }
  }
}
