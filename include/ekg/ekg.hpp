/**
 * MIT License
 * 
 * Copyright (c) 2022-2024 Rina Wilk / vokegpu@gmail.com
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

#ifndef EKG_HPP
#define EKG_HPP

#pragma STDC FENV_ACCESS ON

#include "ekg/core/context.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/io/safety.hpp"

namespace ekg {
  constexpr std::string_view version_id {"2.0.0"};
  constexpr std::string_view version_state {"ALPHA"};

  /**
   * The runtime core of ekg.
   */
  extern ekg::runtime *p_core;

  /** The setup and handling functions of ekg **/

  /**
   * The initialization of EKG,
   * initialize `ekg::runtime_property_t` correct before calling this.
   * 
   * EKG does not has a font-service to handle system fonts, so it is required
   * to load a local (ttf, otf) font file.
   * 
   * GPU API tells which GPU API the application is built in; For OpenGL API
   * do not worry about the initialization setup process; Vulkan API requires
   * more of work to setup, EKG only initialize the necessary descriptor sets,
   * and PSO - you pre-initialize everything -.
   * Check: <link-about-RHI-concept-documentation-not-done>
   * 
   * The OS platform, under the system libraries (X11, Wayland, Win32) or
   * window library (SDL, GLFW, etc).
   * Check: <OS-platform-documention-aobut-not-done>  
   */
  void init(
    ekg::runtime *p_ekg_runtime,
    ekg::runtime_property_t *p_ekg_runtime_property
  );

  /**
   * Quit from all services and main runtime core.
   */
  void quit();

  /**
   * Process events, tasks, services & widgets.
   */
  void update();

  /**
   * Draw all data from gpu allocator.
   */
  void render();
}

#endif
