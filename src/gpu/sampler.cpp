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
#include "ekg/gpu/sampler.hpp"

ekg::sampler_t ekg::sampler_t::not_found {
  .at = ekg::at_t::not_found
};

ekg::flags_t ekg::sampler_src_r8_to_r8g8b8a8(
  const ekg::vec2_t<int32_t> &size,
  const unsigned char *p_src,
  std::vector<unsigned char> &dst
) {
  if (
    p_src == nullptr
    ||
    size.x == 0
    ||
    size.y == 0
    ||
    dst.size() != (size.x * size.y)
  ) {
    return ekg::result::failed;
  }

  size_t index {};

  for (size_t it {}; it < size.x * size.y; it++) {
    const unsigned char &char8_red_color {
      p_src[it]
    };

    index = it * 4;

    if (index == dst.size()) {
      break;
    }

    /**
     * may I be wrong? but the format is ARGB and not RGBA,
     * I do not know.
     **/

    dst.at(index + 0) = char8_red_color;
    dst.at(index + 1) = 255;
    dst.at(index + 2) = 255;
    dst.at(index + 3) = 255;
  } 

  return ekg::result::success;
}
