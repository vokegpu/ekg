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

#ifndef EKG_IO_MEMORY_HPP
#define EKG_IO_MEMORY_HPP

#define EKG_MEMORY_MUST_FREE_TASKS_AUTOMATICALLY true
#define EKG_MEMORY_ACTIONS_SIZE 7

#include <cstdint>
#include <iostream>

namespace ekg {
  typedef uint64_t id_t;
  typedef uint64_t flags_t;

  enum result {
    success           = 2 << 1,
    failed            = 2 << 2,
    could_not_find    = 2 << 3,
    not_implemented   = 2 << 4
  };

  template<typename t>
  constexpr bool has(ekg::flags_t bits, t bit) {
    return bits & bit;
  }

  template<typename t>
  constexpr bool strip(ekg::flags_t &bits, t bit) {
    bits = bits & ~(bit);
    return bits & bit;
  }

  template<typename t>
  constexpr ekg::flags_t &put(ekg::flags_t &bits, t bit) {
    return (bits |= bit);
  }

  template<typename t>
  class value {
  protected:
    t cache {};
    t *p_address {nullptr};
  public:
    bool was_changed {};
  public:
    value() = default;

    value(t *p_address) {
      this->p_address = p_address;
      this->was_changed = true;
    }

    value(t value) {
      this->p_address = nullptr;
      this->cache = value;
      this->was_changed = true;
    }

    value(const char *p_char) {
      this->get_value() = p_char;
      this->was_changed = true;
    }

    void move(t *p_address) {
      this->p_address = p_address;
      this->was_changed = true;
    }

    void set_value(t value) {
      this->get_value() = value;
      this->was_changed = true;
    }

    t &get_value() {
      return (
        this->p_address
        ?
        /**
         *
         * idk idc i want
         * todo: add useless comment soon
         *
         **/
        *this->p_address
        :
        this->cache
      );
    }
  public:
    operator t() {
      return this->get_value();
    }

    template<typename s>
    ekg::value<t> &operator = (s value) {
      this->get_value() = value;
      this->was_changed = true;
      return *this;
    }
  };
}

namespace ekg::io {
  constexpr uint64_t invalid_unique_id {static_cast<uint64_t>(0)};

  template<typename t>
  constexpr t &any_static_cast(void *p_any) {
    return *static_cast<t*>(p_any);
  }
}

#endif
