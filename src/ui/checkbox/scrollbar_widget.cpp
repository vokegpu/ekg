#include "ekg/ui/scrollbar/scrollbar_widget.hpp"
#include "ekg/draw/font_renderer.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/draw/shape.hpp"

float ekg::ui::scrollbar::get_horizontal_scroll_normalized() {
  if (this->descriptor.p_binded_rect == nullptr) {
    return 0.0f;
  }

  return (this->scroll.x / this->descriptor.p_binded_rect->w);
}

float ekg::ui::scrollbar::get_vertical_scroll_normalized() {
  if (this->descriptor.p_binded_rect == nullptr) {
    return 0.0f;
  }

  return (this->scroll.y / this->descriptor.p_binded_rect->h);
}

void ekg::ui::scrollbar::check_scroll() {
  if (this->descriptor.p_binded_rect == nullptr) {
    return;
  }

  this->descriptor.axis.x = this->scrollable_area.w > this->descriptor.p_binded_rect->w;
  this->descriptor.axis.y = this->scrollable_area.h > this->descriptor.p_binded_rect->h;
}

void ekg::ui::scrollbar::reset_scroll() {
  this->scroll.x = this->scroll.z;
  this->scroll.y = this->scroll.w;
}

void ekg::ui::scrollbar::clamp_scroll() {
  if (this->descriptor.p_binded_rect == nullptr) {
    return;
  }

  ekg::vec2_t<float> vertical_scroll_limit {0.0f, this->descriptor.p_binded_rect->h - this->scrollable_area.h};
  ekg::vec2_t<float> horizontal_scroll_limit {0.0f, this->descriptor.p_binded_rect->w - this->scrollable_area.w};

  if (this->scrollable_area.h < this->descriptor.p_binded_rect->h) {
    this->scroll.y = 0.0f;
    this->scroll.w = 0.0f;
  } else if (this->scroll.y < vertical_scroll_limit.y) {
    this->scroll.y = vertical_scroll_limit.y;
    this->scroll.w = vertical_scroll_limit.y;
  } else if (this->scroll.y > vertical_scroll_limit.x) {
    this->scroll.y = vertical_scroll_limit.x;
    this->scroll.w = vertical_scroll_limit.x;
  }

  if (this->scrollable_area.w < this->descriptor.p_binded_rect->w) {
    this->scroll.x = 0.0f;
    this->scroll.z = 0.0f;
  } else if (this->scroll.x < horizontal_scroll_limit.y) {
    this->scroll.x = horizontal_scroll_limit.y;
    this->scroll.z = horizontal_scroll_limit.y;
  } else if (this->scroll.x > horizontal_scroll_limit.x) {
    this->scroll.x = horizontal_scroll_limit.x;
    this->scroll.z = horizontal_scroll_limit.x;
  }
}

bool ekg::ui::scrollbar::is_scrolling(bool state) {
  state = (
    state
    ||
    static_cast<int32_t>(roundf(this->scroll.x)) != static_cast<int32_t>(roundf(this->scroll.z))
    ||
    static_cast<int32_t>(roundf(this->scroll.y)) != static_cast<int32_t>(roundf(this->scroll.w))
  );

  if (!state) {
    this->scroll.x = this->scroll.z;
    this->scroll.y = this->scroll.w;
  }

  return state;
}

void ekg::ui::scrollbar::on_create() {
  ekg::ui::abstract::on_create();

  if (!this->descriptor.p_binded_rect) {
    this->descriptor.p_binded_rect = &this->scrollable_area;
  }

  if (!this->p_descriptor_rect) {
    this->p_descriptor_rect = &this->_blank_descriptor_rect;
  }
}

