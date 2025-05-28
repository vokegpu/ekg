#ifndef EKG_HANDLER_INPUT_HPP
#define EKG_HANDLER_INPUT_HPP

#include "ekg/io/timing.hpp"
#include "ekg/math/geometry.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace ekg {
  enum class system_cursor {
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

  enum class special_key {
    left_shift,
    right_shift,
    left_ctrl,
    right_ctrl,
    left_alt,
    right_alt,
    tab,
    unknown
  };

  struct input_info_t {
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
}

namespace ekg::io {
  struct input_key_t {
  public:
    int32_t key {};
    int32_t scancode {};
  };

  struct input_bind_t {
  public:
    std::vector<strd::string> registry {};
    bool state {};
    bool *p_address {};
  };
}

#endif
