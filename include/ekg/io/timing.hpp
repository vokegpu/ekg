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
