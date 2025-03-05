#ifndef EKG_IO_LOG_HPP
#define EKG_IO_LOG_HPP

#include <iostream>
#include <sstream>

#define EKG_LOG_DEBUG

namespace ekg {
  class log {
  public:
    static std::ostringstream buffer;
    static bool buffered;
    static bool tracked;
  public:
    static void flush() {
      if (ekg::log::buffered) {
        #ifdef __ANDROID__
        __android_log_print(ANDROID_LOG_VERBOSE, "EKG", "%s", ekg::log::buffer.str().c_str());
        #else
        std::cout << ekg::log::buffer.str();
        #endif

        ekg::log::buffer = std::ostringstream {};
        ekg::log::buffered = false;
      }
    }

    static void print(std::string_view msg) {
      std::cout << "[EKG-PRINT] " << msg << std::endl;
    }

    template<typename t>
    static void trace(bool should, t trace, bool interrupt_runtime = false) {
      if (!should) {
        return;
      }

      std::cout << "[EKG-TRACE] " << trace << std::endl;
      if (interrupt_runtime) {
        std::terminate();
      }
    }
  public:
    template<typename t>
    explicit log(t content) {
      std::cout << content << std::endl;
    }

    explicit log() {
      ekg::log::buffered = true;
      ekg::log::buffer << "[EKG-INFO] ";
    }

    ~log() {
      ekg::log::buffer << '\n';
      #ifdef EKG_LOG_DEBUG
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

  struct timing_t {
  public:
    /**
     * The 1 second counter in ms.
     */
    static int64_t second;

    /**
     * The total running ticks since the application was started.
     */
    static int64_t ticks;
  public:
    int64_t elapsed_ticks {};
    int64_t current_ticks {};
    int64_t ticks_going_on {};
  };

  bool reach(
    ekg::timing_t *p_timing,
    int64_t ms
  );

  bool reset(
    ekg::timing_t *p_timing
  );

  bool extend(
    ekg::timing_t *p_timing,
    int64_t ms
  );

  int64_t interval(
    ekg::timing_t *p_timing
  );
}

#endif
