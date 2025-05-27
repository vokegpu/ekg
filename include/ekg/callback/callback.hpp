#ifndef EKG_CALLBACK_HPP
#define EKG_CALLBACK_HPP

namespace ekg {
  struct info_t {
  public:
    std::string tag {};
    void *pv_data {nullptr};
  };

  using callback_function_t = void(*)(ekg::info_t&)

  struct callback_t {
  public:
    static const ekg::type type {/* type */};
    static descriptor_t not_found;
  public:
    ekg::id_t unique_id {};
    ekg::at_t other_descriptor_at {};
    ekg::at_t at {};
  public:
    ekg::info_t info {};
    std::function(void(ekg::info_t&)) lambda {};
    ekg::callback_function_t function {nullptr};
  };
}

#endif
