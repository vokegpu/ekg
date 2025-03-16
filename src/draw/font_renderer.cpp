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

#include "ekg/draw/font_renderer.hpp"
#include "ekg/io/text.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/core/context.hpp"

ekg::sampler_t *ekg::draw::font_renderer::get_atlas_texture_sampler() {
  return &this->atlas_texture_sampler;
}

float ekg::draw::font_renderer::get_text_width(std::string_view text, int32_t &lines) {
  if (
      text.empty()
      ||
      !this->is_any_functional_font_face_loaded
    ) {
    return 0.0f;
  }

  FT_Face ft_face {};
  FT_UInt ft_uint_previous {};
  FT_Vector ft_vector_previous_char {};
  FT_GlyphSlot ft_glyph_slot {};

  float text_width {};
  float largest_text_width {};

  int32_t lines_count {};
  uint64_t text_size {text.size()};
  
  char32_t char32 {};
  uint8_t char8 {};
  std::string utf_string {};

  bool break_text {};
  bool r_n_break_text {};

  ekg::io::font_face_t &text_font_face {this->faces[ekg::io::font_face_type::text]};
  ekg::io::font_face_t &emojis_font_face {this->faces[ekg::io::font_face_type::emojis]};
  ekg::io::font_face_t &kanjis_font_face {this->faces[ekg::io::font_face_type::kanjis]};

  for (uint64_t it {}; it < text_size; it++) {
    char8 = static_cast<uint8_t>(text.at(it));
    it += ekg::utf_check_sequence(char8, char32, utf_string, text, it);
    break_text = char8 == '\n';
    if (break_text || (r_n_break_text = (char8 == '\r' && it < text_size && text.at(it + 1) == '\n'))) {
      it += static_cast<uint64_t>(r_n_break_text);
      largest_text_width = ekg::min_clamp(largest_text_width, text_width);
      text_width = 0.0f;
      lines_count++;
      continue;
    }

    switch (char32 < 256 || !emojis_font_face.was_loaded) {
      case true: {
        ft_face = text_font_face.ft_face;
        ft_glyph_slot = text_font_face.ft_face->glyph;
        break;
      }

      default: {
        ft_face = emojis_font_face.ft_face;
        ft_glyph_slot = emojis_font_face.ft_face->glyph;
        break;
      }
    }

    if (this->ft_bool_kerning && ft_uint_previous) {
      FT_Get_Kerning(ft_face, ft_uint_previous, char32, 0, &ft_vector_previous_char);
      text_width += static_cast<float>(ft_vector_previous_char.x >> 6);
    }

    ekg::io::glyph_char_t &char_data {this->mapped_glyph_char_data[char32]};

    if (!char_data.was_sampled) {
      if (
          FT_Load_Char(
            ft_face,
            char32,
            FT_LOAD_RENDER | FT_LOAD_DEFAULT | FT_LOAD_COLOR
          )
        ) {
        continue;
      }

      char_data.wsize = static_cast<float>(static_cast<int32_t>(ft_glyph_slot->advance.x >> 6));
      this->loaded_sampler_generate_list.emplace_back(char32);
      char_data.was_sampled = true;
    }

    ft_uint_previous = char32;
    text_width += this->mapped_glyph_char_data[char32].wsize;
  }

  lines = ekg::min_clamp(lines, lines_count);
  largest_text_width = ekg::min_clamp(largest_text_width, text_width);

  return largest_text_width;
}

