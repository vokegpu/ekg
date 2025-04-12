#ifndef EKG_UI_SLIDER_HPP
#define EKG_UI_SLIDER_HPP

#include "ekg/math/geometry.hpp"
#include <vector>

namespace ekg {
  struct slider_theme_t {
  public:
    ekg::vec4_t<float> background {};
    ekg::vec4_t<float> bar_background {};
    ekg::vec4_t<float> text {};
    ekg::vec4_t<float> outline {};
    ekg::vec4_t<float> active {};
    ekg::vec4_t<float> bar {};
    ekg::vec4_t<float> bar_outline {};
    ekg::vec4_t<float> highlight {};
    int32_t bar_thickness {};
    int32_t target_thickness {};
  };

  class slider_range {
  protected:
    ekg::number number_type {};

    ekg::value<double> f64 {};
    ekg::value<double> f64_min {};
    ekg::value<double> f64_max {};
  
    ekg::value<float> f32 {};
    ekg::value<float> f32_min {};
    ekg::value<float> f32_max {};

    ekg::value<uint64_t> u64 {};
    ekg::value<uint64_t> u64_min {};
    ekg::value<uint64_t> u64_max {};
  
    ekg::value<int64_t> i64 {};
    ekg::value<int64_t> i64_min {};
    ekg::value<int64_t> i64_max {};

    ekg::value<uint32_t> u32 {};
    ekg::value<uint32_t> u32_min {};
    ekg::value<uint32_t> u32_max {};
  
    ekg::value<int32_t> i32 {};
    ekg::value<int32_t> i32_min {};
    ekg::value<int32_t> i32_max {};

    ekg::value<uint16_t> u16 {};
    ekg::value<uint16_t> u16_min {};
    ekg::value<uint16_t> u16_max {};
  
    ekg::value<int16_t> i16 {};
    ekg::value<int16_t> i16_min {};
    ekg::value<int16_t> i16_max {};

    ekg::value<uint8_t> u8 {};
    ekg::value<uint8_t> u8_min {};
    ekg::value<uint8_t> u8_max {};
  
    ekg::value<int8_t> i8 {};
    ekg::value<int8_t> i8_min {};
    ekg::value<int8_t> i8_max {};
  public:
    template<typename t>
    void range(
      t value,
      t min,
      t max
    ) {
      ekg::retreive_number_type_from_variable_type<t>(
        this->number_type
      );

      switch (this->number_type) {
      case ekg::number::f64:
        this->f64.set_value(value);
        this->f64_min.set_value(min);
        this->f64_max.set_value(max);
        break;
      case ekg::number::f32:
        this->f32.set_value(value);
        this->f32_min.set_value(min);
        this->f32_max.set_value(max);
        break;
      case ekg::number::i64:
        this->i64.set_value(value);
        this->i64_min.set_value(min);
        this->i64_max.set_value(max);
        break;
      case ekg::number::u64:
        this->u64.set_value(value);
        this->u64_min.set_value(min);
        this->u64_max.set_value(max);
        break;
      case ekg::number::i32:
        this->i32.set_value(value);
        this->i32_min.set_value(min);
        this->i32_max.set_value(max);
        break;
      case ekg::number::u32:
        this->u32.set_value(value);
        this->u32_min.set_value(min);
        this->u32_max.set_value(max);
        break;
      case ekg::number::i16:
        this->i16.set_value(value);
        this->i16_min.set_value(min);
        this->i16_max.set_value(max);
        break;
      case ekg::number::u16:
        this->u16.set_value(value);
        this->u16_min.set_value(min);
        this->u16_max.set_value(max);
        break;
      case ekg::number::i8:
        this->i8.set_value(value);
        this->i8_min.set_value(min);
        this->i8_max.set_value(max);
        break;
      case ekg::number::u8:
        this->u8.set_value(value);
        this->u8_min.set_value(min);
        this->u8_max.set_value(max);
        break;
      }
    }

