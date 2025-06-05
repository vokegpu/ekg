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
#ifndef EKG_HANDLER_CALLBACK_HPP
#define EKG_HANDLER_CALLBACK_HPP

#include "ekg/io/memory.hpp"
#include "ekg/io/descriptor.hpp"
#include <string>
#include <functional>

namespace ekg {
  struct info_t {
  public:
    std::string tag {};
    void *pv_data {nullptr};
  };

  using callback_function_t = void(*)(ekg::info_t&);

  struct callback_t {
  public:
    static constexpr ekg::type type {ekg::type::callback};
    static ekg::callback_t not_found;
  public:
    ekg::info_t info {};
    std::function<void(ekg::info_t&)> lambda {};
    ekg::callback_function_t function {nullptr};
  public:
    ekg_descriptor(ekg::callback_t);
  };
}

namespace ekg::io {
  enum class operation {
    swap,
    reload,
    docknize,
    scalenize,
    high_frequency
  };

  void dispatch(
    ekg::io::operation op,
    ekg::at_t property_at = ekg::at_t::not_found
  );
}

#endif
