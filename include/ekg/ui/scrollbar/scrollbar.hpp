#ifndef EKG_UI_SCROLLBAR_HPP
#define EKG_UI_SCROLLBAR_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/ui/properties.hpp"
#include "ekg/io/gpu.hpp"
#include "ekg/io/task.hpp"

namespace ekg {
  struct scrollbar_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> highlight {};
    ekg::vec4_t<float> active {};
    ekg::pixel_thickness_t pixel_thickness {};
    float min_bar_size {};

    /**
     * [0] ekg::scrollbar_t::horizontal;
     * [1] ekg::scrollbar_t::vertical;
     **/
    ekg::layer_t<2> layers {};
  };

  struct scrollbar_t {
  public:
    constexpr static ekg::flags_t horizontal {0};
    constexpr static ekg::flags_t vertical {1};
  public:
    std::string tag {};
    std::vector<ekg::properties_t*> *p_binded_children {};
    ekg::rect_t<float> *p_binded_rect {};
    ekg::type type {ekg::type::scrollbar};
    ekg::actions actions {};
    ekg::scrollbar_theme_t theme {};
  };
}

#endif
