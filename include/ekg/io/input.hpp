#ifndef EKG_IO_INPUT_HPP
#define EKG_IO_INPUT_HPP

#include "ekg/io/log.hpp"
#include "ekg/math/geometry.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace ekg {
  enum class system_cursor_type {
    arrow,
    ibeam,
    wait,
    crosshair,
    wait_arrow,
    size_nwse,
    size_nesw,
    size_we,
    size_ns,
    size_all,
    no,
    hand
  };

  enum class special_key_type {
    left_shift,
    right_shift,
    left_ctrl,
    right_ctrl,
    left_alt,
    right_alt,
    tab,
    unknown
  };

  enum internal_behavior {
    no_auto_set_viewport_when_resize = 2 << 1
  };

  struct input_t {
  public:
    float scroll_speed {0.4f};
    ekg::timing_t ui_timeout_timing {};
    ekg::timing_t ui_scrolling_timing {};
    ekg::timing_t timing_last_interact {};
    ekg::timing_t ui_timing {};
    ekg::vec4_t<float> interact {};
    bool was_pressed {};
    bool was_released {};
    bool has_motion {};
    bool was_wheel {};
    bool was_typed {};
  };

  bool fire(std::string_view tag);
  bool input(std::string_view input);
  void bind(std::string_view tag, std::string_view input);
  void bind(std::string_view tag, std::vector<std::string_view> inputs);
  ekg::input_t &input();
}

namespace ekg::io {
  enum class stage {
    pre,
    process,
    post
  };

  enum class input_event_type {
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

  struct input_key_t {
  public:
    int32_t key {};
    int32_t scancode {};
  };

  struct serialized_input_event_t {
  public:
    ekg::io::input_event_type type {};
    std::string_view text_input {};
    uint8_t mouse_button {};

    int32_t mouse_motion_x {};
    int32_t mouse_motion_y {};

    int32_t mouse_wheel_x {};
    int32_t mouse_wheel_y {};

    float mouse_wheel_precise_x {};
    float mouse_wheel_precise_y {};

    ekg::io::input_key_t key {};

    float finger_x {};
    float finger_y {};

    float finger_dx {};
    float finger_dy {};
  };

  struct input_bind_t {
  public:
    std::vector<std::string> registry {};
    bool state {};
    bool *p_address {};
  };
}

#endif
