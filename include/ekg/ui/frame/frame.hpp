#ifndef EKG_UI_FRAME_HPP
#define EKG_UI_FRAME_HPP

#include "ekg/math/geometry.hpp"
#include "ekg/ui/properties.hpp"
#include "ekg/io/gpu.hpp"
#include "ekg/io/task.hpp"

namespace ekg {
  struct frame_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> border {};
    ekg::vec4_t<float> outline {};
    ekg::pixel_thickness_t actions_margin_pixel_thickness {};

    /**
     * [0] ekg::frame_t;
     **/
    ekg::layer_t<1> layers {};
  };

  struct frame_t {
  public:
    std::string tag {};
    ekg::rect_t<float> rect {};
    ekg::flags_t dock {};
    ekg::flags_t drag_dock {};
    ekg::flags_t resize_dock {};
    ekg::type type {ekg::type::frame};
    ekg::top_level_t top_level {};
    ekg::level level {ekg::level::bottom}; // wtfffffffffffffffff
    ekg::actions actions {};
    ekg::frame_theme_t theme {};
  public:
    ekg::properties_t *p_properties {};
  public:
    operator ekg::top_level_t() {
      return this->p_properties;
    }
  };
}

#endif
