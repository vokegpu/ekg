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
#include "ekg/layout/docknize.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/layout/extentnize.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/math/floating_point.hpp"

void ekg::layout::mask::preset(
  ekg::vec3_t<float> offset,
  ekg::flags_t axis,
  float initial_respective_size
) {
  this->axis = axis;
  this->offset = offset;
  this->respective_all = initial_respective_size;

  ekg::layout::extent_t::v_mask = {};
  ekg::layout::extent_t::h_mask = {};
}

void ekg::layout::mask::insert(
  const ekg::layout::mask::component_t &component
) {
  if (ekg::has(component.dock, ekg::dock::none)) {
    component.p_rect->w = 0.0f;
    component.p_rect->h = 0.0f;
    return;
  }

  this->components.push_back(component);
}

void ekg::layout::mask::docknize() {
  size_t size {};
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
    if (this->components.empty()) {
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

    size = this->components.size();
    for (size_t it {}; it < size; it++) {
      ekg::layout::mask::component_t &component {this->components.at(it)};
      if (component.p_rect == nullptr) {
        continue;
      }

      is_left = ekg::has(component.dock, ekg::dock::left);
      is_right = ekg::has(component.dock, ekg::dock::right);
      is_bottom = ekg::has(component.dock, ekg::dock::bottom);
      is_top = ekg::has(component.dock, ekg::dock::top);
      is_not_concat = !ekg::has(component.dock, ekg::dock::concat);
      is_bind_dimension_not_zero = (dimension_bind > 0.0f);

      rect_width = component.p_rect->w;
      rect_height = component.p_rect->h;

      if (ekg::has(component.dock, ekg::dock::fill)) {
        count = 0;
        ekg::layout::extentnize_mask(
          this->components,
          this->offset,
          ekg::dock::fill,
          ekg::dock::none,
          ekg::axis::horizontal,
          dimensional_extent,
          count
        );

        rect_width = ekg::clamp_min(
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
        component.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + left_corner.w;
        component.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((component.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        component.p_rect->w = rect_width;
        component.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + dimension_width - right_corner.w - component.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((component.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_left) {
        component.p_rect->w = rect_width;
        component.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_left_corner.x - center_left_corner.w - component.p_rect->w;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((component.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        center_left_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (is_right) {
        component.p_rect->x = (is_bind_dimension_not_zero * this->offset.x) + center_right_corner.x + center_right_corner.w;
        component.p_rect->w = rect_width;

        dimension_bind += (
          ((this->offset.x * is_bind_dimension_not_zero) + ((component.p_rect->w + this->offset.x) * !is_bind_dimension_not_zero))
          *
          is_not_concat
        );

        center_right_corner.w += dimension_bind;
        this->mask.w += dimension_bind;
      } else if (component.dock == ekg::dock::center) {
        component.p_rect->w = rect_width;
        component.p_rect->x = (dimension_width / 2.0f) - (component.p_rect->w / 2.0f);
      }

      if (is_top | is_bottom) {
        component.p_rect->y = (
          is_top ? (this->offset.y) : (dimension_height - rect_height - this->offset.y)
        );
      } else {
        component.p_rect->y = (
          (dimension_height / 2.0f) - (rect_height / 2.0f)
        );
      }

      if (!is_not_concat && component.p_rect->w > dimension_bind) {
        dimension_bind = component.p_rect->w;
      } else if (is_not_concat) {
        dimension_bind = 0.0f;
      }

      this->mask.h = dimension_height;
    }

    this->mask.w = ekg::clamp_min(
      this->respective_all,
      this->mask.w
    );
    break;
  case ekg::axis::vertical:
    if (this->components.empty()) {
      this->mask.w = this->offset.z;
      this->mask.h = this->respective_all;
      return;
    }

    this->mask.w = this->offset.z;
    this->mask.h = this->offset.y;
    break;
  }

  this->components.clear();
}

ekg::rect_t<float> &ekg::layout::mask::get_rect() {
  return this->mask;
}

void ekg::layout::docknize_widget(
  ekg::property_t &parent_property
) {
  if (
      parent_property == ekg::property_t::not_found
      ||
      !parent_property.widget.is_children_docknizable
  ) {
    return;
  }

  if (
    parent_property.widget.is_targeting_absolute_parent
    &&
    parent_property.abs_parent_at != ekg::at_t::not_found
  ) {
    parent_property.widget.is_targeting_absolute_parent = false;
    ekg::layout::docknize_widget(ekg::query<ekg::property_t>(parent_property.abs_parent_at));
    return;
  }

  if (parent_property.widget.should_refresh_size) {
    ekg_abstract_todo(
      parent_property.descriptor_at.flags,
      parent_property.descriptor_at,
      ekg::ui::reload(parent_property, descriptor);
    );
  }

  if (parent_property.widget.rect.w == 0.0f || parent_property.widget.rect.h == 0.0f) {
    return;
  }

  parent_property.widget.is_targeting_absolute_parent = false;

  ekg::rect_t<float> container_rect {parent_property.widget.rect};
  ekg::theme_t &current_global_theme {ekg::p_core->handler_theme.get_current_theme()};

  float margin {static_cast<float>(current_global_theme.layout_margin_thickness) * 2.0f};
  container_rect.w -= margin;
  container_rect.h -= margin;

  /**
   * Pixel correction to make margin aligned and symmetric. 
   **/
  container_rect.w -= (
    static_cast<ekg::pixel_thickness_t>(parent_property.widget.rect.w - container_rect.w)
    -
    current_global_theme.layout_margin_thickness
  );

  if (
    parent_property.scroll.is_scrolling.x
    ||
    parent_property.scroll.is_scrolling.y
  ) {
    float nearest_scroll_bar_thickness {
      static_cast<float>(parent_property.scroll.nearest_scroll_bar_thickness)
    };

    container_rect.w -= nearest_scroll_bar_thickness * static_cast<float>(parent_property.scroll.is_scrolling.x);
    container_rect.h -= nearest_scroll_bar_thickness * static_cast<float>(parent_property.scroll.is_scrolling.y);
  }

  ekg::flags_t flags {};

  float dimensional_extent {};
  float extent {};
  int32_t it {}; 
  int32_t count {};

  ekg::rect_t<float> parent_offset {
    margin,
    margin,
    0.0f,
    0.0f
  };

  bool should_reload_widget {};
  bool should_estimate_extent {};
  float max_previous_height {};

  ekg::layout::extent_t::h_widget = {};
  ekg::layout::extent_t h_extent_backup {};

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

  ekg::rect_t<float> pixel_perfect_projection {
    .x = parent_offset.x,
    .y = current_global_theme.layout_offset,
    .w = current_global_theme.layout_offset * 2.0f,
    .h = 0.0f
  };

  pixel_perfect_projection.w = ekg::layout::transform_dimension_from_extent(
    container_rect.w,
    0.0f,
    current_global_theme.layout_offset,
    1
  );

  float align {};
  float imperfect_pixel_max_bounding {pixel_perfect_projection.x + ekg::one_pixel * 5.0f};
  float projected_pixel_perfect_width {pixel_perfect_projection.x + pixel_perfect_projection.w};
  float unsolved_pixel_position {};

  ekg::rect_t<float> rect {};

  for (ekg::at_t &at : parent_property.children) {
    ekg::property_t &property {ekg::query<ekg::property_t>(at)};
    if (property == ekg::property_t::not_found) {
      continue;
    }

    if (property.widget.should_refresh_size) {
      property.widget.should_refresh_size = true;
    }

    ekg_abstract_todo(
      property.descriptor_at.flags,
      property.descriptor_at,
      ekg::ui::reload(property, descriptor);
      flags = descriptor.dock;
      rect = descriptor.rect;
    );

    if (property.at.flags == ekg::type::scrollbar) {
      it++;
      continue;
    }

    is_right  = ekg::has(flags, ekg::dock::right);
    is_left   = ekg::has(flags, ekg::dock::left) || !is_right;
    is_bottom = ekg::has(flags, ekg::dock::bottom);
    is_top    = ekg::has(flags, ekg::dock::top) || !is_bottom;
    is_fill   = ekg::has(flags, ekg::dock::fill);
    is_next   = ekg::has(flags, ekg::dock::next);

    should_estimate_extent = true;

    if (is_fill) {
      count = it;
      ekg::layout::extentnize_widget(
        parent_property,
        ekg::dock::fill,
        (is_top ? ekg::dock::bottom : ekg::dock::bottom),
        ekg::axis::horizontal,
        extent,
        count
      );

      dimensional_extent = ekg::clamp_min(
        ekg::layout::transform_dimension_from_extent(
          container_rect.w,
          extent,
          current_global_theme.layout_offset,
          count
        ),
        property.widget.min_size.x
      );

      if (is_bottom) {
        align = ((dimensional_extent + current_global_theme.layout_offset) * count) + (extent);
        align = (align - pixel_perfect_projection.w) * (align > pixel_perfect_projection.w) * (extent > 0.0f);
        align > 0.0f && (align = (align / count));
      } else {
        align = ((dimensional_extent + current_global_theme.layout_offset) * count) + (extent);
        align = (pixel_perfect_projection.w - align) * (align < pixel_perfect_projection.w) * (extent > 0.0f);
        align > 0.0f && (align = -(align / count));
      }

      rect.w = dimensional_extent - align;

      should_reload_widget = true;
      should_estimate_extent = false;
    }

    switch (flags & ekg::dock::bottom) {
    case ekg::dock::bottom:
      if (ekg::fequalsf(corner_bottom_right.y, 0.0f)) {
        highest_bottom = rect.h;
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
        rect.x = corner_bottom_left.x;
        rect.y = (
          ekg::clamp_min(
            ekg::layout::transform_to_pixel_perfect_position(
              corner_top_right.y,
              corner_bottom_right.y,
              container_rect.h,
              current_global_theme.layout_offset
            ),
            corner_top_right.y + current_global_theme.layout_offset + highest_bottom
          )
        );

        corner_bottom_left.x += rect.w + current_global_theme.layout_offset;
      }

      if (is_next && is_right) {
        corner_bottom_left.x = parent_offset.x;
        corner_bottom_right.x = 0.0f;

        corner_bottom_right.y += highest_bottom + current_global_theme.layout_offset;
        corner_bottom_left.y = corner_bottom_right.y;
        highest_bottom = 0.0f;
      }

      if (is_right) {
        corner_bottom_right.x += rect.w;

        rect.x = projected_pixel_perfect_width - corner_bottom_right.x;
        rect.y = container_rect.h - corner_bottom_right.y;

        corner_bottom_right.x += current_global_theme.layout_offset;
      }

      highest_bottom = ekg::clamp_min(highest_bottom, rect.h);
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
        rect.x = corner_top_left.x;
        rect.y = corner_top_left.y;
  
        corner_top_left.x += rect.w + current_global_theme.layout_offset;
      }

      if (is_next && is_right) {
        corner_top_left.x = parent_offset.x;
        corner_top_right.x = 0.0f;
        corner_top_right.y += highest_top + current_global_theme.layout_offset;
        corner_top_left.y = corner_top_right.y;
        highest_top = 0.0f;
      }

      if (is_right) {
        corner_top_right.x += rect.w;
        rect.x = (
          ekg::layout::transform_to_pixel_perfect_position(
            corner_top_left.x,
            corner_top_right.x,
            container_rect.w,
            current_global_theme.layout_offset
          )
        );

        corner_top_right.x += current_global_theme.layout_offset;
        rect.y = corner_top_right.y;
      }

      highest_top = ekg::clamp_min(highest_top, rect.h);
      break;
    }

    if (should_estimate_extent) {
      count = it;
      //ekg::layout::extentnize_widget(
      //  parent_property,
      //  ekg::dock::fill,
      //  ekg::dock::next | (is_top ? ekg::dock::bottom : ekg::dock::top),
      //  ekg::axis::horizontal,
      //  dimensional_extent,
      //  count
      //);
    }

    max_previous_height = rect.h > max_previous_height ? rect.h : max_previous_height;
    ekg_abstract_todo(
      property.descriptor_at.flags,
      property.descriptor_at,

      descriptor.rect = rect;

      if (should_reload_widget) {
        ekg::ui::reload(property, descriptor);
        should_reload_widget = false;
      }
    );

    h_extent_backup = ekg::layout::extent_t::h_widget;
    if (property.widget.is_children_docknizable && !property.children.empty()) {
      ekg::layout::docknize_widget(property);
    }

    ekg::layout::extent_t::h_widget = h_extent_backup;
    it++;
  }

  parent_property.widget.should_refresh_size = false;

  // TODO: may is necessary to re-docknize the parent widget if previous scroll is disabled but now enabled
}

float ekg::layout::get_widget_height_by_children(
  ekg::property_t &parent_property
) {
  if (
      parent_property == ekg::property_t::not_found
      ||
      !parent_property.widget.is_children_docknizable
  ) {
    return 0.0f;
  }

  ekg::theme_t &current_global_theme {ekg::p_core->handler_theme.get_current_theme()};
  ekg::flags_t flags {};

  float total_height {};
  float height {};

  for (ekg::at_t &at : parent_property.children) {
    ekg::property_t &property {ekg::query<ekg::property_t>(at)};
    if (
        property == ekg::property_t::not_found
      ) {
      continue;
    }

    ekg_abstract_todo(
      property.descriptor_at.flags,
      property.descriptor_at,
      flags = descriptor.dock;
    );

    height = property.widget.rect.h;

    if (
        property.widget.is_children_docknizable
        &&
        !property.children.empty()
      ) {
      height = ekg::layout::get_widget_height_by_children(property);
    }

    total_height += (
      height
      *
      (
        ekg::fequalsf(total_height, 0.0f)
        ||
        ekg::has(flags, ekg::dock::next)
      )
      +
      current_global_theme.layout_offset
    );
  }

  total_height += (
    current_global_theme.layout_offset // top
    +
    current_global_theme.layout_offset // top
    +
    current_global_theme.layout_offset // bottom
    +
    current_global_theme.layout_offset // bottom
  );

  return total_height;
}
