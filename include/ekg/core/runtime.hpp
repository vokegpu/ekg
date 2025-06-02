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
#ifndef EKG_CORE_RUNTIME_HPP
#define EKG_CORE_RUNTIME_HPP

#include "ekg/platform/base.hpp"
#include "ekg/gpu/api.hpp"
#include "ekg/draw/typography/font.hpp"
#include "ekg/draw/allocator.hpp"
#include "ekg/handler/theme/handler.hpp"
#include "ekg/handler/callback/handler.hpp"
#include "ekg/handler/input/handler.hpp"

namespace ekg {
  struct ekg_runtime_properties_info_t {
  public:
    std::string_view font_path_text {};
    std::string_view font_path_emoji {};
    ekg::platform::base *p_platform_base {};
    ekg::gpu::api *p_gpu_api {};
    ekg::ft_library ft_library {};
  };

  extern struct runtime_t {
  public:
    ekg::platform::base *p_platform_base {};
    ekg::gpu::api *p_gpu_api {};
    ekg::ft_library ft_library {};
  public:
    ekg::handler::input handler_input {};
    ekg::handler::callback handler_callback {};
    ekg::handler::theme handler_theme {};
  public:
    ekg::draw::allocator draw_allocator {};
    ekg::draw::font draw_font_small {};
    ekg::draw::font draw_font_medium {};
    ekg::draw::font draw_font_big {};
  public:
    ekg::at_t swap_target_at {ekg::at_t::not_found};
    std::vector<ekg::at_t> collector {};
    std::vector<ekg::at_t> register {};
    std::vector<ekg::at_t> stack {};
    std::vector<ekg::at_t> top_level_stack {};    
    std::vector<ekg::at_t> reload {};
    std::vector<ekg::at_t> docknize {};
    std::vector<ekg::at_t> high_frequency {};
  } *p_core;
}

namespace ekg::core {
  void swap_collector(bool &was_found, ekg::at_t &property_at);
  void swap(ekg::info_t &info);

  void reload(ekg::info_t &info);
  void docknize(ekg::info_t &info);
  void scale(ekg::info_t &info);
  void high_frequency(ekg::info_t &info);
  void poll_events();
}

#endif
