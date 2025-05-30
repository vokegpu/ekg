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
#ifndef EKG_HANDLER_INPUT_HPP
#define EKG_HANDLER_INPUT_HPP

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

    ekg::io::input_key_t key {};

    float finger_x {};
    float finger_y {};

    float finger_dx {};
    float finger_dy {};
  };
}

#endif
