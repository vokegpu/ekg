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
