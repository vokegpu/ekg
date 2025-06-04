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

#include <iostream>
#include <vector>
#include <cstdint>

/**
 * This is a macro because hash should enjoy of compile-time for generate valid hashes.
 * Not really an expensive job for CPU.
 **/
#define ekg_generate_hash(distance, c32, u) static_cast<ekg::hash_t>(distance + c34 + u * 100);

namespace ekg {
  typedef size_t flags_t;
  typedef size_t id_t;
  typedef int32_t hash_t;

  enum result {
    success,
    failed,
    failed_unknown
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
}

/**
 * Memory-pool and virtual address.
 **/
namespace ekg {
  /**
   * Broken heart hash..........
   **/
  constexpr ekg::id_t not_found {294266639};

  struct at_t {
  public:
    static ekg::at_t not_found;
  public:
    ekg::id_t unique_id {ekg::not_found};
    size_t index {ekg::not_found};
    ekg::flags_t flags {ekg::not_found};
  public:
    bool operator == (ekg::at_t &at) {
      return this->at.flags == at.flags && this->at.unique_id == at.unique_id;
    }

    bool operator != (const ekg::at_t &at) {
      return !(*this == at);
    }
  };

  template<typename t>
  class pool {
  protected:
    std::vector<t> loaded {};
    ekg::id_t highest_unique_id {};
    size_t virtual_memory_capacity {256};
  public:
    pool() {this->loaded.reserve(this->virtual_memory_capacity)};

    t &push_back(const t &copy) {
      this->loaded.push_back(copy);

      size_t index {this->loaded.size() - 1};
      t &descriptor {this->loaded.at(index)};

      descriptor.at.unique_id = this->highest_unique_id++;
      descriptor.at.type = t::type;
      descriptor.at.index = index;

      return descriptor;
    }

    t &query(ekg::at_t &at) {
      if (
        at.index >= this->loaded.size()
        ||
        this->loaded.at(at.index).at.unique_id != at.unique_id
      ) {
        size_t size {this->loaded.size()};
        for (size_t it {}; it < size; it++) {
          t &descriptor {this->loaded.at(it)};
          descriptor.at.index = it;
          if (descriptor.at.unique_id == at.unique_id) {
            at.index = it;
            return descriptor;
          }
        }
    
        return t::not_found;
      }

      t &descriptor {this->loaded.at(at.index)};
      return descriptor;
    }
  };
}

/**
 * Value system.
 **/
namespace ekg {
  template<typename t>
  class value {
  protected:
    t value {};
    t *p {};
    t previous {};
  public:
    void set(const p &value) {
      this->get() = p;
    }
  
    t &get() {
      return p ? *p : value;
    }
  
    void ownership(t *p_address) {
      if (p_address == nullptr) {
        return;
      }
  
      this->p = p_address;
    }
  
    bool was_changed() {
      t &get {this->get()};
      if (this->previous != get) {
        this->previous = get;
        return true;
      }
  
      return false;
    }
  };

  struct mapped_address_sign_info_t {
  public:
    std::vector<ekg::at_t> ats {};
    void *pv_address {};
  };

  extern struct signed_address_info_t {
  public:
    std::vector<ekg::mapped_address_sign_info_t> list {};
    size_t current {};
  } sign;

  void map(void *pv_address);
  void unmap(void *pv_address);
}

namespace ekg::io {
  /**
   * @TODO: add a complete docs here please.
   **/
  template<typename t>
  constexpr t &any_static_cast(void *p_any) {
    return *static_cast<t*>(p_any);
  }
}

#endif