float ekg::draw::font_renderer::get_text_width(std::string_view text) {
  if (
      text.empty()
      ||
      !this->is_any_functional_font_face_loaded
    ) {
    return 0.0f;
  }

  FT_Face ft_face {};
  FT_UInt ft_uint_previous {};
  FT_Vector ft_vector_previous_char {};
  FT_GlyphSlot ft_glyph_slot {};

  float text_width {};
  float largest_text_width {};
  char32_t char32 {};

  uint64_t text_size {text.size()};
  std::string utf_string {};
  uint8_t char8 {};

  bool break_text {};
  bool r_n_break_text {};

  ekg::io::font_face_t &text_font_face {this->faces[ekg::io::font_face_type::text]};
  ekg::io::font_face_t &emojis_font_face {this->faces[ekg::io::font_face_type::emojis]};
  ekg::io::font_face_t &kanjis_font_face {this->faces[ekg::io::font_face_type::kanjis]};

  for (uint64_t it {}; it < text_size; it++) {
    char8 = static_cast<uint8_t>(text.at(it));
    it += ekg::utf_check_sequence(char8, char32, utf_string, text, it);

    break_text = char8 == '\n';
    if (break_text || (r_n_break_text = (char8 == '\r' && it < text_size && text.at(it + 1) == '\n'))) {
      it += static_cast<uint64_t>(r_n_break_text);
      largest_text_width = ekg::min_clamp(largest_text_width, text_width);
      text_width = 0.0f;
      continue;
    }

    switch (char32 < 256 || !emojis_font_face.was_loaded) {
      case true: {
        ft_face = text_font_face.ft_face;
        ft_glyph_slot = text_font_face.ft_face->glyph;
        break;
      }

      default: {
        ft_face = emojis_font_face.ft_face;
        ft_glyph_slot = emojis_font_face.ft_face->glyph;
        break;
      }
    }

    if (this->ft_bool_kerning && ft_uint_previous) {
      FT_Get_Kerning(ft_face, ft_uint_previous, char32, 0, &ft_vector_previous_char);
      text_width += static_cast<float>(ft_vector_previous_char.x >> 6);
    }

    ekg::io::glyph_char_t &char_data {this->mapped_glyph_char_data[char32]};

    if (!char_data.was_sampled) {
      if (
          FT_Load_Char(
            ft_face,
            char32,
            FT_LOAD_RENDER | FT_LOAD_DEFAULT | FT_LOAD_COLOR
          )
        ) {
        continue;
      }

      char_data.wsize = static_cast<float>(static_cast<int32_t>(ft_glyph_slot->advance.x >> 6));
      this->loaded_sampler_generate_list.emplace_back(char32);
      char_data.was_sampled = true;
    }

    ft_uint_previous = char32;
    text_width += char_data.wsize;
  }

  largest_text_width = ekg::min_clamp(largest_text_width, text_width);
  return largest_text_width;
}

float ekg::draw::font_renderer::get_text_height() {
  return this->text_height;
}

void ekg::draw::font_renderer::set_font(std::string_view path) {
  ekg::io::font_face_t &font_face {
    this->faces[ekg::io::font_face_type::text]
  };

  if (!path.empty() && font_face.path != path) {
    font_face.path = path;
    font_face.was_face_changed = true;
    font_face.was_size_changed = true;

    this->reload();
  }
}

void ekg::draw::font_renderer::set_font_emoji(std::string_view path) {
  ekg::io::font_face_t &font_face {
    this->faces[ekg::io::font_face_type::emojis]
  };

  if (!path.empty() && font_face.path != path) {
    font_face.path = path;
    font_face.was_face_changed = true;
    font_face.was_size_changed = true;

    this->reload();
  }
}

void ekg::draw::font_renderer::set_size(uint32_t size) {
  if (this->font_size != size) {
    for (size_t it {}; it < ekg::io::supported_faces_size; it++) {
      ekg::io::font_face_t &font_face {
        this->faces[it]
      };

      font_face.size = size;
      font_face.was_size_changed = true;
    }

    this->font_size = size;
    this->reload();
  }
}

