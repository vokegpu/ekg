#ifndef EKG_IO_EVENT_HPP
#define EKG_IO_EVENT_HPP

#include "ekg/handler/input.hpp"

namespace ekg::io {
  enum class event_type {
    none,
    text_input,
    mouse_button_up,
    mouse_button_down,
    mouse_motion,
    mouse_wheel,
    finger_up,
    finger_down,
    finger_motion,
    key_down,
    key_up
  };

  struct event_t {
  public:
    ekg::io::event_type type {};
    std::string_view text_input {};
    uint8_t mouse_button {};

    int32_t mouse_motion_x {};
    int32_t mouse_motion_y {};

    int32_t mouse_wheel_x {};
    int32_t mouse_wheel_y {};

    float mouse_wheel_precise_x {};
    float mouse_wheel_precise_y {};

    ekg::input_key_t key {};

    float finger_x {};
    float finger_y {};

    float finger_dx {};
    float finger_dy {};
  };
}

#endif
