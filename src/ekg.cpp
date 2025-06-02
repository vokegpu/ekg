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
#include "ekg/ekg.hpp"
#include "ekg/io/log.hpp"
#include "ekg/core/context.hpp"

ekg::runtime_t ekg::p_core {nullptr};
ekg::context_t ekg::context {};
ekg::gui_t ekg::gui {};

std::ostringstream ekg::log::buffer {};
bool ekg::log::buffered {};

ekg::flags_t ekg::init(
  ekg::ekg_runtime_properties_info_t &runtime_properties_info,
  ekg::runtime_t *p_runtime
) {
  if (p_runtime == nullptr) {
    ekg::log("[EKG] ~ERROR~ invalid (?) `ekg::runtime_t` pointer address: nullptr");
    return ekg::result::failed;
  }

  ekg::log() << "[EKG] Initializing...";

  ekg::p_core = p_runtime;

  ekg::p_core->p_platform_base = runtime_properties_info.p_platform_base;
  ekg::p_core->p_gpu_api = runtime_properties_info.p_gpu_api;
  ekg::p_core->ft_library = runtime_properties_info.ft_library;

  ekg::p_core->handler_callback.init();
  ekg::p_core->handler_input.init();
  ekg::p_core->draw_allocator.init();

  /* deprecated soon */

  ekg::p_core->draw_font_small.init();
  ekg::p_core->draw_font_small.set_font(runtime_properties_info.font_path_text);
  ekg::p_core->draw_font_small.set_font_emoji(runtime_properties_info.font_path_emoji);
  ekg::p_core->draw_font_small.get_atlas_texture_sampler().gl_protected_active_index = true;;

  ekg::p_core->draw_font_medium.init();
  ekg::p_core->draw_font_medium.set_font(runtime_properties_info.font_path_text);
  ekg::p_core->draw_font_medium.set_font_emoji(runtime_properties_info.font_path_emoji);
  ekg::p_core->draw_font_medium.get_atlas_texture_sampler().gl_protected_active_index = true;;

  ekg::p_core->draw_font_big.init();
  ekg::p_core->draw_font_big.set_font(runtime_properties_info.font_path_text);
  ekg::p_core->draw_font_big.set_font_emoji(runtime_properties_info.font_path_emoji);
  ekg::p_core->draw_font_big.get_atlas_texture_sampler().gl_protected_active_index = true;;

  ekg::log() << "[EKG] Successfully initialized";

  return ekg::result::success;
}

void ekg::quit() {
  if (ekg::p_core == nullptr) {
    return;
  }
}

void ekg::update() {
  if (ekg::p_core == nullptr) {
    return;
  }
}

void ekg::render() {
  if (ekg::p_core == nullptr) {
    return;
  }
}
