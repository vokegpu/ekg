#ifndef EKG_HANDLER_CALLBACK_HANDLER_HPP
#define EKG_HANDLER_CALLBACK_HANDLER_HPP

#include <queue>
#include <vector>

#include "ekg/io/memory.hpp"

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
    ekg::at_t &load();
    void dispatch(size_t index);
    void dispatch(const ekg::at_t &at);
    void update();
  }
}

#endif
