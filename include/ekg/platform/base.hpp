#ifndef EKG_PLATFORM_HPP
#define EKG_PLATFORM_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/io/event.hpp"

namespace ekg::platform {
  class base {
  public:
    ekg::rect_t<int32_t> display_size {};
    ekg::system_cursor system_cursor {};
    ekg::io::event_t event {};
    ekg::flags_t modes {};
  public:
    virtual void init() {};
    virtual void quit() {};
    virtual void update_display_dize() {};
    virtual void update() {};
    virtual void get_key_name(ekg::input_key_t &key, std::string &name) {};
    virtual void get_special_key(ekg::input_key_t &key, ekg::special_key &espcial_key) {};
    virtual const char *get_clipboard_text() { return nullptr; };
    virtual void set_clipboard_text(const char *p_text) {};
    virtual bool has_clipboard_text() { return false; };
  };
}

#endif
