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

#ifndef EKG_UI_ABSTRACT_HPP
#define EKG_UI_ABSTRACT_HPP

#include "ekg/ui/properties.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/io/input.hpp"

#define EKG_ASSERT_VALUE(value) if (value.was_changed && !(value.was_changed = false)) ekg::p_core->dispatch_widget_op(this, ekg::io::operation::reload);

namespace ekg::ui {
  class abstract {
  public:
    ekg::rect_t<float> _blank_parent_rect {};
    ekg::vec4_t<float> _blank_scroll_vec {};
    ekg::rect_t<float> _blank_descriptor_rect {};
  public:
    ekg::properties_t properties {};
    ekg::ui::states_t states {};

    ekg::rect_t<float> scissor {};
    ekg::vec2_t<float> min_size {};

    ekg::rect_t<float> *p_descriptor_rect {};
    ekg::vec4_t<float> *p_scroll_vec {};
    ekg::rect_t<float> *p_parent_rect {};
    ekg::rect_t<float> *p_parent_scissor_rect {};
  public:
    ekg::rect_t<float> &get_abs_rect();

    void action(
      bool must_trigger,
      ekg::action action
    );
  public:
    virtual void on_create();
    virtual void on_destroy();
    virtual void on_reload();
    virtual void on_event(ekg::io::stage stage);
    virtual void on_update();
    virtual void on_draw();
  public:
    template<typename t>
    operator t() {
      return *static_cast<t*>(this->properties.p_descriptor);
    }
  };
}

#endif