void ekg::ui::scrollbar::on_reload() {
  if (
    this->descriptor.p_binded_rect == nullptr
  ) { 
    return;
  }

  this->check_scroll();

  if (
    this->descriptor.p_binded_children == nullptr
  ) { 
    return;
  }

  this->scrollable_area = ekg::rect_t<float> {};
  this->acceleration.x = ekg::draw::get_font_renderer(ekg::font::normal).get_text_height();

  float place {};
  for (ekg::properties_t *&p_properties : *this->descriptor.p_binded_children) {
    if (
      p_properties == nullptr
      ||
      p_properties->p_widget == nullptr
      ||
      p_properties->type == ekg::type::scrollbar
    ) {
      continue;
    }

    ekg::ui::abstract *p_widget {
      static_cast<ekg::ui::abstract*>(p_properties->p_widget)
    };

    place = p_widget->p_descriptor_rect->x + p_widget->p_descriptor_rect->w;
    if (place > this->scrollable_area.w) {
      this->scrollable_area.w = place;
    }

    place = p_widget->p_descriptor_rect->y + p_widget->p_descriptor_rect->h;
    if (place > this->scrollable_area.h) {
      this->scrollable_area.h = place;
    }

    if (p_widget->p_descriptor_rect->h < this->acceleration.y) {
      this->acceleration.y = p_widget->p_descriptor_rect->h;
    }

    p_widget->p_scroll_vec = &this->scroll;
  }

  this->acceleration.y += this->acceleration.x + (this->acceleration.x / 2.0f);

  this->check_scroll();
  this->clamp_scroll();
}

