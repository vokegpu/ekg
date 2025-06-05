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
#ifndef EKG_HANDLER_CALLBACK_HANDLER_HPP
#define EKG_HANDLER_CALLBACK_HANDLER_HPP

#include <queue>
#include <vector>

#include "ekg/io/memory.hpp"
#include "ekg/handler/callback.hpp"

namespace ekg::handler {
  enum status {
    not_dispateched = 2 << 1,
    dispatched = 2 << 2
  };

  class callback {
  protected:
    std::vector<ekg::at_t> loaded {};
    std::queue<ekg::at_t> queue {};
  public:
    void init();
    ekg::callback_t &load();
    void dispatch(size_t index);
    void dispatch(ekg::at_t &at);
    void update();
  };
}

#endif
