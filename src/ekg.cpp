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

#include "ekg/ekg.hpp"
#include "ekg/os/platform.hpp"

void ekg::init(
  ekg::runtime *p_ekg_runtime,
  ekg::runtime_property_t *p_ekg_runtime_property
) {
  ekg::log() << "Initializing FreeType library";

  if (FT_Init_FreeType(&ekg::freetype_library)) {
    ekg::log() << "Error: Failed to init FreeType library";
    return;
  }

  ekg::log() << "Initializing built-in OS platform-interface";

  p_ekg_runtime->p_os_platform = p_ekg_runtime_property->p_os_platform;
  p_ekg_runtime->p_os_platform->init();

  ekg::log() << "Initializing built-in GPU hardware-interface";

  p_ekg_runtime->p_gpu_api = p_ekg_runtime_property->p_gpu_api;
  p_ekg_runtime->p_gpu_api->init();
  p_ekg_runtime->p_gpu_api->log_vendor_details();

  ekg::log() << "Pre-Initializing EKG";

  p_ekg_runtime->draw_fr_small.init();
  p_ekg_runtime->draw_fr_small.set_font(p_ekg_runtime_property->font_path);
  p_ekg_runtime->draw_fr_small.set_font_emoji(p_ekg_runtime_property->font_path_emoji);

  p_ekg_runtime->draw_fr_normal.init();
  p_ekg_runtime->draw_fr_normal.set_font(p_ekg_runtime_property->font_path);
  p_ekg_runtime->draw_fr_normal.set_font_emoji(p_ekg_runtime_property->font_path_emoji);

  p_ekg_runtime->draw_fr_big.init();
  p_ekg_runtime->draw_fr_big.set_font(p_ekg_runtime_property->font_path);
  p_ekg_runtime->draw_fr_big.set_font_emoji(p_ekg_runtime_property->font_path_emoji);

  ekg::log() << "Initializing EKG";

  ekg::p_core = p_ekg_runtime;
  ekg::p_core->init();
}

void ekg::quit() {
  ekg::p_core->p_os_platform->quit();
  ekg::p_core->p_gpu_api->quit();
  ekg::p_core->quit();

  ekg::log() << "Shutdown complete - Thank you for using EKG ;) <3";
}

void ekg::update() {
  ekg::p_core->update();
  ekg::p_core->p_os_platform->update();
  ekg::p_core->p_os_platform->serialized_input_event.type = ekg::io::input_event_type::none;
}

void ekg::render() {
  ekg::p_core->render();
}