void ekg::ui::scrollbar::on_event(ekg::io::stage stage) {
  if (this->descriptor.p_binded_rect == nullptr) {
    return;
  }

  switch (stage) {
    case ekg::io::stage::pre: {
      ekg::input_t &input {ekg::p_core->service_input.input};
      ekg::vec2_t<float> interact {input.interact};

      if (
          input.was_pressed
          ||
          input.was_released
          ||
          input.has_motion
          ||
          input.was_wheel
      ) {
        ekg::rect_t<float> bar_horizontal {this->bar_horizontal};
        bar_horizontal.x += this->descriptor.p_binded_rect->x;
    
        ekg::rect_t<float> bar_vertical {this->bar_vertical};
        bar_vertical.y += this->descriptor.p_binded_rect->y;
    
        bool visible {
          ekg::rect_collide_vec2<float>(this->scissor, interact)
        };

        this->states.is_active = (
          visible
          &&
          (this->descriptor.axis.x || this->descriptor.axis.y)
          &&
          ekg::fire("scrollbar-scroll")
        );

        this->bar_horizontal_states.is_hovering = (
          visible
          &&
          ekg::rect_collide_vec2(bar_horizontal, interact)
        );

        this->bar_vertical_states.is_hovering = (
          visible
          &&
          ekg::rect_collide_vec2(bar_vertical, interact)
        );

        this->states.is_hovering = (
          this->states.is_active
          ||
          this->bar_horizontal_states.is_hovering
          ||
          this->bar_vertical_states.is_hovering
        );
      }

      this->states.is_absolute = this->states.is_scrolling.x || this->states.is_scrolling.y || this->states.is_active;

      break;
    }

    case ekg::io::stage::process: {
      this->check_scroll();

      ekg::input_t &input {ekg::p_core->service_input.input};
      bool is_scroll_fired {ekg::fire("scrollbar-scroll")};

      this->states.is_scrolling.x = false;
      this->states.is_scrolling.y = false;

      #if defined(ANROID)
        if (this->states.is_hovering && this->descriptor.axis.x &is_scroll_fired) {
          this->states.is_scrolling.x = true;

          this->scroll.z = ekg::clamp<float>(
            this->scroll.x + (-input.interact.z * this->acceleration.y),
            this->descriptor.p_binded_rect->w - this->scrollable_area.w,
            0.0f
          );
        }

        if (this->states.is_hovering && this->descriptor.axis.y && is_scroll_fired) {
          this->states.is_scrolling.y = true;
          this->scroll.w = ekg::clamp<float>(
            this->scroll.y + (input.interact.w * this->acceleration.y),
            this->descriptor.p_binded_rect->h - this->scrollable_area.h,
            0.0f
          );
        }
      #else
        bool is_scroll_horizontal_fired {
          ekg::fire("scrollbar-scroll-horizontal")
        };

        if (this->states.is_hovering && this->descriptor.axis.x && is_scroll_fired && is_scroll_horizontal_fired) {
          this->states.is_scrolling.x = true;
          this->scroll.z = ekg::clamp<float>(
            this->scroll.x + (input.interact.w * this->acceleration.x),
            this->descriptor.p_binded_rect->w - this->scrollable_area.w,
            0.0f
          );
        }

        if (this->states.is_hovering && this->descriptor.axis.y && is_scroll_fired && !is_scroll_horizontal_fired) {
          this->states.is_scrolling.y = true;
          this->scroll.w = ekg::clamp<float>(
            this->scroll.y + (input.interact.w * this->acceleration.y),
            this->descriptor.p_binded_rect->h - this->scrollable_area.h,
            0.0f
          );
        }
      #endif

      if (input.has_motion) {
        ekg::io::set<bool>(
          this->bar_horizontal_states.is_highlighting,
          this->bar_horizontal_states.is_hovering
        );

        ekg::io::set<bool>(
          this->bar_vertical_states.is_highlighting,
          this->bar_vertical_states.is_hovering
        );
      }

      if (
        this->states.is_hovering
        &&
        input.was_pressed
        &&
        ekg::fire("scrollbar-drag")
      ) {
        ekg::rect_t<float> bar_horizontal {this->bar_horizontal};
        bar_horizontal.x += this->descriptor.p_binded_rect->x;
        this->delta.x = input.interact.x - bar_vertical.x;

        ekg::io::set<bool>(
          this->bar_horizontal_states.is_active,
          this->bar_horizontal_states.is_hovering
        );

        ekg::rect_t<float> bar_vertical {this->bar_vertical};
        bar_vertical.y += this->descriptor.p_binded_rect->y;
        this->delta.y = input.interact.y - bar_vertical.y;

        ekg::io::set<bool>(
          this->bar_vertical_states.is_active,
          this->bar_vertical_states.is_hovering
        );
      }

      if (
        input.has_motion
        &&
        this->bar_horizontal_states.is_active
        &&
        !is_scroll_fired
      ) {
        this->states.is_scrolling.x = true;
        ekg::rect_t<float> bar_horizontal {this->bar_horizontal};
        bar_horizontal.x = input.interact.x - this->delta.x;
        bar_horizontal.x -= this->descriptor.p_binded_rect->x;

        this->scroll.z = (
          (
            -ekg::clamp<float>(
              bar_horizontal.x / (this->descriptor.p_binded_rect->w - this->bar_horizontal.w),
              0.0f,
              1.0f
            )
          )
          *
          (
            this->scrollable_area.w - this->descriptor.p_binded_rect->w
          )
        );

        this->scroll.x = this->scroll.z;
        ekg::viewport.redraw = true;
      }

      if (
        input.has_motion
        &&
        this->bar_vertical_states.is_active
        &&
        !is_scroll_fired
      ) {
        this->states.is_scrolling.y = true;
        ekg::rect_t<float> bar_vertical {this->bar_horizontal};
        bar_vertical.y = input.interact.y - this->delta.y;
        bar_vertical.y -= this->descriptor.p_binded_rect->y;

        this->scroll.w = (
          (
            -ekg::clamp<float>(
              bar_vertical.y / (this->descriptor.p_binded_rect->h - this->bar_vertical.h),
              0.0f,
              1.0f
            )
          )
          *
          (
            this->scrollable_area.h - this->descriptor.p_binded_rect->h
          )
        );

        this->scroll.y = this->scroll.w;
      }

      if (input.was_released) {
        ekg::io::set<bool>(
          this->bar_horizontal_states.is_active,
          (this->states.is_scrolling.x = false)
        );

        ekg::io::set<bool>(
          this->bar_vertical_states.is_active,
          (this->states.is_scrolling.y = false)
        );
      }

      if (this->properties.p_parent && this->properties.p_parent->p_widget) {
        static_cast<ekg::ui::abstract*>(this->properties.p_parent->p_widget)
          ->states.is_scrolling = this->descriptor.axis;
      }

      if ((this->states.is_active || this->states.is_absolute) && !this->states.is_high_frequency) {
        ekg::p_core->dispatch_widget_op(this, ekg::io::operation::high_frequency);
      }

      if (this->bar_horizontal_states.is_hovering || this->bar_vertical_states.is_hovering) {
        ekg::p_core->p_os_platform->system_cursor = ekg::system_cursor_type::arrow;
      }

      if (this->states.is_scrolling.x || this->states.is_scrolling.y) {
        ekg::viewport.redraw = true;
      }

      break;
    }

    case ekg::io::stage::post: {
      this->states.is_hovering = false;
      this->bar_horizontal_states.is_hovering = false;
      this->bar_vertical_states.is_hovering = false;
      break;
    }
  }
}

