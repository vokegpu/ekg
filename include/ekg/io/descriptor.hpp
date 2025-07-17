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
#ifndef EKG_IO_DESCRIPTOR_HPP
#define EKG_IO_DESCRIPTOR_HPP

#include "ekg/io/memory.hpp"

namespace ekg {
  enum type : ekg::flags_t {
    unknown = 0,
    callback = 1,
    property = 2,
    sampler = 3,
    stack = 4,
    button = 5,
    scrollbar = 6,
    frame = 7,
    slider = 8,
    label = 9,
    popup = 10,
  };
}

#define ekg_descriptor(descriptor_t) \
  public: \
    ekg::at_t at { \
      .unique_id = ekg::not_found, \
      .index = ekg::not_found, \
      .flags = ekg::not_found \
    }; \
    bool is_dead {}; \
  public: \
    bool operator == (descriptor_t &descriptor) { \
      return ( \
        (this->is_dead && descriptor.at == descriptor_t::not_found.at) \
        || \
        (!this->is_dead && this->at == descriptor.at) \
      ); \
    } \
\
    bool operator != (descriptor_t &descriptor) { \
      return !(*this == descriptor); \
    } \
\
    operator ekg::at_t() { \
      return this->at; \
    }

#endif
