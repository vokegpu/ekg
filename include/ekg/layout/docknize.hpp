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
#ifndef EKG_LAYOUR_DOCKNIZE_HPP
#define EKG_LAYOUR_DOCKNIZE_HPP

#include "ekg/ui/property.hpp"

namespace ekg::layout {
  /**
   * Returns the dimensional extent based on count and the offset (space between rects). 
   * 
   * The pixel imperfect issue was solved here...
   * For a long time I did not know what was going on with the pixels,
   * some solutions I used did not work, then I discovered that all the time
   * was this dimension extent with float imprecision loss.
   * 
   * Each pixels represent 1.0f, if the GPU receives pixels with
   * (n + f) `n` a non-floating point number and `f` a floating point;
   * the rasterizer will jump between pixels, resulting in pixel-imperfect.
   * 
   * The following formula make you understand:
   * ( (g - d) - (c * o) ) / c
   * 
   * g = group rect
   * d = dimensional extent
   * c = amount of widgets with fill property flag until any flag next
   * o = UI offset setting
   * 
   * Float-only without the int32_t cast may results in pixel-imperfect
   * due the influence of dimensional size of parent rect, font height, font width etc.
   * 
   * - Rina.
   **/
  constexpr float transform_dimension_from_extent(
    float dimension,
    float extent,
    float offset,
    int32_t count
  ) {
    return static_cast<float>(
      (
        static_cast<int32_t>(dimension)
        -
        static_cast<int32_t>(extent)
        -
        static_cast<int32_t>(count * offset)
      )
      /
      count
    );
  }

  /**
   * Pixel imperfection is a problem for UI widget placements, this macro provides
   * correction position for right/bottom based on left/top position.
   * 
   * It is important to understand that is impossible to remove all the pixel
   * imperfections, but there are ways to round it, as example, you can place widget
   * from a side (left or right, top or bottom) and align with an offset. That is how EKG fix it.
   * 
   * This method calculate the minimum possible position based on left until the pixel
   * escape for two or more offsets distance. Instead you use the container width directly
   * to calculate right widgets positions, EKG must use the left consistency to get the
   * real container (width/height) from the side of left (when using the container width directly
   * the position is pixel imperfect).
   * 
   * - Rina.
   **/
  constexpr float transform_to_pixel_perfect_position(
    float side_a,
    float side_b,
    float container_dimension,
    float offset
  ) {
    return ekg::clamp_min(
      (
        (side_a + (container_dimension - side_a) + offset)
        -
        (side_b)
      ),
      side_a
    );
  }

  class mask {
  public:
    struct item_t {
    public:
      ekg::rect_t<float> *p_rect {};
      ekg::flags_t dock {};
    };
  protected:
    std::vector<ekg::layout::mask::component_t> items {};
    float respective_all {};
    float respective_center {};
    ekg::flags_t axis {};
    ekg::vec3_t<float> offset {};
    ekg::rect_t<float> mask {};
  public:
    void preset(
      ekg::vec3_t<float> offset,
      ekg::flags_t axis,
      float initial_respective_size = 0.0f
    );
    
    void insert(
      const ekg::layout::mask::component_t &rect_descriptor
    );

    void docknize();
    ekg::rect_t<float> &get_rect();
  };

  /**
   * A mid-functional feature to process dock position from widgets.
   * Note: Recursive.
   **/
  void docknize_widget(
    ekg::property_t &property
  );
}

#endif
