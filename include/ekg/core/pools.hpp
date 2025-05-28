#ifndef EKG_CORE_POOLS_HPP
#define EKG_CORE_POOLS_HPP

#include "ekg/io/memory.hpp"
#include "ekg/handler/callback.hpp"

namespace ekg {
  extern struct pools_t {
  public:
    ekg::pool<ekg::callback_t> callback {ekg::callback_t::not_found};
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
        &ekg::poos.callback.query(at)
      );
    }

    return t::not_found;
  }
}

#endif
