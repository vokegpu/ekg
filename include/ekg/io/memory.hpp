#ifndef EKG_IO_MEMORY_HPP
#define EKG_IO_MEMORY_HPP

#include <iostream>
#include <vector>
#include <cstdint>

namespace ekg {
  typedef size_t flags_t;
  typedef size_t id_t;

  constexpr ekg::id_t not_found {29496526662939};

  enum result {
    success,
    failed,
    failed_unknown
  };

  struct at_t {
  public:
    static ekg::at_t not_found;
  public:
    ekg::flags_t type {};
    ekg::id_t unique_id {};
    size_t index {};
    ekg::flags_t flags {};
  public:
    bool operator == (const ekg::at_t &at) {
      ekg::at_t::not_found.unique_id = ekg::not_found;
      return this->at.unique_id == at.unique_id;
    }

    bool operator != (const ekg::at_t &at) {
      return !(*this == at);
    }
  };

  template<typename t>
  class pool {
  protected:
    std::vector<t> loaded {};
    t not_found {};
    ekg::id_t highest_unique_id {};
    ekg::flags_t type {};
  public:
    pool(ekg::flags_t type, const t &not_found)
      : type(type), not_found(not_found) {}

    t &push_back(const t &copy) {
      this->loaded.push_back(copy);

      size_t index {this->loaded.size() - 1};
      t &ref {this->loaded.at(index)};

      ref.at.unique_id = this->highest_unique_id++;
      ref.at.type = this->type;
      ref.at.index = index;

      return ref;
    }

    t &query(ekg::at_t &at) {
      if (
        at.index >= this->loaded.size()
        ||
        this->loaded.at(at.index).at.unique_id != at.unique_id
      ) {
        size_t size {this->loaded.size()};
        for (size_t it {}; it < size; it++) {
          t &element {this->loaded.at(it)};
          element.at.index = it;
          if (element.at.unique_id == at.unique_id) {
            at.index = it;
            return element;
          }
        }
    
        return this->not_found;
      }
    
      return this->loaded.at(at.index);
    }
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
