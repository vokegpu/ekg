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

    }

    template<typename t>
    ekg::value<t> &value(
      t val
    ) {

    }

    template<typename t>
    ekg::value<t> &value(
      t *p_val
    ) {

    }

    template<typename t>
    ekg::value<t> &min() {

    }

    template<typename t>
    ekg::value<t> &min(
      t min
    ) {
      
    }

    template<typename t>
    ekg::value<t> &min(
      t &p_min
    ) {
      
    }

    template<typename t>
    ekg::value<t> &max() {
      
    }

    template<typename t>
    ekg::value<t> &max(
      t max
    ) {
      
    }

    template<typename t>
    ekg::value<t> &max(
      t &p_max
    ) {
      
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
      return this->ranges.at(index);
    } 
  };
}

#endif
