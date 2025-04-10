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

#include "ekg/io/task.hpp"
#include "ekg/core/runtime.hpp"

void ekg::io::dispatch(
  ekg::io::operation op
) {
  switch (op) {
  case ekg::io::operation::high_frequency:
    break;
  default:
    ekg::p_core->service_handler.dispatch_pre_allocated_task(
      static_cast<uint64_t>(op)
    );
    break;
  }
}

void ekg::io::trigger(
  bool must_trigger,
  ekg::action action,
  ekg::actions actions,
  ekg::properties_t *p_properties
) {
  ekg::task_t *&p_task {
    actions[action]
  };

  if (
    p_task == nullptr
    ||
    p_properties == nullptr
    ||
    !must_trigger
  ) {
    return;
  }

  p_task->info.p_properties = p_properties;
  p_task->function(p_task->info);
}