    template<typename t>
    ekg::value<t> &value() {
      switch (this->number_type) {
      case ekg::number::f64:
        return this->f64;
      case ekg::number::f32:
        return this->f32;
      case ekg::number::i64:
        return this->i64;
      case ekg::number::u64:
        return this->u64;
      case ekg::number::i32:
        return this->i32;
      case ekg::number::u32:
        return this->u32;
      case ekg::number::i16:
        return this->i16;
      case ekg::number::u16:
        return this->u16;
      case ekg::number::i8:
        return this->i8;
      default:
        break;
      }

      return this->u8;
    }

    template<typename t>
    ekg::value<t> &value(
      t val
    ) {
      switch (this->number_type) {
      case ekg::number::f64:
        this->f64.set_value(val);
        return this->f64;
      case ekg::number::f32:
        this->f32.set_value(val);
        return this->f32;
      case ekg::number::i64:
        this->i64.set_value(val);
        return this->i64;
      case ekg::number::u64:
        this->u64.set_value(val);
        return this->u64;
      case ekg::number::i32:
        this->i32.set_value(val);
        return this->i32;
      case ekg::number::u32:
        this->u32.set_value(val);
        return this->u32;
      case ekg::number::i16:
        this->i16.set_value(val);
        return this->i16;
      case ekg::number::u16:
        this->u16.set_value(val);
        return this->u16;
      case ekg::number::i8:
        this->i8.set_value(val);
        return this->i8;
      default:
        break;
      }

      this->u8.set_value(val);
      return this->u8;
    }

    template<typename t>
    ekg::value<t> &value(
      t *p_val
    ) {
      switch (this->number_type) {
      case ekg::number::f64:
        this->f64.move(p_val);
        return this->f64;
      case ekg::number::f32:
        this->f32.move(p_val);
        return this->f32;
      case ekg::number::i64:
        this->i64.move(p_val);
        return this->i64;
      case ekg::number::u64:
        this->u64.move(p_val);
        return this->u64;
      case ekg::number::i32:
        this->i32.move(p_val);
        return this->i32;
      case ekg::number::u32:
        this->u32.move(p_val);
        return this->u32;
      case ekg::number::i16:
        this->i16.move(p_val);
        return this->i16;
      case ekg::number::u16:
        this->u16.move(p_val);
        return this->u16;
      case ekg::number::i8:
        this->i8.move(p_val);
        return this->i8;
      default:
        break;
      }

      this->u8.move(p_val);
      return this->u8;
    }

    template<typename t>
    ekg::value<t> &min() {
      switch (this->number_type) {
      case ekg::number::f64:
        return this->f64_min;
      case ekg::number::f32:
        return this->f32_min;
      case ekg::number::i64:
        return this->i64_min;
      case ekg::number::u64:
        return this->u64_min;
      case ekg::number::i32:
        return this->i32_min;
      case ekg::number::u32:
        return this->u32_min;
      case ekg::number::i16:
        return this->i16_min;
      case ekg::number::u16:
        return this->u16_min;
      case ekg::number::i8:
        return this->i8_min;
      default:
        break;
      }

      return this->u8_min;
    }

    template<typename t>
    ekg::value<t> &min(
      t min
    ) {
      switch (this->number_type) {
      case ekg::number::f64:
        this->f64_min.set_value(min);
        return this->f64_min;
      case ekg::number::f32:
        this->f32_min.set_value(min);
        return this->f32_min;
      case ekg::number::i64:
        this->i64_min.set_value(min);
        return this->i64_min;
      case ekg::number::u64:
        this->u64_min.set_value(min);
        return this->u64_min;
      case ekg::number::i32:
        this->i32_min.set_value(min);
        return this->i32_min;
      case ekg::number::u32:
        this->u32_min.set_value(min);
        return this->u32_min;
      case ekg::number::i16:
        this->i16_min.set_value(min);
        return this->i16_min;
      case ekg::number::u16:
        this->u16_min.set_value(min);
        return this->u16_min;
      case ekg::number::i8:
        this->i8_min.set_value(min);
        return this->i8_min;
      default:
        break;
      }

      this->u8_min.set_value(min);
      return this->u8_min;
    }

