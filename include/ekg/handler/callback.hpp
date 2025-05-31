#ifndef EKG_HANDLER_CALLBACK_HPP
#define EKG_HANDLER_CALLBACK_HPP

#include "ekg/io/memory.hpp"
#include "ekg/io/descriptor.hpp"
#include <string>

namespace ekg {
  struct info_t {
  public:
    std::string tag {};
    void *pv_data {nullptr};
  };

  using callback_function_t = void(*)(ekg::info_t&)

  struct callback_t {
  public:
    static constexpr ekg::type type {ekg::type::callback};
    static ekg::callback_t not_found;
  public:
    ekg::info_t info {};
    std::function(void(ekg::info_t&)) lambda {};
    ekg::callback_function_t function {nullptr};
  public:
    ekg_descriptor(ekg::callback_t);
  };
}

#endif
