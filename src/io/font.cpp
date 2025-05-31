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

#include "ekg/io/font.hpp"
#include "ekg/core/runtime.hpp"

ekg::flags_t ekg::io::font(
  ekg::io::font_face_t &font_face
) {
  if (font_face.was_face_changed) {
    if (font_face.was_loaded) {
      FT_Done_Face(font_face.ft_face);
      font_face.was_loaded = false;
    }

    font_face.was_loaded = FT_New_Face(
      ekg::p_core->,
      font_face.path.data(),
      0,
      &font_face.ft_face
    );

    if (font_face.was_loaded) {
      ekg::log() << "Could not load font " << font_face.path;
      return ekg::result::failed;
    }

    ekg::log() << "Font '" << font_face.path << "' loaded!"; 

    font_face.was_loaded = true;
    font_face.was_face_changed = false;
  }

  if (font_face.was_loaded && font_face.was_size_changed) {
    FT_Set_Pixel_Sizes(font_face.ft_face, 0, font_face.size);
    font_face.was_size_changed = false;
  }

  return ekg::result::success;
}
