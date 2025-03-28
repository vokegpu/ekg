#ifndef EKG_LAYOUT_EXTENTNIZE_HPP
#define EKG_LAYOUT_EXTENTNIZE_HPP

#include "ekg/ui/abstract.hpp"

namespace ekg::layout {
  struct extent_t {
  public:
    static ekg::layout::extent_t v_widget;
    static ekg::layout::extent_t h_widget;
    static ekg::layout::extent_t v_rect_descriptor;
    static ekg::layout::extent_t h_rect_descriptor;
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
  void extentnize_rect_descriptor(
    std::vector<ekg::rect_descriptor_t> &rect_descriptor_list,
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
    ekg::ui::abstract *p_widget,
    ekg::flags_t flag_ok,
    ekg::flags_t flag_stop,
    ekg::flags_t flag_axis,
    float &extent,
    int32_t &in_out_count
  );
}

#endif
