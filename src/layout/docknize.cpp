#include "ekg/layout/docknize.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/layout/extentnize.hpp"

void ekg::layout::mask::preset(
  ekg::vec3_t<float> offset,
  ekg::flags_t axis,
  float initial_respective_size
) {
  this->axis = axis;
  this->offset = offset;
  this->respective_all = initial_respective_size;

  ekg::layout::extent_t::v_rect_descriptor = {};
  ekg::layout::extent_t::h_rect_descriptor = {};
}

void ekg::layout::mask::insert(
  ekg::rect_descriptor_t rect_descriptor
) {
  if (ekg::has(rect_descriptor.flags, ekg::dock::none)) {
    rect_descriptor.p_rect->w = 0.0f;
    rect_descriptor.p_rect->h = 0.0f;
    return;
  }

  this->rect_descriptor_list.push_back(rect_descriptor);
}

void ekg::layout::mask::docknize() {
  ekg::layout::fill_align_t fill_align {};

  int32_t count {};
  float dimensional_extent {};
  float rect_height {};
  float rect_width {};
  float dimension_width {};
  float dimension_height {};
  float dimension_bind {};

  bool is_left {};
  bool is_right {};
  bool is_top {};
  bool is_bottom {};
  bool is_not_concat {};
  bool is_bind_dimension_not_zero {};

  /**
   * All dock rects within left corner (ekg::dock::left)
   * starts from left to right.
   **/
  ekg::rect_t<float> left_corner {};

  /**
   * All dock rects within right corner (ekg::dock::right)
   * starts from right (dimension width based) to left.
   **/
  ekg::rect_t<float> right_corner {};

  /**
   * All dock rects within center left corner (ekg::dock::right & ekg::dock::center)
   * starts from right (dimension width divided by 2 based) to left.
   **/
  ekg::rect_t<float> center_left_corner {};

  /**
   * All dock rects within center right corner (ekg::dock::right & ekg::dock::center)
   * starts from left (dimension width divided by 2 based) to right.
   **/
  ekg::rect_t<float> center_right_corner {};

  switch (this->axis) {
  case ekg::axis::horizontal:
    if (this->rect_descriptor_list.empty()) {
      this->mask.w = this->respective_all;
      this->mask.h = this->offset.z;
      return;
    }

    this->mask.w = this->offset.x;
    this->mask.h = this->offset.z;

    dimension_width = this->respective_all;
    dimension_height = this->offset.z;

    left_corner.w += this->offset.x;
    right_corner.w += this->offset.x;

    center_left_corner.x = dimension_width / 2.0f;
    center_left_corner.w = this->offset.x;

    center_right_corner.x = dimension_width / 2.0f;
    center_right_corner.w = this->offset.x;

    for (size_t it {}; it < this->rect_descriptor_list.size(); it++) {
      ekg::rect_descriptor_t &rect_descriptor {this->rect_descriptor_list.at(it)};
      if (rect_descriptor.p_rect == nullptr) {
        continue;
      }

      is_left = ekg::has(rect_descriptor.flags, ekg::dock::left);
      is_right = ekg::has(rect_descriptor.flags, ekg::dock::right);
      is_bottom = ekg::has(rect_descriptor.flags, ekg::dock::bottom);
      is_top = ekg::has(rect_descriptor.flags, ekg::dock::top);
      is_not_concat = !ekg::has(rect_descriptor.flags, ekg::dock::concat);
      is_bind_dimension_not_zero = (dimension_bind > 0.0f);

      rect_width = rect_descriptor.p_rect->w;
      rect_height = rect_descriptor.p_rect->h;

      if (ekg::has(rect_descriptor.flags, ekg::dock::fill)) {
        count = 0;
        ekg::layout::extentnize_rect_descriptor(
          this->rect_descriptor_list,
          this->offset,
          ekg::dock::fill,
          ekg::dock::none,
          ekg::axis::horizontal,
          fill_align,
          dimensional_extent,
          count
        );

        rect_width = ekg::min_clamp(
          ekg::layout::transform_dimension_from_extent(
            this->respective_all,
            dimensional_extent,
            this->offset.x,
            count
          ),
          1.0f
        );
      }

      if (is_left) {
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + left_corner.w;
        rect_descriptor.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        rect_descriptor.p_rect->w = rect_width;
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + dimension_width - right_corner.w - rect_descriptor.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_left) {
        rect_descriptor.p_rect->w = rect_width;
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_left_corner.x - center_left_corner.w - rect_descriptor.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        center_left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        rect_descriptor.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_right_corner.x + center_right_corner.w;
        rect_descriptor.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((rect_descriptor.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        center_right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (rect_descriptor.flags == ekg::dock::center) {
        rect_descriptor.p_rect->w = rect_width;
        rect_descriptor.p_rect->x = (dimension_width / 2.0f) - (rect_descriptor.p_rect->w / 2.0f);
      }

      if (is_top | is_bottom) {
        rect_descriptor.p_rect->y = (
          is_top ? (this->offset.y) : (dimension_height - rect_height - this->offset.y)
        );
      } else {
        rect_descriptor.p_rect->y = (
          (dimension_height / 2.0f) - (rect_height / 2.0f)
        );
      }

      if (!is_not_concat && rect_descriptor.p_rect->w > dimension_bind) {
        dimension_bind = rect_descriptor.p_rect->w;
      } else if (is_not_concat) {
        dimension_bind = 0.0f;
      }

      this->mask.h = dimension_height;
    }

    this->mask.w = ekg::min_clamp(
      this->respective_all,
      this->mask.w
    );
    break;
  case ekg::axis::vertical:
    if (this->rect_descriptor_list.empty()) {
      this->mask.w = this->offset.z;
      this->mask.h = this->respective_all;
      return;
    }

    this->mask.w = this->offset.z;
    this->mask.h = this->offset.y;
    break;
  }

  this->rect_descriptor_list.clear();
}

ekg::rect_t<float> &ekg::layout::mask::get_rect() {
  return this->mask;
}

void ekg::layout::docknize_widget(
  ekg::ui::abstract *p_widget_parent
) {
  if (
      p_widget_parent->p_descriptor_rect == nullptr
      ||
      p_widget_parent == nullptr
      ||
      !p_widget_parent->properties.is_docknizable
  ) {
    return;
  }

  ekg::type type {p_widget_parent->properties.type};
  bool is_group {type == ekg::type::frame};
  ekg::rect_t<float> &abs_parent_rect {p_widget_parent->get_abs_rect()};

  if (!is_group || abs_parent_rect.w == 0 || abs_parent_rect.h == 0) {
    return;
  }

  if (p_widget_parent->states.is_targeting_absolute_parent) {
    p_widget_parent->states.is_targeting_absolute_parent = false;

    if (p_widget_parent->properties.p_abs_parent) {
      ekg::layout::docknize_widget(
        static_cast<ekg::ui::abstract*>(
          p_widget_parent->properties.p_abs_parent->p_widget
        )
      );
      return;
    }
  }

  ekg::rect_t<float> container_rect {*p_widget_parent->p_descriptor_rect};
  ekg::theme_t &current_global_theme {ekg::theme()};
  float initial_offset {static_cast<float>(current_global_theme.scrollbar.pixel_thickness)};

  if (
    p_widget_parent->states.is_scrolling.x
    ||
    p_widget_parent->states.is_scrolling.y
  ) {
    initial_offset *= static_cast<float>(!current_global_theme.symmetric_layout);
    container_rect.w -= initial_offset * static_cast<float>(p_widget_parent->states.is_scrolling.x);
    container_rect.h -= initial_offset * static_cast<float>(p_widget_parent->states.is_scrolling.y);
  }

  initial_offset = (
    static_cast<float>(current_global_theme.scrollbar.pixel_thickness)
    *
    static_cast<float>(current_global_theme.symmetric_layout)
  );

  float container_size_offset {(initial_offset + current_global_theme.layout_offset) * 2.0f};
  container_rect.w -= container_size_offset;
  container_rect.h -= container_size_offset;

  ekg::ui::abstract *p_widgets {};
  ekg::flags_t flags {};

  float dimensional_extent {};
  int32_t it {};
  int32_t count {};

  ekg::rect_t<float> parent_offset {
    current_global_theme.layout_offset + initial_offset,
    current_global_theme.layout_offset + initial_offset,
    0.0f,
    0.0f
  };

  bool should_reload_widget {};
  bool should_estimate_extent {};
  float max_previous_height {};

  ekg::layout::extent_t::h_widget = {};
  ekg::layout::extent_t h_extent_backup {};
  ekg::layout::fill_align_t fill_align {};

  bool is_left {};
  bool is_right {};
  bool is_top {};
  bool is_bottom {};
  bool is_fill {};
  bool is_next {};

  ekg::rect_t<float> corner_top_left {parent_offset};
  ekg::rect_t<float> corner_top_right {0.0f, parent_offset.y, 0.0f, 0.0f};
  ekg::rect_t<float> corner_bottom_left {parent_offset.x, 0.0f, 0.0f, 0.0f};
  ekg::rect_t<float> corner_bottom_right {};

  float highest_top {};
  float highest_bottom {};

  for (ekg::properties_t *&p_properties : p_widget_parent->properties.children) {
    if (p_properties == nullptr || p_properties->p_widget == nullptr) {
      continue;
    }

    p_widgets = static_cast<ekg::ui::abstract*>(p_properties->p_widget);
    if (p_widgets->p_descriptor_rect == nullptr) {
      continue;
    }

    // @TODO Prevent useless scrolling reload.
    p_widgets->on_reload();

    type = p_widgets->properties.type;
    flags = p_properties->dock;

    if (type == ekg::type::scrollbar) {
      it++;
      continue;
    }

    should_estimate_extent = true;

    is_right  = ekg::has(flags, ekg::dock::right);
    is_left   = ekg::has(flags, ekg::dock::left) || !is_right;
    is_bottom = ekg::has(flags, ekg::dock::bottom);
    is_top    = ekg::has(flags, ekg::dock::top) || !is_bottom;
    is_fill   = ekg::has(flags, ekg::dock::fill);
    is_next   = ekg::has(flags, ekg::dock::next);

    if (is_fill) {
      count = it;
      ekg::layout::extentnize_widget(
        p_widget_parent,
        ekg::dock::fill,
        ekg::dock::next | (is_top ? ekg::dock::bottom : ekg::dock::top),
        ekg::axis::horizontal,
        fill_align,
        dimensional_extent,
        count
      );

      dimensional_extent = ekg::min_clamp(
        ekg::layout::transform_dimension_from_extent(
          container_rect.w,
          dimensional_extent,
          current_global_theme.layout_offset,
          count
        ),
        p_widgets->min_size.x
      );

      p_widgets->p_descriptor_rect->w = dimensional_extent;

      should_reload_widget = true;
      should_estimate_extent = false;
    }

    switch (flags & ekg::dock::bottom) {
    case ekg::dock::bottom:
      if (ekg::fequalsf(corner_bottom_right.y, 0.0f)) {
        corner_bottom_right.y += highest_bottom + current_global_theme.layout_offset;
        corner_bottom_left.y = corner_bottom_right.y;
      }

      if (is_next && is_left) {
        corner_bottom_left.x = parent_offset.x;
        corner_bottom_right.x = 0.0f;

        corner_bottom_left.y += highest_bottom + current_global_theme.layout_offset;
        corner_bottom_right.y = corner_bottom_left.y;
        highest_bottom = 0.0f;
      }

      if (is_left) {
        p_widgets->p_descriptor_rect->x = corner_bottom_left.x;
        p_widgets->p_descriptor_rect->y = (
          ekg::min_clamp(
            ekg::layout::transform_to_pixel_perfect_position(
              corner_top_right.y,
              corner_bottom_right.y,
              container_rect.h,
              current_global_theme.layout_offset
            ),
            corner_top_right.y + current_global_theme.layout_offset + highest_bottom
          )
        );

        corner_bottom_left.x += p_widgets->p_descriptor_rect->w + current_global_theme.layout_offset;
      }

      if (is_next && is_right) {
        corner_bottom_left.x = parent_offset.x;
        corner_bottom_right.x = 0.0f;

        corner_bottom_right.y += highest_bottom + current_global_theme.layout_offset;
        corner_bottom_left.y = corner_bottom_right.y;
        highest_bottom = 0.0f;
      }

      if (is_right) {
        corner_bottom_right.x += p_widgets->p_descriptor_rect->w;
        p_widgets->p_descriptor_rect->x = (
          ekg::layout::transform_to_pixel_perfect_position(
            corner_bottom_left.x,
            corner_bottom_right.x,
            container_rect.w,
            current_global_theme.layout_offset
          )
        );

        p_widgets->p_descriptor_rect->y = (
          ekg::min_clamp<float>(
            ekg::layout::transform_to_pixel_perfect_position(
              corner_top_right.y,
              corner_bottom_right.y,
              container_rect.h,
              current_global_theme.layout_offset
            ),
            corner_top_right.y + current_global_theme.layout_offset + highest_bottom
          )
        );

        corner_bottom_right.x += current_global_theme.layout_offset;
      }

      highest_bottom = ekg::min_clamp(highest_bottom, p_widgets->p_descriptor_rect->h);
      break;
    default:
      if (is_next && is_left) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_left.y += highest_top + current_global_theme.layout_offset;
        corner_top_right.y = corner_top_left.y;
        highest_top = 0.0f;
      }

      if (is_left) {
        p_widgets->p_descriptor_rect->x = corner_top_left.x;
        p_widgets->p_descriptor_rect->y = corner_top_left.y;
  
        corner_top_left.x += p_widgets->p_descriptor_rect->w + current_global_theme.layout_offset;
      }

      if (is_next && is_right) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_right.y += highest_top + current_global_theme.layout_offset;
        corner_top_left.y = corner_top_right.y;
        highest_top = 0.0f;
      }

      if (is_right) {
        corner_top_right.x += p_widgets->p_descriptor_rect->w;
        p_widgets->p_descriptor_rect->x = (
          ekg::layout::transform_to_pixel_perfect_position(
            corner_top_left.x,
            corner_top_right.x,
            container_rect.w,
            current_global_theme.layout_offset
          )
        );

        corner_top_right.x += current_global_theme.layout_offset;
        p_widgets->p_descriptor_rect->y = corner_top_right.y;
      }

      highest_top = ekg::min_clamp(highest_top, p_widgets->p_descriptor_rect->h);
      break;
    }

    if (should_estimate_extent) {
      count = it;
      ekg::layout::extentnize_widget(
        p_widget_parent,
        ekg::dock::fill,
        ekg::dock::next | (is_top ? ekg::dock::bottom : ekg::dock::top),
        ekg::axis::horizontal,
        fill_align,
        dimensional_extent,
        count
      );
    }

    if (
      !fill_align.was_pixel_perfect_calculated
      &&
      fill_align.must_calculate_pixel_perfect
    ) {
      fill_align.align = container_rect.w - (p_widgets->p_descriptor_rect->x + p_widgets->p_descriptor_rect->w);
      fill_align.was_pixel_perfect_calculated = true;
      
      corner_top_right.x = fill_align.align;
      corner_bottom_right.x = fill_align.align + current_global_theme.layout_offset;
    } else if (
      is_fill
      &&
      fill_align.was_last_fill_found
      &&
      fill_align.was_pixel_perfect_calculated
    ) {
      p_widgets->p_descriptor_rect->w = (
        (container_rect.w - p_widgets->p_descriptor_rect->x) - fill_align.align
      );
    }

    max_previous_height = p_widgets->p_descriptor_rect->h > max_previous_height ? p_widgets->p_descriptor_rect->h : max_previous_height;
    if (should_reload_widget) {
      p_widgets->on_reload();
      should_reload_widget = false;
    }

    h_extent_backup = ekg::layout::extent_t::h_widget;
    if (p_properties->is_docknizable && !p_properties->children.empty()) {
      ekg::layout::docknize_widget(p_widgets);
    }

    ekg::layout::extent_t::h_widget = h_extent_backup;
    it++;
  }

  // TODO: may is necessary to re-docknize the parent widget if previous scroll is disabled but now enabled
}
