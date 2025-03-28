#ifndef EKG_CORE_CONTEXT_HPP
#define EKG_CORE_CONTEXT_HPP

#include "ekg/io/typography.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/ui/types.hpp"

namespace ekg {
  extern FT_Library freetype_library;

  extern struct viewport_t {
    float x {};
    float y {};
    float w {};
    float h {};
    float dt {};
    float minimum_possible_size {10.0f};
    bool redraw {};
  } viewport;

  extern struct dpi_t {
    ekg::rect_t<float> scale {0.0f, 0.0f, 1920.0f, 1080.0f};
    float scale_interval {25.0f};

    float font_scale {18.0f};
    float factor_scale {};

    ekg::vec2_t<uint32_t> font_offset {4, 6};
    bool auto_scale {true};
  } dpi;

  extern struct current_t {
    ekg::id_t pressed {};
    ekg::type pressed_type {};
    ekg::id_t released {};
    ekg::type released_type {};
    ekg::id_t last {};
    ekg::id_t unique_id {};
    ekg::type type {};
  } current;

  extern struct tweaks_t {
    int64_t task_latency {500};
  } tweaks;

  constexpr uint32_t minimum_small_font_height {4};
  constexpr uint32_t minimum_font_height {8};
  constexpr uint32_t minimum_big_font_height {12};
}

#endif