    template<typename t>
    ekg::value<t> &min(
      t *p_min
    ) {
      switch (this->number_type) {
      case ekg::number::f64:
        this->f64_min.move(p_min);
        return this->f64_min;
      case ekg::number::f32:
        this->f32_min.move(p_min);
        return this->f32_min;
      case ekg::number::i64:
        this->i64_min.move(p_min);
        return this->i64_min;
      case ekg::number::u64:
        this->u64_min.move(p_min);
        return this->u64_min;
      case ekg::number::i32:
        this->i32_min.move(p_min);
        return this->i32_min;
      case ekg::number::u32:
        this->u32_min.move(p_min);
        return this->u32_min;
      case ekg::number::i16:
        this->i16_min.move(p_min);
        return this->i16_min;
      case ekg::number::u16:
        this->u16_min.move(p_min);
        return this->u16_min;
      case ekg::number::i8:
        this->i8_min.move(p_min);
        return this->i8_min;
      default:
        break;
      }

      this->u8_min.move(p_min);
      return this->u8_min;
    }

    template<typename t>
    ekg::value<t> &max() {
      switch (this->number_type) {
      case ekg::number::f64:
        return this->f64_max;
      case ekg::number::f32:
        return this->f32_max;
      case ekg::number::i64:
        return this->i64_max;
      case ekg::number::u64:
        return this->u64_max;
      case ekg::number::i32:
        return this->i32_max;
      case ekg::number::u32:
        return this->u32_max;
      case ekg::number::i16:
        return this->i16_max;
      case ekg::number::u16:
        return this->u16_max;
      case ekg::number::i8:
        return this->i8_max;
      default:
        break;
      }

      return this->u8_max;
    }

    template<typename t>
    ekg::value<t> &max(
      t max
    ) {
      switch (this->number_type) {
      case ekg::number::f64:
        this->f64_max.set_value(max);
        return this->f64_max;
      case ekg::number::f32:
        this->f32_max.set_value(max);
        return this->f32_max;
      case ekg::number::i64:
        this->i64_max.set_value(max);
        return this->i64_max;
      case ekg::number::u64:
        this->u64_max.set_value(max);
        return this->u64_max;
      case ekg::number::i32:
        this->i32_max.set_value(max);
        return this->i32_max;
      case ekg::number::u32:
        this->u32_max.set_value(max);
        return this->u32_max;
      case ekg::number::i16:
        this->i16_max.set_value(max);
        return this->i16_max;
      case ekg::number::u16:
        this->u16_max.set_value(max);
        return this->u16_max;
      case ekg::number::i8:
        this->i8_max.set_value(max);
        return this->i8_max;
      default:
        break;
      }

      this->u8_max.set_value(max);
      return this->u8_max;
    }

    template<typename t>
    ekg::value<t> &max(
      t *p_max
    ) {
      switch (this->number_type) {
      case ekg::number::f64:
        this->f64_max.move(p_max);
        return this->f64_max;
      case ekg::number::f32:
        this->f32_max.move(p_max);
        return this->f32_max;
      case ekg::number::i64:
        this->i64_max.move(p_max);
        return this->i64_max;
      case ekg::number::u64:
        this->u64_max.move(p_max);
        return this->u64_max;
      case ekg::number::i32:
        this->i32_max.move(p_max);
        return this->i32_max;
      case ekg::number::u32:
        this->u32_max.move(p_max);
        return this->u32_max;
      case ekg::number::i16:
        this->i16_max.move(p_max);
        return this->i16_max;
      case ekg::number::u16:
        this->u16_max.move(p_max);
        return this->u16_max;
      case ekg::number::i8:
        this->i8_max.move(p_max);
        return this->i8_max;
      default:
        break;
      }

      this->u8_max.move(p_max);
      return this->u8_max;
    }
  };

  struct slider_t {
  public:
    std::string tag {};
    ekg::flags_t dock {ekg::dock::left};
    std::vector<ekg::slider_range> ranges {};
    ekg::font text_font_size {ekg::font::normal};
    ekg::rect_t<float> rect {};
    ekg::type type {ekg::type::slider};
    ekg::actions actions {};
    ekg::slider_theme_t theme {};
  public:
    ekg::slider_range &operator [] (size_t index) {
      if (index >= this->ranges.size()) {
        this->ranges.resize(index + 1);
      }

      return this->ranges.at(index);
    } 
  };
}

#endif