void ekg::draw::font_renderer::reload() {
  if (this->font_size == 0) {
    return;
  }

  size_t functional_fonts {};
  ekg::flags_t flags {};

  for (size_t it {}; it < ekg::io::supported_faces_size; it++) {
    ekg::io::font_face_t &font_face {
      this->faces[it]
    };

    flags = ekg::io::refresh_font_face(
      &font_face
    );

    if (ekg::has(flags, ekg::result::success)) {
      functional_fonts++;
    }
  }

  this->is_any_functional_font_face_loaded = functional_fonts != 0;
  if (!this->is_any_functional_font_face_loaded) {
    ekg::log() << "EKG could not make font rendering reload, there are no functional font faces loaded!";
    return;
  }

  this->atlas_rect.w = 0;
  this->atlas_rect.h = 0;

  ekg::io::font_face_t &text_font_face {this->faces[ekg::io::font_face_type::text]};
  ekg::io::font_face_t &emojis_font_face {this->faces[ekg::io::font_face_type::emojis]};
  ekg::io::font_face_t &kanjis_font_face {this->faces[ekg::io::font_face_type::kanjis]};

  text_font_face.highest_glyph_size = FT_Vector {};
  emojis_font_face.highest_glyph_size = FT_Vector {};

  this->ft_bool_kerning = FT_HAS_KERNING(text_font_face.ft_face);
  text_font_face.ft_glyph_slot = text_font_face.ft_face->glyph;

  if (emojis_font_face.was_loaded) {
    emojis_font_face.ft_glyph_slot = emojis_font_face.ft_face->glyph;
  }

  FT_GlyphSlot ft_glyph_slot {};
  FT_Face ft_face {};

  ekg::io::font_face_t *p_font_face_picked {};
  for (char32_t &char32 : this->loaded_sampler_generate_list) {
    switch (char32 < 256 || !emojis_font_face.was_loaded) {
      case true: {
        ft_face = text_font_face.ft_face;
        ft_glyph_slot = text_font_face.ft_face->glyph;
        flags = FT_LOAD_RENDER;
        p_font_face_picked = &text_font_face;
        break;
      }

      default: {
        ft_face = emojis_font_face.ft_face;
        ft_glyph_slot = emojis_font_face.ft_face->glyph;
        flags = FT_LOAD_RENDER | FT_LOAD_COLOR;
        p_font_face_picked = &emojis_font_face;
        break;
      }
    }

    if (FT_Load_Char(ft_face, char32, flags)) {
      continue;
    }

    ekg::io::glyph_char_t &char_data {
      this->mapped_glyph_char_data[char32]
    };

    char_data.w = static_cast<float>(ft_glyph_slot->bitmap.width);
    char_data.h = static_cast<float>(ft_glyph_slot->bitmap.rows);

    char_data.left = static_cast<float>(ft_glyph_slot->bitmap_left);
    char_data.top = static_cast<float>(ft_glyph_slot->bitmap_top);
    char_data.wsize = static_cast<float>(static_cast<int32_t>(ft_glyph_slot->advance.x >> 6));

    this->atlas_rect.w += static_cast<int32_t>(char_data.w);
    this->atlas_rect.h = ekg::min_clamp<int32_t>(this->atlas_rect.h, static_cast<int32_t>(char_data.h));

    p_font_face_picked->highest_glyph_size.x = ekg::min_clamp(
      static_cast<int32_t>(p_font_face_picked->highest_glyph_size.x),
      static_cast<int32_t>(char_data.w)
    );

    p_font_face_picked->highest_glyph_size.y = ekg::min_clamp(
      static_cast<int32_t>(p_font_face_picked->highest_glyph_size.y),
      static_cast<int32_t>(char_data.h)
    );
  }

  this->text_height = static_cast<float>(this->font_size);
  this->offset_text_height = static_cast<int32_t>(this->text_height / 6) / 2;

  ekg::p_core->p_gpu_api->gen_font_atlas_and_map_glyph(
    &this->atlas_texture_sampler,
    &text_font_face,
    &emojis_font_face,
    nullptr, // must impl kanjis
    this->atlas_rect,
    this->loaded_sampler_generate_list,
    this->mapped_glyph_char_data,
    this->non_swizzlable_range
  );
}

void ekg::draw::font_renderer::bind_allocator(ekg::gpu::allocator *p_allocator_bind) {
  this->p_allocator = p_allocator_bind;
}

