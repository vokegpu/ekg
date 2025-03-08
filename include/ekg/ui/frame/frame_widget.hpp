#ifndef EKG_UI_FRAME_WIDGET_HPP
#define EKG_UI_FRAME_WIDGET_HPP

#include "frame.hpp"
#include "ekg/ui/abstract.hpp"

namespace ekg::ui {
  class frame : public ekg::ui::abstract {
  public:
    void on_create() override;
    void on_destroy() override;
    void on_reload() override;
    void on_event(ekg::io::stage stage) override;
    void on_update() override;
    void on_draw() override;
  };
}

#endif
