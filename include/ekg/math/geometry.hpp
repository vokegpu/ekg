#ifndef EKG_MATH_GEOMETRY_HPP
#define EKG_MATH_GEOMETRY_HPP

#include "ekg/io/memory.hpp"
#include <cstdint>
#include <string>

namespace ekg {
  enum dock {
    none   = 2 << 1,
    free   = 2 << 2,
    top    = 2 << 3,
    bottom = 2 << 4,
    right  = 2 << 5,
    left   = 2 << 6,
    center = 2 << 7,
    full   = 2 << 8,
    next   = 2 << 9,
    fill   = 2 << 10,
    resize = 2 << 11,
    concat = 2 << 12
  };

  enum axis {
    vertical   = 2 << 13,
    horizontal = 2 << 14
  };

  template<typename t>
  struct vec2_t {
    union {
      struct {
        t x {};
        t y {};
      };
    };
  public:
    inline vec2_t() = default;

    inline vec2_t(t _x, t _y) {
      this->x = _x;
      this->y = _y;
    }

    // TODO: add non-useless vector 2 properties operators
  
    template<typename s>
    operator ekg::vec2_t<s>() {
      return ekg::vec2_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y)
      };
    }

    template<typename s>
    ekg::vec2_t<t> operator / (s div_by) {
      return ekg::vec2_t<t> {
        this->x / div_by,
        this->y / div_by
      };
    }
  };

  template<typename t>
  struct vec3_t {
    union {
      struct {
        t x {};
        t y {};
        t z {};
      };
    };
  public:
    inline vec3_t() = default;

    inline vec3_t(t _x, t _y, t _z) {
      this->x = _x;
      this->y = _y;
      this->z = _z;
    }

    // TODO: add non-useless vector 3 properties operators

    template<typename s>
    operator ekg::vec3_t<s>() {
      return ekg::vec3_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y),
        static_cast<s>(this->z)
      };
    }
  };

  template<typename t>
  struct vec4_t {
    union {
      struct {
        t x {};
        t y {};
        t z {};
        t w {};
      };
    };
  public:
    inline vec4_t() = default;

    inline vec4_t(t _x, t _y, t _z, t _w) {
      this->x = _x;
      this->y = _y;
      this->z = _z;
      this->w = _w;
    }

    // TODO: add non-useless vector 4 properties operators

    template<typename s>
    operator ekg::vec2_t<s>() {
      return ekg::vec2_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y)
      };
    }

    template<typename s>
    operator ekg::vec3_t<s>() {
      return ekg::vec3_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y),
        static_cast<s>(this->z)
      };
    }

    template<typename s>
    operator ekg::vec4_t<s>() {
      return ekg::vec4_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y),
        static_cast<s>(this->z),
        static_cast<s>(this->w)
      };
    }

    template<typename s>
    ekg::vec4_t<t> operator / (s div_by) {
      return ekg::vec4_t<t> {
        this->x / div_by,
        this->y / div_by,
        this->z / div_by,
        this->w / div_by
      };
    }

    template<typename s>
    ekg::vec4_t<t> operator / (ekg::vec4_t<s> div_by) {
      return ekg::vec4_t<t> {
        this->x / div_by.x,
        this->y / div_by.y,
        this->z / div_by.z,
        this->w / div_by.w
      };
    }
  };

  template<typename t>
  struct rect_t {
  public: 
    union {
      struct {
        t x {};
        t y {};
        t w {};
        t h {};
      };
    };
  public:
    inline rect_t() = default;

    inline rect_t(t _x, t _y, t _w, t _h) {
      this->x = _x;
      this->y = _y;
      this->w = _w;
      this->h = _h;
    }

    // TODO: add non-useless rect (aka vector 4 properties) operators

    template<typename s>
    bool operator == (ekg::rect_t<s> comparable) {
      return (

        /**
         * If im not crazy may fequalsf be missing here, idk
         **/        

        static_cast<s>(this->x) == comparable.x
        &&
        static_cast<s>(this->y) == comparable.y
        &&
        static_cast<s>(this->w) == comparable.w
        &&
        static_cast<s>(this->h) == comparable.h
      );
    }

    template<typename s>
    bool operator != (ekg::rect_t<s> comparable) {
      return !(*this == comparable);
    }

    template<typename s>
    operator ekg::rect_t<s>() {
      return ekg::rect_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y),
        static_cast<s>(this->w),
        static_cast<s>(this->h)
      };
    }
    
    template<typename s>
    operator ekg::vec4_t<s>() {
      return ekg::vec4_t<s>{
        static_cast<s>(this->x),
        static_cast<s>(this->y),
        static_cast<s>(this->w),
        static_cast<s>(this->h)
      };
    }

    template<typename s>
    ekg::rect_t<t> operator + (ekg::rect_t<s> rect) {
      return ekg::rect_t<t>(
        this->x + static_cast<t>(rect.x),
        this->y + static_cast<t>(rect.y),
        this->w + static_cast<t>(rect.w),
        this->h + static_cast<t>(rect.h)
      );
    }

    template<typename s>
    ekg::rect_t<t> operator + (ekg::vec4_t<s> vec) {
      return ekg::rect_t<t>(
        this->x + static_cast<t>(vec.x),
        this->y + static_cast<t>(vec.y),
        this->w + static_cast<t>(vec.z),
        this->h + static_cast<t>(vec.w)
      );
    }

    template<typename s>
    ekg::rect_t<t> operator + (s expect_number) {
      return ekg::rect_t<t>(
        this->x + static_cast<t>(expect_number),
        this->y + static_cast<t>(expect_number),
        this->w + static_cast<t>(expect_number),
        this->h + static_cast<t>(expect_number)
      );
    }

    operator std::string() {
      std::string content {};
      content += std::to_string(this->x);
      content += ' ';
      content += std::to_string(this->y);
      content += ' ';
      content += std::to_string(this->w);
      content += ' ';
      content += std::to_string(this->h);
      return content;
    }
  };

  template<typename t>
  bool rect_precise_collide_vec2(ekg::rect_t<t> a, ekg::vec2_t<t> b) {
    return (
      b.x >= a.x && b.x <= a.x + a.w
      &&
      b.y >= a.y && b.y <= a.y + a.h
    );
  }

  template<typename t>
  bool rect_collide_vec2(ekg::rect_t<t> a, ekg::vec2_t<t> b) {
    return (
      b.x >= a.x && b.x <= a.x + a.w
      &&
      b.y >= a.y && b.y <= a.y + a.h
    );
  }

  template<typename t>
  bool rect_precise_collide_vec4(ekg::rect_t<t> a, ekg::vec4_t<t> b) {
    return (
      a.x <= b.x + b.z && a.x + a.w >= b.x
      &&
      a.y <= b.y + b.w && a.y + a.h >= b.y
    );
  }

  template<typename t>
  bool rect_collide_vec4(ekg::rect_t<t> a, ekg::vec4_t<t> b) {
    return (
      a.x < b.x + b.z && a.x + a.w > b.x
      &&
      a.y < b.y + b.w && a.y + a.h > b.y
    );
  }

  template<typename t>
  bool rect_precise_collide_rect(ekg::rect_t<t> a, ekg::rect_t<t> b) {
    return (
      a.x <= b.x + b.w && a.x + a.w >= b.x
      &&
      a.y <= b.y + b.h && a.y + a.h >= b.y
    );
  }

  template<typename t>
  bool rect_collide_rect(ekg::rect_t<t> a, ekg::rect_t<t> b) {
    return (
      a.x < b.x + b.w && a.x + a.w > b.x
      &&
      a.y < b.y + b.h && a.y + a.h > b.y
    );
  }

  struct rect_descriptor_t {
  public:
    ekg::rect_t<float> *p_rect {};
    ekg::flags_t flags {};
  };

  template<typename t>
  struct docker_t {
  public:
    ekg::rect_t<t> left {};
    ekg::rect_t<t> right {};
    ekg::rect_t<t> top {};
    ekg::rect_t<t> bottom {};
    ekg::rect_t<t> center {};
    ekg::rect_t<t> rect {};
  };

  template<typename t>
  ekg::flags_t vec2_collide_docker_if(
    ekg::vec2_t<t> vec,
    ekg::docker_t<t> docker,
    ekg::flags_t if_dock
  ) {
    if (
      ekg::has(if_dock, ekg::dock::full)
    ) {
      return ekg::rect_collide_vec2<t>(
        docker.rect, vec
      );
    }

    ekg::flags_t collided_dock {
      ekg::dock::none
    };

    collided_dock = (
      (
        ekg::has(if_dock, ekg::dock::left)
        &&
        ekg::rect_collide_vec2<t>(docker.left, vec)
      ) ?
      ekg::put(collided_dock, ekg::dock::left)
      :
      collided_dock
    );

    collided_dock = (
      (
        ekg::has(if_dock, ekg::dock::right)
        &&
        ekg::rect_collide_vec2<t>(docker.right, vec)
      ) ?
      ekg::put(collided_dock, ekg::dock::right)
      :
      collided_dock
    );

    collided_dock = (
      (
        ekg::has(if_dock, ekg::dock::top)
        &&
        ekg::rect_collide_vec2<t>(docker.top, vec)
      ) ?
      ekg::put(collided_dock, ekg::dock::top)
      :
      collided_dock
    );

    collided_dock = (
      (
        ekg::has(if_dock, ekg::dock::bottom)
        &&
        ekg::rect_collide_vec2<t>(docker.bottom, vec)
      ) ?
      ekg::put(collided_dock, ekg::dock::bottom)
      :
      collided_dock
    );
  
    collided_dock = (
      (
        ekg::has(if_dock, ekg::dock::center)
        &&
        ekg::rect_collide_vec2<t>(docker.center, vec)
      ) ?
      ekg::put(collided_dock, ekg::dock::center)
      :
      collided_dock
    );

    return collided_dock;
  }

  template<typename t>
  void scale_docker_by_margin(
    ekg::docker_t<t> &docker,
    ekg::vec2_t<t> margin,
    ekg::rect_t<t> rect
  ) {
    docker.rect = rect;
  
    docker.left.x = rect.x;
    docker.left.y = rect.y;
    docker.left.w = margin.x;
    docker.left.h = rect.h;
  
    docker.right.w = margin.x;
    docker.right.h = rect.h;
    docker.right.x = rect.x + rect.w - margin.x;
    docker.right.y = rect.y;
  
    docker.top.x = rect.x;
    docker.top.y = rect.y;
    docker.top.w = rect.w;
    docker.top.h = margin.y;
  
    docker.bottom.w = rect.w;
    docker.bottom.h = margin.y;
    docker.bottom.x = rect.x;
    docker.bottom.y = rect.y + rect.h - margin.y;
  
    float dx = margin.x / 2;
    float dy = margin.y / 2;
  
    docker.center.x = rect.x + (rect.w / 2) - dx;
    docker.center.y = rect.y + (rect.h / 2) - dy;
    docker.center.w = dx;
    docker.center.h = dy;
  }

  template<typename t>
  ekg::vec4_t<float> color(
    t r,
    t g,
    t b,
    t a
  ) {
    return ekg::vec4_t<t>(r, g, b, a) / 255;
  }

  template<typename t>
  constexpr t min_clamp(t a, t b) {
    return a < b ? b : a;
  }

  template<typename t>
  constexpr t max_clamp(t a, t b) {
    return a > b ? b : a;
  }

  template<typename t>
  constexpr t clamp(t a, t b, t c) {
    return ekg::min_clamp(ekg::max_clamp(a, c), b);
  }

  template<typename t>
  ekg::rect_t<t> clamp_rect_by_square(
    ekg::rect_t<t> rect,
    float square
  ) {
    const t zero {}; 
    return ekg::rect_t<t>(
      ekg::min_clamp<t>(rect.x, zero),
      ekg::min_clamp<t>(rect.y, zero),
      ekg::max_clamp<t>(rect.w, square),
      ekg::max_clamp<t>(rect.h, square)
    );
  } 

  void ortho(
    float *p_mat4x4,
    float left,
    float right,
    float top,
    float bottom
  );
}

#endif
