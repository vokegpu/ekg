#include "ekg/layout/extentnize.hpp"
#include "ekg/core/runtime.hpp"

ekg::layout::extent_t ekg::layout::extent_t::v_widget {};
ekg::layout::extent_t ekg::layout::extent_t::h_widget {};
ekg::layout::extent_t ekg::layout::extent_t::v_rect_descriptor {};
ekg::layout::extent_t ekg::layout::extent_t::h_rect_descriptor {};

void ekg::layout::extentnize_rect_descriptor(
  std::vector<ekg::rect_descriptor_t> &rect_descriptor_list,
  ekg::vec3_t<float> offset,
  ekg::flags_t flag_ok,
  ekg::flags_t flag_stop,
  ekg::flags_t flag_axis,
  ekg::layout::fill_align_t &fill_align,
  float &extent,
  int32_t &in_out_count
) {
  extent = 0.0f;
  switch (flag_axis & ekg::axis::horizontal) {
    case ekg::axis::horizontal: {
      int32_t it {in_out_count};

      if (
          it > ekg::layout::extent_t::h_rect_descriptor.begin_index
          &&
          it < ekg::layout::extent_t::h_rect_descriptor.end_index
        ) {
        in_out_count = ekg::layout::extent_t::h_rect_descriptor.count;
        extent = ekg::layout::extent_t::h_rect_descriptor.extent;
        return;
      }

      ekg::layout::extent_t::h_rect_descriptor.begin_index = static_cast<float>(it);

      int32_t size {static_cast<int32_t>(rect_descriptor_list.size())};
      int32_t latest_index {static_cast<int32_t>(size - (!rect_descriptor_list.empty()))};
      int32_t should_skip_next {};
      int32_t flag_ok_count {};

      bool is_last_index {};
      bool is_ok_flag {};
 
      extent += offset.x;

      for (it = it; it < size; it++) {
        ekg::rect_descriptor_t &rect_descriptor {rect_descriptor_list.at(it)};
        if (rect_descriptor.p_rect == nullptr) {
          continue;
        }

        is_last_index = it == latest_index;

        if (
            (ekg::has(rect_descriptor.flags, flag_stop) && it != in_out_count)
            ||
            is_last_index
          ) {
          extent -= offset.x;
          flag_ok_count += (
            (is_ok_flag = (!ekg::has(rect_descriptor.flags, flag_stop) && (ekg::has(rect_descriptor.flags, flag_ok)) && is_last_index))
          );

          /**
           * Basically if the container/frame mother ends with any non flag ok (ekg::dock::fill)
           * it MUST add the width size to extend.
           *
           * :blush:
           **/
          extent += ( 
            (rect_descriptor.p_rect->w + offset.x)
            *
            (is_last_index && (!ekg::has(rect_descriptor.flags, flag_ok) && should_skip_next == 0))
          );

          ekg::layout::extent_t::h_rect_descriptor.end_index = it + is_last_index;
          ekg::layout::extent_t::h_rect_descriptor.extent = extent;
          ekg::layout::extent_t::h_rect_descriptor.count = flag_ok_count + (flag_ok_count == 0);
          break;
        }

        should_skip_next += ekg::has(rect_descriptor.flags, ekg::dock::concat);

        if (should_skip_next > 0) {
          should_skip_next = (should_skip_next + 1) * (should_skip_next < 2);
          flag_ok_count += ekg::has(rect_descriptor.flags, flag_ok);
          continue;
        }

        if (ekg::has(rect_descriptor.flags, flag_ok)) {
          flag_ok_count++;
          continue;
        }

        extent += rect_descriptor.p_rect->w + offset.x;
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
  ekg::ui::abstract *p_widget,
  ekg::flags_t flag_ok,
  ekg::flags_t flag_stop,
  ekg::flags_t flag_axis,
  ekg::layout::fill_align_t &fill_align,
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
      ekg::ui::abstract *p_widgets {};
      ekg::theme_t &current_global_theme {ekg::p_core->service_theme.get_current_theme()};

      int32_t size {static_cast<int32_t>(p_widget->properties.children.size())};
      int32_t latest_index {size - (!p_widget->properties.children.empty())};
      int32_t flag_ok_count {};

      bool is_scrollbar {};
      bool is_last_index {};
      bool is_last_index_but {};
      bool is_ok {};
      bool is_stop {};

      for (it = it; it < size; it++) {
        ekg::properties_t *&p_properties {p_widget->properties.children.at(it)};
        if (p_properties == nullptr) {
          continue;
        }

        p_widgets = static_cast<ekg::ui::abstract*>(p_properties->p_widget);
        if (p_widgets->p_descriptor_rect == nullptr) {
          continue;
        }

        is_scrollbar = p_properties->type == ekg::type::scrollbar;
        is_last_index = it == latest_index;

        is_ok = ekg::has(p_properties->dock, flag_ok);
        is_stop = ekg::has(p_properties->dock, flag_stop);

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
            p_widgets->p_descriptor_rect->w
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

          fill_align.index = is_last_index_but ? it - (it > 0) : -1;

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

        extent += p_widgets->p_descriptor_rect->w + current_global_theme.layout_offset;
      }


      in_out_count = flag_ok_count + (flag_ok_count == 0);
      if ((!fill_align.was_found && is_stop) || (!fill_align.must_calculate_pixel_perfect && fill_align.previous_end_index == begin_index)) {
        fill_align.was_found = true;
        if (fill_align.previous_end_index == begin_index) {
          fill_align.index = begin_index;
          ekg::log() << p_widgets->properties.tag;
        }
      }

      if (is_stop) {
        fill_align.previous_end_index = it;
      }

      break;
    }

    case ekg::axis::vertical: {
      break;
    }
  }

  fill_align.was_last_fill_found = fill_align.index == begin_index;
  if (
      fill_align.was_found
      &&
      !fill_align.must_calculate_pixel_perfect
      &&
      fill_align.was_last_fill_found
  ) {
    fill_align.must_calculate_pixel_perfect = true;
  }
}
