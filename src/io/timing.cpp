#include "ekg/io/timing.hpp"

int64_t ekg::timing_t::second {};
int64_t ekg::timing_t::ticks {};

bool ekg::reach(ekg::timing_t &timing, int64_t ms) {
  timing.ticks_going_on = ekg::timing_t::ticks;
  timing.current_ticks = timing.ticks_going_on - timing.elapsed_ticks;
  return timing.current_ticks > ms;
}

bool ekg::reset(ekg::timing_t &timing) {
  timing.elapsed_ticks = timing.ticks_going_on;
  return true;
}

bool ekg::reset_if_reach(ekg::timing_t &timing, int64_t ms) {
  timing.ticks_going_on = ekg::timing_t::ticks;
  timing.current_ticks = timing.ticks_going_on - timing.elapsed_ticks;
  
  if (timing.current_ticks > ms) {
    timing.elapsed_ticks = timing.ticks_going_on;
    return true;
  }

  return false;
}

bool ekg::extend(ekg::timing_t &timing, int64_t ms) {
  timing.elapsed_ticks = timing.ticks_going_on - ms;
  return true;
}

int64_t ekg::interval(ekg::timing_t &timing) {
  timing.ticks_going_on = ekg::timing_t::ticks;
  return (
    timing.current_ticks = timing.ticks_going_on - timing.elapsed_ticks
  );
}
