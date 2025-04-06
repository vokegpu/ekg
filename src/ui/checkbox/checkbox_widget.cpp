#include "ekg/ui/checkbox/checkbox_widget.hpp"
#include "ekg/draw/font_renderer.hpp"
#include "ekg/draw/shape.hpp"
#include "ekg/core/runtime.hpp"

void ekg::ui::checkbox::on_reload() {
  this->get_abs_rect();

  ekg::draw::font_renderer &text_fr {
    ekg::draw::get_font_renderer(this->descriptor.text_font_size)
  };

  this->text_rect.w = text_fr.get_text_width(static_cast<std::string>(this->descriptor.text));
  this->text_rect.h = text_fr.get_text_height();

  /* checkbox box square size is the same size as font renderer text height */

  this->box_rect.w = this->text_rect.h;
  this->box_rect.h = this->text_rect.h;

  ekg::axis pick_axis {
    ekg::axis::horizontal
  };

  ekg::aligned_t aligned_dimension {};
  ekg::align_rect_dimension(
    pick_axis,
    this->text_rect,
    ekg::viewport.minimum_possible_size,
    aligned_dimension
  );

  this->descriptor.rect.scaled_height = ekg::clamp_min<float>(this->descriptor.rect.scaled_height, ekg::pixel);
  
  if (this->properties.must_refresh_size) {
    this->descriptor.rect.w = ekg::clamp_min<float>(aligned_dimension.w, this->descriptor.rect.w);
    this->properties.must_refresh_size = false;
  }

  this->descriptor.rect.h = ekg::clamp_min<float>(aligned_dimension.h * this->descriptor.rect.scaled_height, this->descriptor.rect.h);
  
  ekg::layout::mask &mask {
    ekg::p_core->layout_mask 
  };

  mask.preset(
    {
      aligned_dimension.offset,
      aligned_dimension.offset,
      aligned_dimension.h
    },
    pick_axis,
    this->descriptor.rect.w
  );

  mask.insert({&this->box_rect, this->descriptor.box_dock});
  mask.insert({&this->text_rect, this->descriptor.text_dock});
  mask.docknize();
}

void ekg::ui::checkbox::on_event(ekg::io::stage stage) {
  switch (stage) {
    case ekg::io::stage::process: {
      ekg::input_t &input {ekg::p_core->service_input.input};
    
      if (
          input.has_motion
          ||
          input.was_pressed
          ||
          input.was_released
      ) {
        ekg::io::set<bool>(
          this->states.is_highlighting,
          this->states.is_hovering
        );

        ekg::io::set<bool>(
          this->states.is[ekg::checkbox_t::box],
          (
            this->states.is_hovering
            &&
            ekg::rect_collide_vec2<float>(
              (this->box_rect + this->get_abs_rect()),
              static_cast<ekg::vec2_t<float>>(input.interact)
            )
          )
        );

        ekg::io::trigger(
          (
            input.has_motion
            &&
            this->states.is_hovering
            &&
            (ekg::timing_t::second > ekg::tweaks.task_latency)
          ),
          ekg::action::motion,
          this->descriptor.actions
        );
      }
    
      if (
          input.was_pressed
          &&
          !this->states.is_active
          &&
          this->states.is_hovering
          &&
          ekg::fire("checkbox-active")
      ) {
        ekg::io::trigger(
          true,
          ekg::action::press,
          this->descriptor.actions
        );
    
        ekg::io::set<bool>(this->states.is_active, true);
      } else if (input.was_released && this->states.is_active) {
        ekg::io::trigger(
          this->states.is_hovering,
          ekg::action::release,
          this->descriptor.actions
        );

        ekg::io::trigger(
          (
            this->states.is_hovering
            &&
            (this->descriptor.value.set_value(!this->descriptor.value.get_value()) || true)
          ),
          ekg::action::active,
          this->descriptor.actions
        );

        ekg::io::set<bool>(
          this->states.is_active,
          false
        );
      }

      break;
    }

    default:
      ekg::ui::abstract::on_event(stage);
      break;
  }
}

void ekg::ui::checkbox::on_draw() {
  ekg::rect_t<float> &rect {this->get_abs_rect()};

  EKG_ASSERT_SCISSOR(
    this->scissor,
    rect,
    this->p_parent_scissor_rect
  );

  EKG_ASSERT_VALUE(this->descriptor.value);
  EKG_ASSERT_VALUE(this->descriptor.text);

  bool is_value_true {
    this->descriptor.value.get_value()
  };

  ekg::rect_t<float> box {this->box_rect + rect};

  ekg::draw::rect(
    rect,
    this->descriptor.theme.background,
    ekg::draw_mode::filled,
    this->descriptor.theme.layers[ekg::layer::background]
  );

  if (this->states.is_highlighting && !this->states.is[ekg::checkbox_t::box]) {
    ekg::draw::rect(
      rect,
      this->descriptor.theme.highlight,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::layer::highlight]
    );
  }

  if (this->states.is[ekg::checkbox_t::box]) {
    ekg::draw::rect(
      box,
      this->descriptor.theme.box_highlight,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::checkbox_t::box][ekg::layer::highlight]
    );
  }

  ekg::draw::rect(
    box,
    this->descriptor.theme.box_background,
    ekg::draw_mode::filled,
    this->descriptor.theme.layers[ekg::checkbox_t::box][ekg::layer::background]
  ); 

  if (this->states.is_active && this->states.is[ekg::checkbox_t::box]) {
    ekg::draw::rect(
      box,
      this->descriptor.theme.box_active,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::checkbox_t::box][ekg::layer::active]
    );
  } else if (this->states.is_active) {
    ekg::draw::rect(
      rect,
      this->descriptor.theme.active,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::layer::active]
    );
  }

  if (is_value_true) {
    ekg::draw::rect(
      box,
      this->descriptor.theme.box_active,
      ekg::draw_mode::filled,
      this->descriptor.theme.layers[ekg::checkbox_t::box][ekg::layer::active]
    );
  }

  ekg::draw::rect(
    box,
    this->descriptor.theme.box_outline,
    ekg::draw_mode::outline
  );

  ekg::draw::get_font_renderer(this->descriptor.text_font_size)
    .blit(
      static_cast<std::string>(this->descriptor.text),
      rect.x + this->text_rect.x,
      rect.y + this->text_rect.y,
      this->descriptor.theme.text
    );

  ekg::draw::rect(
    rect,
    this->descriptor.theme.outline,
    ekg::draw_mode::outline
  );
}
