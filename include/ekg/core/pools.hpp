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
#ifndef EKG_CORE_POOLS_HPP
#define EKG_CORE_POOLS_HPP

#include "ekg/io/memory.hpp"
#include "ekg/handler/callback.hpp"
#include "ekg/gpu/sampler.hpp"

namespace ekg {
  extern struct pools_t {
  public:
    ekg::pool<ekg::callback_t> callback {ekg::callback_t::not_found};
    ekg::pool<ekg::sampler_t> sampler {ekg::sampler_t::not_found};
  } pools;

  template<typename t>
  t &make(
    t descriptor
  ) {
    switch (t::type) {
    case ekg::type::callback:
      return ekg::pools.callback.push_back(
        ekg::io::any_static_cast<ekg::callback_t>(&descriptor)
      );
    case ekg::type::sampler:
      return ekg::pools.sampler.push_back(
        ekg::io::any_static_cast<ekg::sampler_t>(&descriptor)
      );
    }

    return t::not_found;
  }

  template<typename t>
  t &query(
    ekg::at_t &at
  ) {
    switch (t::type) {
    case ekg::type::callback:
      return ekg::io::any_static_cast<ekg::callback_t>(
        &ekg::pools.callback.query(at)
      );
    case ekg::type::sampler:
      return ekg::io::any_static_cast<ekg::sampler_t>(
        &ekg::pools.sampler.query(at)
      );
    }

    return t::not_found;
  }
}

#endif
