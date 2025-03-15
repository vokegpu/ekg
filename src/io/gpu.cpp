#include "ekg/core/runtime.hpp"

ekg::flags_t ekg::gpu_allocate_sampler(
  ekg::sampler_allocate_info_t *p_sampler_allocate_info,
  ekg::sampler_t *p_sampler
) {
  return ekg::p_core->p_gpu_api->allocate_sampler(
    p_sampler_allocate_info,
    p_sampler
  );
}

ekg::flags_t ekg::gpu_fill_sampler(
  ekg::sampler_fill_info_t *p_sampler_fill_info,
  ekg::sampler_t *p_sampler
) {
  return ekg::p_core->p_gpu_api->fill_sampler(
    p_sampler_fill_info,
    p_sampler
  );
}

ekg::flags_t ekg::image_src_r8_convert_to_r8g8b8a8(
  ekg::vec2_t<int32_t> size,
  const unsigned char *p_src,
  std::vector<unsigned char> &dst
) {
  if (size.x == 0 || size.y == 0 || p_src == nullptr || dst.empty()) {
    return ekg::result::failed;
  }

  size_t index {};
  for (size_t it {}; it < (size.x * size.y); it++) {
    const unsigned char &char8_red_color {
      p_src[it]
    };

    index = it * 4;

    if (index == dst.size()) {
      break;
    }

    /**
     * may I be wrong? but the format is ARGB and not RGBA,
     * I do not know.
     **/

    dst.at(index + 0) = char8_red_color;
    dst.at(index + 1) = 255;
    dst.at(index + 2) = 255;
    dst.at(index + 3) = 255;
  }

  return ekg::result::success;
}
