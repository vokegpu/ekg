#include "ekg/ui/abstract.hpp"
#include "ekg/ekg.hpp"

ekg::rect_t<float> &ekg::ui::abstract::get_abs_rect() {
  return (
    this->properties.rect = (
      this->rect + *this->p_parent_rect + *this->p_scroll_vec
    )
  );
}

void ekg::ui::abstract::action(
  bool must_trigger,
  ekg::action action
) {
  if (!must_trigger) {
    return;
  }

  ekg::task_t *p_task {
    this->properties.actions.at(static_cast<size_t>(action))
  };

  if (p_task == nullptr) {
    return;
  } 

  ekg::p_core->service_handler.dispatch(
    p_task
  );
}

void ekg::ui::abstract::on_create() {
  this->p_parent_rect = &this->_blank_parent_rect;
  this->p_parent_scissor_rect = &this->_blank_parent_scissor_rect;
  this->p_scroll_vec = &this->_blank_scroll_vec;
}

void ekg::ui::abstract::on_destroy() {

}

void ekg::ui::abstract::on_reload() {

}

void ekg::ui::abstract::on_event(ekg::io::stage stage) {
  ekg::input_t &input {
    ekg::p_core->service_input.input
  };

  switch (stage) {
    case ekg::io::stage::pre: {
      if (input.was_pressed || input.was_released || input.has_motion || input.was_wheel) {
        ekg::rect_t<float> &rect {this->get_abs_rect()};
        ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};

        this->states.is_hover = (
          ekg::rect_collide_vec2(rect, interact)
          &&
          (
            this->properties.level == ekg::level::top
            ||
            this->properties.p_parent == nullptr
            ||
            ekg::rect_collide_vec2(this->scissor, interact)
          )
        );
  }
      break;
    }

    case ekg::io::stage::post: {
      this->states.is_hover = false;

      #if defined(__ANDROID__)
      this->states.is_highlight = (
        !(
          !this->states.is_hover
          &&
          input.was_released
        )
        &&
        this->states.is_highlight
      )
      #endif

      break;
    }
  }
}

void ekg::ui::abstract::on_update() {
}

void ekg::ui::abstract::on_draw() {

}
