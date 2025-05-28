#ifndef EKG_PLATFORM_SDL_SDL2_HPP
#define EKG_PLATFORM_SDL_SDL2_HPP

#if defined(__ANDROID__)
  #include "SDL.h"
#else
  #include <SDL2/SDL.h>
#endif

#include "ekg/platform/platform.hpp"
#include <array>

namespace ekg {
  class sdl2 : public ekg::platform::base {
  protected:
    std::array<SDL_Cursor*, 12> loaded_system_cursors {};
    SDL_Window *p_sdl_win {};
  public:
    explicit sdl2(
      SDL_Window *p_sdl_win,
      ekg::flags_t modes = static_cast<ekg::flags_t>(0)
    );
  public:
    void init() override;
    void quit() override;
    void update_display_size() override;
    void update() override;
    void get_key_name(ekg::io::input_key_t &key, std::string &name) override;
    void get_special_key(ekg::io::input_key_t &key, ekg::special_key &special_key) override;
    const char *get_clipboard_text() override;
    void set_clipboard_text(const char *p_text) override;
    bool has_clipboard_text() override;
  };
}

#endif
