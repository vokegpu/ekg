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

#include "ekg/ui/abstract.hpp"
#include "ekg/service/handler.hpp"
#include "ekg/service/theme.hpp"
#include "ekg/service/input.hpp"
#include "ekg/gpu/allocator.hpp"
#include "ekg/layout/docknize.hpp"
#include "ekg/draw/font_renderer.hpp"
#include "ekg/io/algorithm.hpp"

#include <memory>

namespace ekg {
  struct runtime_property_t {
  public:
    std::string font_path {};
    std::string font_path_emoji {};
    ekg::gpu::api *p_gpu_api {};
    ekg::os::platform *p_os_platform {};
  };

  /**
   * The main core of EKG.
   **/
  extern class runtime {
  private:
    /**
     * TODO: answer questions about this part specifically until second `:` statment.
     **/
    std::vector<std::unique_ptr<ekg::ui::abstract>> loaded_widget_list {};
    ekg::id_t global_id {};

    std::vector<ekg::ui::abstract*> context_widget_list {};
    std::vector<ekg::ui::abstract*> high_frequency_widget_list {};
    std::vector<ekg::ui::abstract*> reload_widget_list {};
    std::vector<ekg::ui::abstract*> layout_docknize_list {};

    ekg::ui::abstract *p_abs_activity_widget {};
    ekg::io::target_collector_t swap_target_collector {};

    ekg::properties_t *p_current_parent_properties {};
  public:
    ekg::service::handler service_handler {};
    ekg::service::theme service_theme {};
    ekg::service::input service_input {};

    ekg::os::platform *p_os_platform {};

    ekg::gpu::allocator gpu_allocator {};
    ekg::gpu::api *p_gpu_api {};

    ekg::draw::font_renderer draw_fr_small {};
    ekg::draw::font_renderer draw_fr_normal {};
    ekg::draw::font_renderer draw_fr_big {};

    ekg::layout::mask layout_mask {};
  public:
    ekg::ui::abstract *emplace_back_new_widget_safety(
      ekg::ui::abstract *p_widget
    );

    ekg::id_t generate_unique_id();

    void dispatch_widget_op(
      ekg::ui::abstract *p_widget,
      ekg::io::operation op
    );

    ekg::properties_t *get_current_parent_properties();
  public:
    void init();
    void quit();
    void update();
    void render();
    void poll_events();
  } *p_core;
}

#endif
