#include "ekg/ui/label/label_widget.hpp"
#include "ekg/draw/font_renderer.hpp"
#include "ekg/draw/shape.hpp"
#include "ekg/core/runtime.hpp"

void ekg::ui::label::on_reload() {
  this->get_abs_rect();

  ekg::draw::font_renderer &text_fr {
    ekg::draw::get_font_renderer(this->descriptor.text_font_size)
  };

  this->text_rect.w = text_fr.get_text_width(static_cast<std::string>(this->descriptor.text));
  this->text_rect.h = text_fr.get_text_height();

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

  this->descriptor.rect.scaled_height = ekg::min_clamp<float>(this->descriptor.rect.scaled_height, ekg::pixel);
  
  if (this->properties.must_refresh_size) {
    this->descriptor.rect.w = ekg::min_clamp<float>(aligned_dimension.w, this->descriptor.rect.w);
    this->properties.must_refresh_size = false;
  }

  this->descriptor.rect.h = ekg::min_clamp<float>(aligned_dimension.h * this->descriptor.rect.scaled_height, this->descriptor.rect.h);
  
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

  mask.insert({&this->text_rect, this->descriptor.text_dock});
  mask.docknize();
}

void ekg::ui::label::on_draw() {
  EKG_ASSERT_VALUE(this->descriptor.text);

  ekg::rect_t<float> &rect {
    this->get_abs_rect()
  };

  EKG_ASSERT_SCISSOR(
    this->scissor,
    rect,
    this->p_parent_scissor_rect
  );

  ekg::draw::rect(
    rect,
    this->descriptor.theme.background,
    ekg::draw_mode::filled,
    this->descriptor.theme.layers[ekg::layer::background]
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
