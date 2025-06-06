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
#include "ekg/handler/callback/handler.hpp"
#include "ekg/io/log.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/core/runtime.hpp"

void ekg::handler::callback::init() {
  ekg::log() << "Initialising callback-handler";

  ekg::callback_t &swap {this->load()};
  swap.info.tag = "swap";
  swap.function = &ekg::core::swap;

  ekg::callback_t &reload {this->load()};
  reload.info.tag = "reload";
  reload.function = &ekg::core::reload;

  ekg::callback_t &docknize {this->load()};
  docknize.info.tag = "docknize";
  docknize.function = &ekg::core::docknize;

  ekg::callback_t &scalenize {this->load()};
  scalenize.info.tag = "scalenize";
  scalenize.function = &ekg::core::scalenize;
}

ekg::callback_t &ekg::handler::callback::load() {
  ekg::callback_t &task {ekg::make<ekg::callback_t>({})};
  this->loaded.emplace_back() = task.at;
  return task;
}

void ekg::handler::callback::dispatch(ekg::at_t &at) {
  ekg::callback_t &callback {ekg::query<ekg::callback_t>(at)};
  if (
    callback != ekg::callback_t::not_found
    &&
    !ekg::has(callback.at.flags, ekg::handler::status::dispatched)
  ) {
    callback.at.flags |= ekg::handler::status::dispatched;
    this->queue.push(callback.at);
  }
}

void ekg::handler::callback::dispatch(uint64_t index) {
  ekg::at_t &at {this->loaded.at(index)};
  ekg::callback_t &callback {ekg::query<ekg::callback_t>(at)};

  if (
    callback != ekg::callback_t::not_found
    &&
    !ekg::has(callback.at.flags, ekg::handler::status::dispatched)
  ) {
    callback.at.flags |= ekg::handler::status::dispatched;
    this->queue.push(callback.at);
  }
}

void ekg::handler::callback::update() {
  while (!this->queue.empty()) {
    ekg::callback_t &callback {ekg::query<ekg::callback_t>(this->queue.front())};
    this->queue.pop();

    if (callback == ekg::callback_t::not_found) {
      continue;
    }

    if (callback.lambda) {
      callback.lambda(callback.info);
    }

    if (callback.function) {
      callback.function(callback.info);
    }
  }
}
