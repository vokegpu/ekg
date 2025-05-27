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
