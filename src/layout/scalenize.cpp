/**
 * MIT License
 * 
 * Copyright (c) 2022-2025 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "ekg/core/runtime.hpp"
#include "ekg/core/context.hpp"
#include "ekg/layout/scalenize.hpp"

void ekg::layout::scale_calculate() {
  ekg::vec2_t<float> display_size {ekg::context::dpi_scale.w, ekg::context::dpi_scale.h};
  ekg::vec2_t<float> viewport {ekg::context.viewport.w, ekg::context.viewport.h};

  if (ekg::context::dpi_auto_scale) {
    ekg::p_core->p_os_platform->update_display_size();

    display_size.x = ekg::p_core->p_os_platform->display_size.w;
    display_size.y = ekg::p_core->p_os_platform->display_size.h;

    ekg::context::dpi_scale.w = 1920.0f;
    ekg::context::dpi_scale.h = 1080.0f;

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
    ekg::context::dpi_scale.w * ekg::context::dpi_scale.h
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
    roundf(factor / ekg::context::dpi_scale_interval)
    /
    (display_scale_percent / ekg::context::dpi_scale_interval)
  );

  ekg::context::dpi_factor_scale = ekg::clamp(factor, 0.5f, 2.0f);
}
