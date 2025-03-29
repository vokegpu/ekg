#ifndef EKG_UI_LABEL_WIDGET_HPP
#define EKG_UI_LABEL_WIDGET_HPP

#include "ekg/ui/abstract.hpp"
#include "ekg/ui/label/label.hpp"

namespace ekg::ui {
  class label : public ekg::ui::abstract {
  public:
    ekg::properties_t properties {};
    ekg::label_t descriptor {};
  public:
    ekg::rect_t<float> text_rect {};
  public:
    void on_reload() override;
    void on_draw() override;
  };
}

#endif
