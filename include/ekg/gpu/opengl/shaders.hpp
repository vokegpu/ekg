#ifndef EKG_GPU_OPENGL_SHADERS_HPP
#define EKG_GPU_OPENGL_SHADERS_HPP

#include "ekg/gpu/api.hpp"

namespace ekg::gpu {
  void glsl_opengl_pipeline_vsh(
    const std::string &glsl_version,
    ekg::which_gpu_api which_gpu_api,
    std::string &out_shader
  );

  void glsl_opengl_pipeline_fsh(
    const std::string &glsl_version,
    ekg::which_gpu_api which_gpu_api,
    std::string &out_shader
  );
}

#endif
