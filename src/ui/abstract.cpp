#include "ekg/ui/abstract.hpp"

ekg::rect_t<float> &ekg::ui::abstract::get_abs_rect() {
  return (
    this->properties.rect = (
      this->rect + *this->p_parent_rect + *this->p_scroll_vec
    )
  );
}

void ekg::ui::abstract::on_create() {

}

void ekg::ui::abstract::on_destroy() {

}

void ekg::ui::abstract::on_reload() {

}

void ekg::ui::abstract::on_event(ekg::io::stage stage) {
  
}

void ekg::ui::abstract::on_update() {
}

void ekg::ui::abstract::on_draw() {

}
