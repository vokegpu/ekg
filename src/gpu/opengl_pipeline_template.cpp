#include "ekg/gpu/opengl_pipeline_template.hpp"

void ekg::gpu::get_standard_vertex_shader(
  std::string glsl_version,
  ekg::gpu_api gpu_api,
  std::string &output_kernel_source
) {
  output_kernel_source = glsl_version + R"(
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aTexCoord;

    uniform mat4 uProjection;
    uniform vec4 uRect;

    out vec2 vTexCoord;
    out vec2 vPos;
    out vec4 vRect;

    void main() {
      vec2 vertex = aPos;

      if (uRect.z > -1.0f && uRect.w > -1.0f) {
        vertex *= uRect.zw;
      }

      vertex += uRect.xy;

      gl_Position = uProjection * vec4(vertex, 0.0f, 1.0f);
      vTexCoord = aTexCoord;
      vRect = uRect;
      vPos = aPos;
    }
  )";
}

void ekg::gpu::get_standard_fragment_shader(
  std::string glsl_version,
  ekg::gpu_api gpu_api,
  std::string &output_kernel_source
) {
  output_kernel_source = glsl_version + R"(
    layout (location = 0) out vec4 aFragColor;
    uniform sampler2D uTextureSampler;

    in vec2 vTexCoord;
    in vec2 vPos;
    in vec4 vRect;

    uniform int uLineThickness;
    uniform int uActiveTexture;
    uniform float uViewportHeight;
    uniform float uContent[8];

    void main() {
      aFragColor = vec4(
        uContent[0],
        uContent[1],
        uContent[2],
        uContent[3]
      );

      vec2 fragPos = vec2(gl_FragCoord.x, uViewportHeight - gl_FragCoord.y);

      /**
       * The scissoring works like swapchain-one (does not stack), of course,
       * this scissor is a little different, the pixel-perfect precision makes
       * a better cut of fragments. And does not require any overhead from
       * calling command buffers to GPU rastarizer.
       **/
      bool shouldDiscard = (
        fragPos.x <= uContent[4]
        ||
        fragPos.y <= uContent[5]
        ||
        fragPos.x >= uContent[4] + uContent[6]
        ||
        fragPos.y >= uContent[5] + uContent[7]
      );

      float lineThicknessf = float(uLineThickness);

      /**
       * The pixel-perfect outline is possible on fragment shader,
       * due the precision of fragments position, and the
       * normalised-space.
       **/
      if (uLineThickness > 0) {
        vec4 outline = vec4(
          vRect.x + lineThicknessf,
          vRect.y + lineThicknessf,
          vRect.z - (lineThicknessf * 2.0f),
          vRect.w - (lineThicknessf * 2.0f)
        );

        shouldDiscard = (
          shouldDiscard || (
            fragPos.x > outline.x &&
            fragPos.x < outline.x + outline.z &&
            fragPos.y > outline.y &&
            fragPos.y < outline.y + outline.w
          )
        );
      } else if (uLineThickness < 0) {
        float radius = vRect.z / 2.0f;

        vec2 diff = vec2(
          (vRect.x + radius) - fragPos.x,
          (vRect.y + radius) - fragPos.y
        );

        float dist = (diff.x * diff.x + diff.y * diff.y);
        aFragColor.w = (
          1.0f - smoothstep(0.0, radius * radius, dot(dist, dist))
        );
      }

      /**
       * The discard must not call `discard` keyword,
       * discarding pixels using keyword is performanceless
       * comparated to alpha blending equals to zero.
       **/
      if (shouldDiscard) {
        aFragColor.w = 0.0f;
      } else {
        vec4 textureColor;
        switch (uActiveTexture) {
          case 1:           
            textureColor = texture(uTextureSampler, vTexCoord);

            /**
             * The sampler used here is the font, and this sampler needs swizzled mapped,
             * instead of doing swizzling on CPU-side, here is actually the best place.
             * Due the necessity of put swizzle for ttf text fonts, the emojis must not swizzle.
             * The non swizzable range masterfully fix it.
             * 
             * vRect.z is negative, because any concave rendering shape does not have a fixed
             * dimension size. So the rendering engine re-uses the Rect width to calculate
             * when must stop the GPU-side swizzle.
             **/
            float non_swizzlable_range = -vRect.z;

            if (vTexCoord.x < non_swizzlable_range) {
              textureColor = textureColor.aaar;
              textureColor = vec4(
                textureColor.rgb * aFragColor.rgb,
                textureColor.a
              );
            }

            aFragColor = vec4(
              textureColor.rgb,
              textureColor.a - (1.0f - aFragColor.a)
            );

            //aFragColor = vec4(aFragColor.a, 0.0f, 0.0f, 1.0f);
            break;
          case 2:
            textureColor = texture(uTextureSampler, vPos);

            aFragColor = vec4(
              textureColor.rgb,
              textureColor.a - (1.0f - aFragColor.a)
            );
            break;
        }
      }
    }
  )";
}
