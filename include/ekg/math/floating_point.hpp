#ifndef EKG_MATH_FLOATING_POINT_HPP
#define EKG_MATH_FLOATING_POINT_HPP

#include <cfloat>
#include <cmath>

namespace ekg {
  constexpr float pi {3.1415927f};
  constexpr float pixel {1.0000000f};
  constexpr float pixel_div_2 {0.5000000f};

  constexpr bool fequalsf(float compare, float compared) {
    return (
      fabsf(compare - compared)
      <=
      (
        FLT_EPSILON
        *
        fmaxf(1.0f, fmaxf(fabsf(compare), fabsf(compared)))
      )
    );
  }
}

#endif
