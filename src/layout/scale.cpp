#include "ekg/core/runtime.hpp"
#include "ekg/core/context.hpp"
#include "ekg/layout/scale.hpp"

void ekg::layout::scale_calculate() {
  ekg::vec2_t<float> display_size {ekg::viewport.scale.w, ekg::viewport.scale.h};
  ekg::vec2_t<float> viewport {ekg::viewport.w, ekg::viewport.h};

  if (ekg::viewport.auto_scale) {
    ekg::p_core->p_os_platform->update_display_size();

    display_size.x = ekg::p_core->p_os_platform->display_size.w;
    display_size.y = ekg::p_core->p_os_platform->display_size.h;

    ekg::viewport.scale.w = 1920.0f;
    ekg::viewport.scale.h = 1080.0f;

    viewport = display_size;
  }

  /**
   * The scale is step-based, each step change the scale, e.g:
   * scale percent interval = 25
   * scale percent = 100 (scale GUI resolution == window size)
   *
   * scale percent in:
   * 0   == 0
   * 25  == 1
   * 50  == 2
   * 75  == 3
   * 100 == 4
   *
   * Then it is divided by 4 (4 is the maximum value)
   * e.g: 2/4 = 0.5f --> 3/4 = 0.75f
   **/

  float base_scale {
    ekg::viewport.scale.w * ekg::viewport.scale.h
  };

  float display_scale {
    display_size.x * display_size.y
  };

  float display_factor {
    display_scale / base_scale
  };

  float display_scale_percent {
    display_factor * 100.0f
  };
  
  float factor {
    (
      (viewport.x * viewport.y)
      /
      base_scale
    ) * 100.0f
  };

  factor = (
    roundf(factor / ekg::viewport.scale_interval)
    /
    (display_scale_percent / ekg::viewport.scale_interval)
  );

  ekg::viewport.factor_scale = ekg::clamp(factor, 0.5f, 2.0f);
}
