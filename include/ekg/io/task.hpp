#ifndef EKG_IO_TASK_HPP
#define EKG_IO_TASK_HPP

#include <functional>
#include <string_view>

#include "ekg/core/context.hpp"

namespace ekg {
  struct info_t {
  public:
    std::string_view tag {};
    void *p_properties {};
    void *p_data {};
  };

  struct task_t {
  public:
    ekg::info_t info {};
    std::function<void(ekg::info_t&)> function {};
    bool was_dispatched {};
    bool unsafe_is_heap_memory_type {};
  public:
    void *operator new (size_t mem_alloc_size) {
      ekg::task_t *p_instance {
        static_cast<ekg::task_t*>(
          ::operator new(mem_alloc_size)
        )
      };

      p_instance->unsafe_is_heap_memory_type = true;
      return p_instance;
    }
  };

  enum class action {
    motion,
    press,
    release,
    active,
    focus,
    drag,
    resize,
    hover
  };
}

namespace ekg::io {
  enum operation {
    swap,
    reload,
    layout_docknize,
    scale_update,
    high_frequency
  };

  void dispatch(
    ekg::io::operation op
  );

  template<typename t>
  constexpr t &set(t &must_set, t value) {
    if (must_set == value) {
      return must_set;
    }

    ekg::viewport.redraw = true;
    return (
      must_set = value
    );
  }
}

#endif
