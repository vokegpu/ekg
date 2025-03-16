/**
 * MIT License
 * 
 * Copyright (c) 2022-2024 Rina Wilk / vokegpu@gmail.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef EKG_DRAW_FONT_RENDERER_HPP
#define EKG_DRAW_FONT_RENDERER_HPP

#include <unordered_map>

#include "ekg/gpu/allocator.hpp"
#include "ekg/math/geometry.hpp"
#include "ekg/gpu/api.hpp"

#define FT_CONFIG_OPTION_USE_PNG

namespace ekg::draw {
  /**
   * Normally an UV is normalized-clamped, so multiplying by 100
   * do the number be able to sum the factor and show some
   * effect on screen.
   * 
   * For example, a same number but UV was updated, no factor is changed
   * then the way to fix it is make UV noticable.
   **/
  constexpr int32_t generate_factor_hash(
    float axis,
    char32_t char32,
    float x
  ) {
    return static_cast<int32_t>(
      axis + char32 + x * 100
    );
  }

  class font_renderer {
  public:
    std::vector<char32_t> loaded_sampler_generate_list {};
    uint64_t last_sampler_generate_list_size {};

    std::unordered_map<char32_t, ekg::io::glyph_char_t> mapped_glyph_char_data {};
    std::array<ekg::io::font_face_t, ekg::io::supported_faces_size> faces {};

    ekg::sampler_t atlas_texture_sampler {};
    ekg::rect_t<int32_t> atlas_rect {};
    float offset_text_height {};

    uint32_t font_size {};
    float text_height {};
    float non_swizzlable_range {};
    FT_Bool ft_bool_kerning {};

    bool font_size_changed {};
    bool was_initialized {};
    bool is_any_functional_font_face_loaded {};

    ekg::gpu::allocator *p_allocator {};
  public:
    /**
     * Return the sampler atlas with all font(s) combined.
     */
    ekg::sampler_t *get_atlas_texture_sampler();

    /**
     * Return the text width.
     * Note: font-rendering is UTF-8-based.
     */
    float get_text_width(std::string_view text);

    /**
     * Return the text width and the lines `\n`.
     * Note: font-rendering is UTF-8-based.
     */
    float get_text_width(std::string_view text, int32_t &lines);

    /**
     * Return the font face height.
     */
    float get_text_height();

    /**
     * Set a new font face for emoji, check FreeType docs.
     */
    void set_font_emoji(std::string_view font_face_emoji_path);

    /**
     * Set a new font face, check FreeType docs.
     */
    void set_font(std::string_view font_face_path);

    /**
     * Set the font face height.
     */
    void set_size(uint32_t font_face_size);

    /**
     * Reload the font face with the new metrics and file path.
     */
    void reload();

    /**
     * Bind an external GPU allocator, but is not recommend pass a nullptr value.
     */
    void bind_allocator(ekg::gpu::allocator *p_allocator_bind);

    /**
     * Generate GPU data to draw text on screen.
     */
    void blit(std::string_view text, float x, float y, const ekg::vec4_t<float> &color);

    /**
     * Init the internal-system of font-rendering.
     */
    void init();

    /**
     * Quit the internal-system and free FreeType features from memory.
     */
    void quit();

    /**
     * flush new glyph(s) to re-generate texture atlas and map for the GPU-side.
     **/
    void flush();
  };

  /**
   * Temp solution for fixed-font-renderer
   **/
  ekg::draw::font_renderer &get_font_renderer(ekg::font font);
}

#endif
