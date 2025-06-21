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
#ifndef EKG_UI_SLIDER_HPP
#define EKG_UI_SLIDER_HPP

#include "ekg/io/descriptor.hpp"
#include "ekg/math/geometry.hpp"

namespace ekg {
  struct slider_color_scheme_t {
  public:
    ekg::rgba_t<uint8_t> background {};
    ekg::rgba_t<uint8_t> outline {};
    ekg::rgba_t<uint8_t> bar_background {};
    ekg::rgba_t<uint8_t> bar_highlight {};
    ekg::rgba_t<uint8_t> bar_active {};
    ekg::rgba_t<uint8_t> bar_outline {};
    ekg::rgba_t<uint8_t> text_foreground {};
    ekg::pixel_thickness_t bar_thickness {25};
    ekg::pixel_thickness_t bar_target_thickness {25};
  };

  struct slider_t {
  public:
    struct range {
    protected:
      ekg::value<char[8]> memory_tape_value {};
      ekg::value<char[8]> memory_tape_minimum_value {};
      ekg::value<char[8]> memory_tape_maximum_value {};
    public:
      template<typename t>
      t &value() {
        return ekg::io::any_static_cast<t>(
          this->memory_tape_value.get()
        );
      }

      template<typename t>
      t &value(t *p_ownership_address) {
        this->memory_tape_value.ownership<t>(p_ownership_address);
        return this->value<t>();
      }

      template<typename t>
      t &min() {
        return ekg::io::any_static_cast<t>(
          this->memory_tape_minimum_value.get()
        );
      }

      template<typename t>
      t &min(t *p_ownership_address) {
        this->memory_tape_minimum_value.ownership<t>(p_ownership_address);
        return this->min<t>();
      }

      template<typename t>
      t &max() {
        return ekg::io::any_static_cast<t>(
          this->memory_tape_maximum_value.get()
        );
      }

      template<typename t>
      t &max(t *p_ownership_address) {
        this->memory_tape_maximum_value.ownership<t>(p_ownership_address);
        return this->max<t>();
      }
    };
  public:
    static constexpr ekg::type type {ekg::type::slider};
    static ekg::slider_t not_found;
  public:
    ekg::at_t property_at {};
  public:
    std::string tag {};
    ekg::rect_t<float> rect {};
    ekg::flags_t dock {};
    std::vector<ekg::slider_t::range> ranges {};
    ekg::slider_color_scheme_t color_scheme {};
  public:
    ekg_descriptor(ekg::slider_t);
  };
}

#endif
