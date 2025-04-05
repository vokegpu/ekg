#ifndef EKG_UI_SCROLLBAR_WIDGET_HPP
#define EKG_UI_SCROLLBAR_WIDGET_HPP

#include "ekg/ui/abstract.hpp"
#include "scrollbar.hpp"

namespace ekg::ui {
  class scrollbar : public ekg::ui::abstract {
  public:
    ekg::scrollbar_t descriptor {};
  public:
    ekg::rect_t<float> scrollable_area {};
    ekg::rect_t<float> scissor {};
    ekg::rect_t<float> bar_horizontal {};
    ekg::rect_t<float> bar_vertical {};
    ekg::vec4_t<float> scroll {};
    ekg::vec2_t<float> delta {};
    ekg::vec2_t<float> acceleration {};

    ekg::ui::states_t bar_horizontal_states {};
    ekg::ui::states_t bar_vertical_states {};
  public:
    void clamp_scroll();
    void reset_scroll();
    void check_scroll();
    bool is_scrolling(bool state);
    float get_horizontal_scroll_normalized();
    float get_vertical_scroll_normalized();
  public:
    void on_create() override;
    void on_reload() override;
    void on_event(ekg::io::stage stage) override;
    void on_update() override;
    void on_draw() override;
  };
}

#endif