void ekg::ui::scrollbar::on_update() {
  float delta {
    ekg::p_core->service_input.input.scroll_speed
    *
    ekg::viewport.dt
  };

  this->scroll.x = ekg::lerp<float>(
    this->scroll.x,
    this->scroll.z,
    delta
  );

  this->scroll.y = ekg::lerp<float>(
    this->scroll.y,
    this->scroll.w,
    delta
  );

  #if defined(__ANDROID__)
    this->clamp_scroll();
  #endif

  if (
      (
        this->states.is_high_frequency = (
          this->is_scrolling(
            this->states.is_hovering || this->states.is_active
          )
        )
      )
  ) {
    ekg::viewport.redraw = true;
  }
}

void ekg::ui::scrollbar::on_draw() {
  if (this->descriptor.p_binded_rect == nullptr) {
    return;
  }

  this->p_descriptor_rect->w = this->descriptor.p_binded_rect->w;
  this->p_descriptor_rect->h = this->descriptor.p_binded_rect->h;

  EKG_ASSERT_SCISSOR(
    this->scissor,
    this->get_abs_rect(),
    this->p_parent_scissor_rect
  );

  this->bar_horizontal.w = 0.0f;
  this->bar_vertical.h = 0.0f;

  this->descriptor.axis.x = this->scrollable_area.w > this->descriptor.p_binded_rect->w;
  this->descriptor.axis.y = this->scrollable_area.h > this->descriptor.p_binded_rect->h;

  if (!this->descriptor.axis.x && !this->descriptor.axis.y) {
    return;
  }

  this->bar_vertical.w = static_cast<float>(this->descriptor.theme.pixel_thickness * this->descriptor.axis.y);
  this->bar_vertical.x = (
    this->descriptor.p_binded_rect->x
    +
    this->descriptor.p_binded_rect->w
    -
    this->bar_vertical.w
  );

  float out_of_binded_rect_width {(this->scrollable_area.h - this->descriptor.p_binded_rect->h)};
  float y_pos_factor {abs(this->scroll.y) / out_of_binded_rect_width};

  this->bar_vertical.h = (
    this->descriptor.p_binded_rect->h - (
      static_cast<int32_t>(out_of_binded_rect_width) < 0
      ?
      this->descriptor.p_binded_rect->h
      :
      ekg::clamp_max<float>(
        out_of_binded_rect_width,
        this->descriptor.p_binded_rect->h - this->descriptor.theme.min_bar_size
      )
    )
  );

  this->bar_vertical.y = y_pos_factor * (this->descriptor.p_binded_rect->h - this->bar_vertical.h);

  ekg::draw::rect(
    this->bar_vertical.x,
    this->descriptor.p_binded_rect->y + this->bar_vertical.y,
    this->bar_vertical.w,
    this->bar_vertical.h,
    this->descriptor.theme.background,
    ekg::draw_mode::filled,
    this->descriptor.theme.layers[ekg::scrollbar_t::vertical][ekg::layer::background]
  );

  if (this->bar_vertical_states.is_highlighting) {
    ekg::draw::rect(
      this->bar_vertical.x,
      this->descriptor.p_binded_rect->y + this->bar_vertical.y,
      this->bar_vertical.w,
      this->bar_vertical.h,
      this->descriptor.theme.highlight,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::scrollbar_t::vertical][ekg::layer::highlight]
    );
  }

  if (this->bar_vertical_states.is_active) {
    ekg::draw::rect(
      this->bar_vertical.x,
      this->descriptor.p_binded_rect->y + this->bar_vertical.y,
      this->bar_vertical.w,
      this->bar_vertical.h,
      this->descriptor.theme.active,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::scrollbar_t::vertical][ekg::layer::active]
    );
  }

  ekg::draw::rect(
    this->bar_vertical.x,
    this->descriptor.p_binded_rect->y + this->bar_vertical.y,
    this->bar_vertical.w,
    this->bar_vertical.h,
    this->descriptor.theme.outline,
    ekg::draw_mode::filled
  );

  this->bar_horizontal.h = static_cast<float>(this->descriptor.theme.pixel_thickness * this->descriptor.axis.x);
  this->bar_horizontal.y = (
    this->descriptor.p_binded_rect->y + this->descriptor.p_binded_rect->h - this->bar_horizontal.h
  );

  float out_of_mother_width {(this->scrollable_area.w - this->descriptor.p_binded_rect->w)};
  float x_pos_factor {abs(this->scroll.x) / out_of_mother_width};

  this->bar_horizontal.w = (
    this->descriptor.p_binded_rect->w
    -
    (
      static_cast<int32_t>(out_of_mother_width) < 0
      ?
      this->descriptor.p_binded_rect->w
      :
      ekg::clamp_max<float>(
        out_of_mother_width,
        this->descriptor.p_binded_rect->w - this->descriptor.theme.min_bar_size
      )
    )
  );

  this->bar_horizontal.x = x_pos_factor * (this->descriptor.p_binded_rect->w - this->bar_horizontal.w);

  ekg::draw::rect(
    this->descriptor.p_binded_rect->x + this->bar_horizontal.x,
    this->bar_horizontal.y,
    this->bar_horizontal.w,
    this->bar_horizontal.h,
    this->descriptor.theme.background,
    ekg::draw_mode::filled,
    this->descriptor.theme.layers[ekg::scrollbar_t::horizontal][ekg::layer::background]
  );

  if (this->bar_horizontal_states.is_highlighting) {
    ekg::draw::rect(
      this->descriptor.p_binded_rect->x + this->bar_horizontal.x,
      this->bar_horizontal.y,
      this->bar_horizontal.w,
      this->bar_horizontal.h,
      this->descriptor.theme.highlight,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::scrollbar_t::horizontal][ekg::layer::highlight]
    );
  }

  if (this->bar_horizontal_states.is_active) {
    ekg::draw::rect(
      this->descriptor.p_binded_rect->x + this->bar_horizontal.x,
      this->bar_horizontal.y,
      this->bar_horizontal.w,
      this->bar_horizontal.h,
      this->descriptor.theme.active,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::scrollbar_t::horizontal][ekg::layer::active]
    );
  }

  ekg::draw::rect(
    this->descriptor.p_binded_rect->x + this->bar_horizontal.x,
    this->bar_horizontal.y,
    this->bar_horizontal.w,
    this->bar_horizontal.h,
    this->descriptor.theme.outline,
    ekg::draw_mode::outline
  );
}
