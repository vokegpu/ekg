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
#ifndef EKG_IO_LOG_HPP
#define EKG_IO_LOG_HPP

#include <iostream>
#include <cstdint>
#include <sstream>

//#define EKG_LOG_DEBUG
//#define EKG_INPUT_DEBUG

namespace ekg {
  class log {
  public:
    static std::ostringstream buffer;
    static bool buffered;
    static bool tracked;
  public:
    static void flush() {
      if (ekg::log::buffered) {
        #if defined(__ANDROID__)
          __android_log_print(ANDROID_LOG_VERBOSE, "EKG", "%s", ekg::log::buffer.str().c_str());
        #else
          std::cout << ekg::log::buffer.str();
        #endif

        ekg::log::buffer = std::ostringstream {};
        ekg::log::buffered = false;
      }
    }
  public:
    template<typename t>
    explicit log(t content) {
      std::cout << content << std::endl;
    }

    explicit log() {
      ekg::log::buffered = true;
      ekg::log::buffer << "[EKG] ";
    }

    ~log() {
      ekg::log::buffer << '\n';

      #if defined(EKG_LOG_DEBUG)
        ekg::log::flush();
        std::cout << std::endl;
      #endif
    }

    template<typename t>
    log &operator<<(const t &value) {
      ekg::log::buffer << value;
      return *this;
    }
  };
}

#endif