void ekg::draw::font_renderer::blit(
  std::string_view text,
  float x,
  float y,
  const ekg::vec4_t<float> &color
) {
  if (
      this->p_allocator == nullptr
      ||
      color.w < 0.1f 
      ||
      text.empty()
      ||
      !this->is_any_functional_font_face_loaded
  ) {
    return;
  }

  x = static_cast<float>(static_cast<int32_t>(x));
  y = static_cast<float>(static_cast<int32_t>(y - this->offset_text_height));

  ekg::io::gpu_data_t &data {this->p_allocator->bind_current_data()};

  data.buffer_content[0] = x;
  data.buffer_content[1] = y;
  data.buffer_content[2] = static_cast<float>(-this->non_swizzlable_range);
  data.buffer_content[3] = static_cast<float>(ekg::gpu::allocator::concave);

  data.buffer_content[4] = color.x;
  data.buffer_content[5] = color.y;
  data.buffer_content[6] = color.z;
  data.buffer_content[7] = color.w;

  ekg::rect_t<float> vertices {};
  ekg::rect_t<float> coordinates {};

  x = 0.0f;
  y = 0.0f;

  data.factor = 1;
  char32_t char32 {};
  uint8_t char8 {};

  std::string utf_string {};
  uint64_t text_size {text.size()};

  bool break_text {};
  bool r_n_break_text {};

  FT_Face ft_face {};
  FT_Vector ft_vector_previous_char {};
  char32_t ft_uint_previous {};

  ekg::io::font_face_t &text_font_face {this->faces[ekg::io::font_face_type::text]};
  ekg::io::font_face_t &emojis_font_face {this->faces[ekg::io::font_face_type::emojis]};
  ekg::io::font_face_t &kanjis_font_face {this->faces[ekg::io::font_face_type::kanjis]};

  for (uint64_t it {}; it < text_size; it++) {
    char8 = static_cast<uint8_t>(text.at(it));
    it += ekg::utf_check_sequence(char8, char32, utf_string, text, it);

    break_text = char8 == '\n';
    if (
        break_text
        ||
        (
          r_n_break_text = (
            char8 == '\r' && it < text_size && text.at(it + 1) == '\n'
          )
        )
      ) {
      ekg::io::glyph_char_t &char_data {this->mapped_glyph_char_data[char32]};

      it += static_cast<uint64_t>(r_n_break_text);
      data.factor += ekg::draw::generate_factor_hash(y, char32, char_data.x);

      y += this->text_height;
      x = 0.0f;
      continue;
    }

    if (this->ft_bool_kerning && ft_uint_previous) {
      switch (char32 < 256 || !emojis_font_face.was_loaded) {
        case true: {
          ft_face = text_font_face.ft_face;
          break;
        }

        default: {
          ft_face = emojis_font_face.ft_face;
          break;
        }
      }

      FT_Get_Kerning(ft_face, ft_uint_previous, char32, 0, &ft_vector_previous_char);
      x += static_cast<float>(ft_vector_previous_char.x >> 6);
    }

    ekg::io::glyph_char_t &char_data {this->mapped_glyph_char_data[char32]};

    if (!char_data.was_sampled) {
      this->loaded_sampler_generate_list.emplace_back(char32);
      char_data.was_sampled = true;
    }

    vertices.x = x + char_data.left;
    vertices.y = y + this->atlas_rect.h - char_data.top;

    vertices.w = char_data.w;
    vertices.h = char_data.h;

    coordinates.x = char_data.x;
    coordinates.w = vertices.w / this->atlas_rect.w;
    coordinates.h = vertices.h / this->atlas_rect.h;

    this->p_allocator->push_back_geometry(
      vertices.x,
      vertices.y,
      coordinates.x,
      coordinates.y
    );

    this->p_allocator->push_back_geometry(
      vertices.x,
      vertices.y + vertices.h,
      coordinates.x,
      coordinates.y + coordinates.h
    );

    this->p_allocator->push_back_geometry(
      vertices.x + vertices.w,
      vertices.y + vertices.h,
      coordinates.x + coordinates.w,
      coordinates.y + coordinates.h
    );

    this->p_allocator->push_back_geometry(
      vertices.x + vertices.w,
      vertices.y + vertices.h,
      coordinates.x + coordinates.w,
      coordinates.y + coordinates.h
    );

    this->p_allocator->push_back_geometry(
      vertices.x + vertices.w,
      vertices.y,
      coordinates.x + coordinates.w,
      coordinates.y
    );

    this->p_allocator->push_back_geometry(
      vertices.x,
      vertices.y,
      coordinates.x,
      coordinates.y
    );

    x += char_data.wsize;
    ft_uint_previous = char32;

    data.factor += ekg::draw::generate_factor_hash(x, char32, char_data.x);
  }

  this->flush();
  this->p_allocator->bind_texture(&this->atlas_texture_sampler);
  this->p_allocator->dispatch();
}

void ekg::draw::font_renderer::flush() {
  uint64_t size {this->loaded_sampler_generate_list.size()};
  if (this->last_sampler_generate_list_size != size) {
    ekg::log() << "Sampler updated from-to: " << this->last_sampler_generate_list_size << " " << size;

    this->reload();
    this->last_sampler_generate_list_size = size;

    ekg::viewport.redraw = true;
  }
}

void ekg::draw::font_renderer::init() {
  if (this->was_initialized) {
    return;
  }

  this->was_initialized = true;

  this->loaded_sampler_generate_list.resize(256);
  for (char32_t char32 {}; char32 < 256; char32++) {
    this->loaded_sampler_generate_list.at(char32) = char32;
  }

  ekg::log() << "Initializing 256 default chars!";
}

void ekg::draw::font_renderer::quit() {
  ekg::io::font_face_t &text_font_face {this->faces[ekg::io::font_face_type::text]};
  ekg::io::font_face_t &emojis_font_face {this->faces[ekg::io::font_face_type::emojis]};
  ekg::io::font_face_t &kanjis_font_face {this->faces[ekg::io::font_face_type::kanjis]};

  if (text_font_face.was_loaded) {
    FT_Done_Face(text_font_face.ft_face);
    text_font_face.was_loaded = false;
  }

  if (emojis_font_face.was_loaded) {
    FT_Done_Face(emojis_font_face.ft_face);
    emojis_font_face.was_loaded = false;
  }

  if (kanjis_font_face.was_loaded) {
    FT_Done_Face(kanjis_font_face.ft_face);
    kanjis_font_face.was_loaded = false;
  }
}

ekg::draw::font_renderer &ekg::draw::get_font_renderer(ekg::font font) {
  switch (font) {
  case ekg::font::small:
    return ekg::p_core->draw_fr_small;
  case ekg::font::big:
    return ekg::p_core->draw_fr_big;
  default:
    break;
  }

  return ekg::p_core->draw_fr_normal;
}
