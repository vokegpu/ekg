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
