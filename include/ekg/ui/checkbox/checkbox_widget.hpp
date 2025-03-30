#ifndef EKG_UI_CHECKBOX_WIDGET_HPP
#define EKG_UI_CHECKBOX_WIDGET_HPP

#include "ekg/ui/abstract.hpp"
#include "ekg/ui/checkbox/checkbox.hpp"

namespace ekg::ui {
  class checkbox : public ekg::ui::abstract {
  public:
    ekg::checkbox_t descriptor {};
  public:
    ekg::rect_t<float> text_rect {};
    ekg::rect_t<float> box_rect {};
  public:
    void on_reload() override;
    void on_event(ekg::io::stage stage) override;
    void on_draw() override;
  };
}

#endif 
