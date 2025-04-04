#include "ekg/ui/scrollbar/scrollbar_widget.hpp"
#include "ekg/draw/font_renderer.hpp"
#include "ekg/core/runtime.hpp"

float ekg::ui::scrollbar::get_horizontal_scroll_normalized() {
  return (this->scroll.x / this->descriptor.p_binded_rect->w);
}

float ekg::ui::scrollbar::get_vertical_scroll_normalized() {
  return (this->scroll.y / this->descriptor.p_binded_rect->h);
}

void ekg::ui::scrollbar::check_scroll() {
  this->descriptor.axis.x = this->scrollable_area.h > this->descriptor.p_binded_rect->h;
  this->descriptor.axis.y = this->scrollable_area.w > this->descriptor.p_binded_rect->w;
}

void ekg::ui::scrollbar::reset_scroll() {
  this->scroll.x = this->scroll.z;
  this->scroll.y = this->scroll.w;
}

void ekg::ui::scrollbar::clamp_scroll() {
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
  if (!this->descriptor.p_binded_rect) {
    this->descriptor.p_binded_rect = &this->scrollable_area;
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
        bar_horizontal.y += this->descriptor.p_binded_rect->y;
    
        ekg::rect_t<float> bar_vertical {this->bar_vertical};
        bar_vertical.x += this->descriptor.p_binded_rect->x;
    
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
    
        this->states.is_hovering = (
          this->states.is_active ||
          (
            visible && (
              ekg::rect_collide_vec2<float>(bar_horizontal, interact)
              ||
              ekg::rect_collide_vec2<float>(bar_vertical, interact)
            )
          )
        );
      }

      break;
    }

    case ekg::io::stage::process: {
      this->check_scroll();

      ekg::input_t &input {ekg::p_core->service_input.input};
      ekg::vec2_t<float> interact {input.interact};

      bool is_fire {ekg::fire("scrollbar-scroll")};

      this->states.is_scrolling.x = this->states.is_hovering && this->descriptor.axis.x && is_fire;
      this->states.is_scrolling.y = this->states.is_hovering && this->descriptor.axis.y && is_fire;

      break;
    }

    case ekg::io::stage::post: {
      break;
    }
  }
}

void ekg::ui::scrollbar::on_update() {

}

void ekg::ui::scrollbar::on_draw() {

}
