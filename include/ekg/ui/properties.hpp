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

#ifndef EKG_UI_PROPERTIES_HPP
#define EKG_UI_PROPERTIES_HPP

#include "ekg/io/memory.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/io/gpu.hpp"
#include "ekg/ui/types.hpp"

#include <string>
#include <array>
#include <vector>

namespace ekg {
  struct properties_t {
  public:
    std::string tag {};
    ekg::flags_t dock {};
    ekg::type type {};
    ekg::id_t unique_id {};
    ekg::level level {};
    ekg::rect_t<float> rect {};

    void *p_descriptor {};
    void *p_widget {};
    void *p_stack {};

    ekg::properties_t *p_abs_parent {};
    ekg::properties_t *p_parent {};
    std::vector<ekg::properties_t*> children {};

    bool is_enabled {};
    bool is_alive {};
    bool is_visible {};
    bool is_parentable {};
    bool is_docknizable {};
    bool must_refresh_size {true};
  public:
    template<typename t>
    operator t&() {
      return *static_cast<t*>(this->p_descriptor);
    }

    template<typename t>
    operator t*() {
      return static_cast<t*>(this->p_widget);
    }
  };

  typedef ekg::properties_t* top_level_t;
}

#endif
