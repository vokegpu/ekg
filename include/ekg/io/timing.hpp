#ifndef EKG_IO_TIMING_HPP
#define EKG_IO_TIMING_HPP

#include <iostream>

namespace ekg {
  struct timing_t {
  public:
    /**
     * The 1 second counter in ms.
     **/
    static int64_t second;

    /**
     * The total running ticks since the application was started.
     **/
    static int64_t ticks;
  public:
    int64_t elapsed_ticks {};
    int64_t current_ticks {};
    int64_t ticks_going_on {};
  };

  bool reach(
    ekg::timing_t &timing,
    int64_t ms
  );

  bool reset_if_reach(
    ekg::timing_t &timing,
    int64_t ms
  );

  bool reset(
    ekg::timing_t &timing
  );

  bool extend(
    ekg::timing_t &timing,
    int64_t ms
  );

  int64_t interval(
    ekg::timing_t &timing
  );
}

#endif
