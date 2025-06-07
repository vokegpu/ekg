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
#ifndef EKG_LAYOUT_EXTENTNIZE_HPP
#define EKG_LAYOUT_EXTENTNIZE_HPP

#include "docknize.hpp"

namespace ekg::layout {
  struct extent_t {
  public:
    static ekg::layout::extent_t v_widget;
    static ekg::layout::extent_t h_widget;
    static ekg::layout::extent_t v_mask;
    static ekg::layout::extent_t h_mask;
  public:
    int32_t end_index {};
    int32_t begin_index {};
    int32_t count {};
    float extent {};
  };

  /**
   * The extentize is a method to calculate split (between all elements with `ekg::dock::fill` flag)
   * data from a container dimension, extent is the amount of non `ekg::dock::fill` calculated dimension.
   * 
   * Both functions `extentnize_*` works almost equals, but with
   * difference of specialization-level, rect descriptors are a
   * compact way to calculate extent, while widgets works with some additional
   * variables. For performance reasons EKG does not want to make generic
   * this way, but soon maybe we can change it.
   *
   * ---
   *  
   * How it works: 
   * 
   * The last index does not check if contains a next flag,
   * so it is needed to brute-check to stop at end of index. 
   *
   * The extent data store the previous bounding indices,
   * in simply words, prevent useless iteration.
   *
   * The min offset is added for extent, because we need count
   * the offset position when split the dimension width, but the
   * last extent space is not necessary, so we need to subtract.
   **/

  /**
   * Obtain the remain extent size, from the latest rect descriptor index `in`
   * and return the new count `out`.
   **/
  void extentnize_mask(
    std::vector<ekg::layout::mask::component_t> &components,
    ekg::vec3_t<float> offset,
    ekg::flags_t flag_ok,
    ekg::flags_t flag_stop,
    ekg::flags_t flag_axis,
    float &extent,
    int32_t &in_out_count
  );

  /**
   * Obtain the remain extent size, from the latest widget index `in`
   * and return the new count `out`.
   **/
  void extentnize_widget(
    ekg::property_t &property,
    ekg::flags_t flag_ok,
    ekg::flags_t flag_stop,
    ekg::flags_t flag_axis,
    float &extent,
    int32_t &in_out_count
  );
}

#endif
