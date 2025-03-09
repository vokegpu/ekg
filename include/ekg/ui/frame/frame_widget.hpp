#ifndef EKG_UI_FRAME_WIDGET_HPP
#define EKG_UI_FRAME_WIDGET_HPP

#include "frame.hpp"
#include "ekg/ui/abstract.hpp"

namespace ekg::ui {
  class frame : public ekg::ui::abstract {
  public:
    ekg::frame_t descriptor {};
  public:
    ekg::flags_t target_dock_drag {};
    ekg::flags_t target_dock_resize {};

    ekg::docker_t<float> docker_drag {};
    ekg::docker_t<float> docker_resize {};

    ekg::rect_t<float> rect_delta {};
    ekg::rect_t<float> rect_cache {};
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
