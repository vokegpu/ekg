/**
 * MIT License
 * 
 * Copyright (c) 2022-2025 Rina Wilk / vokegpu@gmail.com
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
#include "ekg/ui/scrollbar/widget.hpp"
#include "ekg/ui/textbox/widget.hpp"
#include "ekg/layout/docknize.hpp"
#include "ekg/io/log.hpp"
#include "ekg/ui/abstract.hpp"
#include "ekg/draw/shape/shape.hpp"
#include "ekg/draw/typography/font.hpp"
#include "ekg/core/context.hpp"
#include "ekg/core/runtime.hpp"
#include "ekg/core/pools.hpp"
#include "ekg/math/floating_point.hpp"

bool ekg::ui::find_cursor(
  ekg::textbox_t &textbox,
  ekg::vec2_t<size_t> &index,
  ekg::textbox_t::cursor_t &cursor_out
) {
  for (ekg::textbox_t::cursor_t &cursor : textbox.widget.cursors) {
    if (cursor >= index && cursor <= index) {
      cursor_out = cursor;
      return true;
    }
  }

  return false;
}

bool ekg::ui::find_index_by_interact(
  ekg::property_t &property,
  ekg::textbox_t &textbox,
  ekg::vec2_t<float> &interact,
  size_t &index
) {
  ekg::rect_t<float> &rect_abs {
    ekg::ui::get_abs_rect(property, textbox.rect)
  };
}

void ekg::ui::reload(
  ekg::property_t &property,
  ekg::textbox_t &textbox
) {
  ekg::axis pick_axis {
    ekg::axis::horizontal
  };

  ekg::draw::font &draw_font {
    ekg::draw::get_font_renderer(textbox.font_size)
  };

  textbox.widget.rect_text_size.h = draw_font.get_text_height();
  textbox.widget.rect_text_size.w = textbox.widget.rect_text_size.h;

  ekg::aligned_t aligned_dimension {};
  ekg::align_rect_dimension(
    pick_axis,
    textbox.widget.rect_text_size,
    ekg::dpi.min_sizes,
    aligned_dimension
  );

  textbox.rect.scaled_height = ekg::max<ekg::pixel_thickness_t>(1, textbox.rect.scaled_height);
  textbox.rect.h = aligned_dimension.h * textbox.rect.scaled_height;

  size_t highest_size {};
  std::vector<ekg::io::chunk_t> &chunks {textbox.text.chunks_data()};

  if (textbox.text.audited()) {
    for (ekg::io::chunk_t &chunk : chunks) {
      for (std::string &line : chunk) {
        highest_size = ekg::max(line.size(), highest_size);
      }
    };

    property.widget.should_buffering = true;
  }

  ekg::rect_t<float> &rect_abs {
    ekg::ui::get_abs_rect(property, textbox.rect)
  };

  textbox.widget.scrollbar.rect.x = rect_abs.x;
  textbox.widget.scrollbar.rect.y = rect_abs.y;
  textbox.widget.scrollbar.rect.w = highest_size * draw_font.text_height;
  textbox.widget.scrollbar.acceleration = {draw_font.text_height, draw_font.text_height};

  textbox.widget.scrollbar.color_scheme = ekg::p_core->handler_theme.get_current_theme().scrollbar_color_scheme;

  ekg::ui::reload(
    textbox.widget.scrollbar_property,
    textbox.widget.scrollbar,
    rect_abs,
    property.children,
    false
  );
}

void ekg::ui::event(
  ekg::property_t &property,
  ekg::textbox_t &textbox,
  const ekg::io::stage &stage
) {
  ekg::property_t &scrollbar_property {
    textbox.widget.scrollbar_property
  };

  scrollbar_property.widget.rect_scissor = property.widget.rect_scissor;

  ekg::ui::event(
    scrollbar_property,
    textbox.widget.scrollbar,
    stage,
    ekg::ui::get_abs_rect(property, textbox.rect)
  );

  property.widget.should_buffering = scrollbar_property.widget.should_buffering;
  property.states.is_absolute = scrollbar_property.states.is_absolute;
  property.scroll = scrollbar_property.scroll;

  switch (stage) {
    default: {
      ekg::input_info_t &input {ekg::p_core->handler_input.input};
      ekg::vec2_t<float> interact {static_cast<ekg::vec2_t<float>>(input.interact)};

      bool should_enable_high_frequency {
        property.states.is_hovering
        ||
        property.states.is_focused
        ||
        scrollbar_property.widget.is_high_frequency
      };

      if (
        !property.states.is_focused
        &&
        property.states.is_hovering
        &&
        ekg::fire("textbox-focus")
      ) {
        should_enable_high_frequency = true;
        property.states.is_focused = true;

        if (textbox.widget.cursors.empty()) {

        }
      }

      if (
        property.states.is_focused
        &&
        !property.states.is_hovering
        &&
        input.was_pressed
        &&
        !input.was_typed
      ) {
        property.states.is_focused = false;
        should_enable_high_frequency = false;
      }

      if (
        should_enable_high_frequency
      ) {
        ekg::io::dispatch(
          ekg::io::operation::high_frequency,
          property.at
        );
      }

      break;
    }
  case ekg::io::stage::pre:
    ekg::ui::pre_event(property, textbox.rect, false);
    break;
  case ekg::io::stage::post:
    ekg::ui::post_event(property);
    break;
  }
}

void ekg::ui::high_frequency(
  ekg::property_t &property,
  ekg::textbox_t &textbox
) {
  ekg::rect_t<float> &rect_abs {
    ekg::ui::get_abs_rect(property, textbox.rect)
  };

  ekg::ui::high_frequency(
    textbox.widget.scrollbar_property,
    textbox.widget.scrollbar,
    rect_abs
  );

  ekg::gui.ui.redraw = true;
  property.widget.should_buffering = true;
  property.widget.is_high_frequency = (
    property.states.is_focused
    ||
    textbox.widget.scrollbar_property.widget.is_high_frequency
  );
}

void ekg::ui::pass(
  ekg::property_t &property,
  ekg::textbox_t &textbox
) {
  ekg_draw_allocator_bind_local(
    &property.widget.geometry_buffer,
    &property.widget.gpu_data_buffer
  );

  if (property.widget.should_buffering || !textbox.text.audited()) {
    property.widget.should_buffering = true;
    return;
  }

  ekg_draw_allocator_pass();
}

void ekg::ui::buffering(
  ekg::property_t &property,
  ekg::textbox_t &textbox
) {
  ekg::rect_t<float> &rect_abs {
    ekg::ui::get_abs_rect(property, textbox.rect)
  };

  ekg_draw_allocator_assert_scissor(
    property.widget.rect_scissor,
    rect_abs,
    ekg::query<ekg::property_t>(property.parent_at).widget.rect_scissor,
    ekg::always_parented
  );

  textbox.text.unset_audited();

  /* start of background */

  ekg::draw::rect(
    rect_abs,
    textbox.color_scheme.background,
    ekg::draw::mode::fill,
    textbox.layers[ekg::layer::bg]
  );

  /* start of select */

  ekg::vec2_t<float> scrollable_pos {};

  for (ekg::textbox_t::select_draw_layer_t &layer : textbox.widget.layers_select) {
    if (layer.is_ab_equals && ekg::timing_t::second > 500) {
      continue;
    }

    ekg::draw::rect(
      layer.rect + rect_abs + textbox.widget.scrollbar_property.scroll.position,
      layer.is_ab_equals ? textbox.color_scheme.text_cursor_foreground : textbox.color_scheme.text_select_foreground,
      ekg::draw::mode::fill,
      ekg::at_t::not_found
    );
  }

  /* start of text */

  ekg::draw::font &draw_font {
    ekg::draw::get_font_renderer(textbox.font_size)
  };

  ekg::gpu::data_t &data {
    ekg::p_core->draw_allocator.bind_current_data()
  };

  ekg::vec2_t<float> pos {
    static_cast<float>(static_cast<int32_t>(rect_abs.x + textbox.color_scheme.gutter_margin)) + textbox.widget.scrollbar_property.scroll.position.x,
    floorf(static_cast<float>(static_cast<int32_t>(rect_abs.y - draw_font.offset_text_height * 2)))
  };

  data.buffer[0] = pos.x;
  data.buffer[1] = pos.y;
  data.buffer[2] = static_cast<float>(-draw_font.non_swizzlable_range);

  data.buffer[4] = static_cast<float>(textbox.color_scheme.text_foreground.x / 255);
  data.buffer[5] = static_cast<float>(textbox.color_scheme.text_foreground.y / 255);
  data.buffer[6] = static_cast<float>(textbox.color_scheme.text_foreground.z / 255);
  data.buffer[7] = static_cast<float>(textbox.color_scheme.text_foreground.w / 255);

  ekg::hash_t hash {};
  ekg::rect_t<float> vertex {};
  ekg::rect_t<float> uv {};
  ekg::rect_t<float> rendered_size {};

  size_t text_len {};
  size_t utf8_text_len {};
  uint8_t uc8 {};
  char32_t c32 {};

  bool break_text {};
  bool r_n_break_text {};

  FT_Face ft_face {};
  FT_Vector ft_vector_previous_char {};
  char32_t ft_uint_previous {};

  ekg::io::font_face_t &text_font_face {draw_font.faces[ekg::io::font_face_type::text]};
  ekg::io::font_face_t &emojis_font_face {draw_font.faces[ekg::io::font_face_type::emojis]};
  ekg::io::font_face_t &kanjis_font_face {draw_font.faces[ekg::io::font_face_type::kanjis]};

  bool is_inline_selected {};
  bool is_complete_line_selected {};
  bool is_ab_equals_selected {};
  bool was_complete_line_selected {};
  bool cursors_going_on {!textbox.widget.cursors.empty()};
  bool is_end_of_line_and_text {};
  bool oka_found_visual_index {};
  bool get_out {};

  size_t current_line_for_cursor_complete {UINT64_MAX};
  size_t text_total_chars {textbox.text.length_of_chars()};
  size_t text_total_lines {textbox.text.length_of_lines()};
  size_t ij {};
  size_t addition_chunk_index {};
  size_t chunk_size {};
  ekg::vec2_t<size_t> index {};

  ekg::pixel_t line_wsize {};
  ekg::textbox_t::cursor_t cursor {};
  std::vector<ekg::io::chunk_t> &chunks {textbox.text.chunks_data()};

  textbox.widget.scrollbar.rect.h = text_total_lines * draw_font.text_height;

  size_t visual_target_line_index {
    static_cast<size_t>(
      static_cast<float>(
        -(
          textbox.widget.scrollbar_property.scroll.position.y
          /
          textbox.widget.scrollbar.rect.h
        )
      )
      *
      text_total_lines
    )
  };

  /**
   * This technique prevent from floating-point precision loss inside GPU.
   * 
   * Think that you are scrolling a 1milion text, if you subtract the scroll-position (1 milion)
   * with the text-content, for do scrolling effect, this will send 1000000.0f to the GPU.
   * Some GPUs can not handle high floating numbers, because matrix projection calculate
   * is too heavy to a large float32.
   * 
   * This method send to the GPU: from 0.0 to textbox-height. The scrolling effect happens but
   * without sending 1 milion of f32 to the GPU. The effect is emulated by scrolling only the first
   * line: from 0.0 to negative text-height; so the effect occurs without the needs of stupid
   * scrolling everything.
   * 
   * This technique works because the line height is fixed, ultimately, soon, should be re-worked
   * to support differents text-heights at same time, also, for widgets scrolling (I do not think
   * someone can write a GUI context with +5000000 heights from widgets without pages).
   * 
   * - Rina - 11:39; 08/06/2025
   **/
  float visible_text_height {static_cast<float>(visual_target_line_index * draw_font.text_height)};
  pos.y = textbox.widget.scrollbar_property.scroll.position.y + visible_text_height;
  pos.y = static_cast<float>(static_cast<int32_t>(floorf(pos.y)));
  pos.x = 0.0f;

  textbox.widget.layers_select.clear();

  for (ekg::io::chunk_t &chunk : chunks) {
    chunk_size = chunk.size();
    if (addition_chunk_index + chunk_size < visual_target_line_index) {
      pos.x = 0.0f;
      index.y += chunk_size;
      addition_chunk_index += chunk_size;
      continue;
    }

    ij = 0;
    if (!oka_found_visual_index) {
      ij = visual_target_line_index - addition_chunk_index;
      index.y += ij;
      oka_found_visual_index = true;
    }

    addition_chunk_index += chunk_size;
    for (;ij < chunk_size; ij++) {
      std::string &line {chunk.at(ij)};
      index.x = 0;
      text_len = line.size();
      for (size_t it {}; it < text_len; it++) {
        uc8 = static_cast<uint8_t>(line.at(it));

        ekg::utf8_sequence(
          uc8,
          c32,
          line,
          it
        );

        if (draw_font.ft_bool_kerning && ft_uint_previous) {
          switch (c32 < 256 || !emojis_font_face.was_loaded) {
            case true: {
              ft_face = text_font_face.ft_face;
              break;
            }

            default: {
              ft_face = emojis_font_face.ft_face;
              break;
            }
          }

          FT_Get_Kerning(ft_face, ft_uint_previous, c32, 0, &ft_vector_previous_char);
          pos.x += static_cast<float>(ft_vector_previous_char.x >> 6);
        }

        ekg::io::glyph_t &glyph {draw_font.mapped_glyph[c32]};

        if (
          cursors_going_on
          &&
          ekg::ui::find_cursor(textbox, index, cursor)
        ) {
          is_ab_equals_selected = property.states.is_focused && cursor == index;
          is_inline_selected = cursor >= index && cursor < index && !is_ab_equals_selected;
          is_complete_line_selected = false;

          if (is_inline_selected) {
            if (
              (
                cursor >= ekg::vec2_t<size_t>(0, index.y)
              )
              &&
              (
                cursor <= ekg::vec2_t<size_t>(text_len, index.y)
              )
            ) {
              is_inline_selected = false;
            }
            is_complete_line_selected = !is_inline_selected;
          }

          if (is_ab_equals_selected) {
            cursor.rect.x = pos.x;
            cursor.rect.y = pos.y;
            cursor.rect.w = textbox.color_scheme.caret_cursor ? glyph.wsize : textbox.color_scheme.cursor_thickness;
            cursor.rect.h = draw_font.text_height;
            textbox.widget.layers_select.push_back({true, cursor.rect});
          }

          if (is_inline_selected) {
            cursor.rect.x = pos.x;
            cursor.rect.y = pos.y;
            cursor.rect.w = glyph.wsize;
            cursor.rect.h = draw_font.text_height;
            textbox.widget.layers_select.push_back({false, cursor.rect});
          }

          if (
            was_complete_line_selected
            &&
            (
              (current_line_for_cursor_complete != index.y)
              || 
              (is_end_of_line_and_text = index.y + 1 == text_total_lines && it + 1 == text_len)
            )
          ) {
            cursor.rect.x = 0.0f;
            cursor.rect.y = pos.y - (draw_font.text_height * !is_end_of_line_and_text);
            cursor.rect.w = line_wsize + (glyph.wsize * is_end_of_line_and_text);
            cursor.rect.h = draw_font.text_height;
            textbox.widget.layers_select.push_back({false, cursor.rect});
            was_complete_line_selected = false;            
          }

          if (is_complete_line_selected && current_line_for_cursor_complete != index.y) {
            line_wsize = 0.0f;
            current_line_for_cursor_complete = index.y;
            was_complete_line_selected = true;
          }

          if (is_complete_line_selected) {
            line_wsize += glyph.wsize;
          }
        }

        if (!glyph.was_sampled) {
          draw_font.new_glyphs_to_atlas.emplace_back(c32);
          glyph.was_sampled = true;
        }

        vertex.x = pos.x + glyph.left;
        vertex.y = pos.y + draw_font.atlas_rect.h - glyph.top;

        vertex.w = glyph.w;
        vertex.h = glyph.h;

        uv.x = glyph.x;
        uv.w = vertex.w / draw_font.atlas_rect.w;
        uv.h = vertex.h / draw_font.atlas_rect.h;

        ekg::p_core->draw_allocator.push_back_geometry(
          vertex.x,
          vertex.y,
          uv.x,
          uv.y
        );

        ekg::p_core->draw_allocator.push_back_geometry(
          vertex.x,
          vertex.y + vertex.h,
          uv.x,
          uv.y + uv.h
        );

        ekg::p_core->draw_allocator.push_back_geometry(
          vertex.x + vertex.w,
          vertex.y + vertex.h,
          uv.x + uv.w,
          uv.y + uv.h
        );

        ekg::p_core->draw_allocator.push_back_geometry(
          vertex.x + vertex.w,
          vertex.y + vertex.h,
          uv.x + uv.w,
          uv.y + uv.h
        );

        ekg::p_core->draw_allocator.push_back_geometry(
          vertex.x + vertex.w,
          vertex.y,
          uv.x + uv.w,
          uv.y
        );

        ekg::p_core->draw_allocator.push_back_geometry(
          vertex.x,
          vertex.y,
          uv.x,
          uv.y
        );

        pos.x += glyph.wsize;
        ft_uint_previous = c32;

        /**
         * Peek `ekg/io/memory.hpp` for better hash definition and purpose.
         **/
        hash += ekg_generate_hash(pos.x, c32, glyph.x);
        index.x++;
      }

      pos.x = 0.0f;
      pos.y += draw_font.text_height;
      rendered_size.y += draw_font.text_height;
      index.y++;
      hash += pos.y * 32;
    
      if (rendered_size.y > rect_abs.h) {
        get_out = true;
        break;
      }
    }

    if (get_out) {
      break;
    }
  }

  data.hash = hash;

  ekg::draw::allocator::is_simple_shape = false;
  ekg::p_core->draw_allocator.bind_texture(draw_font.atlas_texture_sampler_at);
  ekg::p_core->draw_allocator.dispatch();

  /* start of scrollbar */

  textbox.widget.scrollbar.rect.x = rect_abs.x;
  textbox.widget.scrollbar.rect.y = rect_abs.y;

  ekg::ui::buffering(
    textbox.widget.scrollbar_property,
    textbox.widget.scrollbar,
    rect_abs,
    ekg::query<ekg::property_t>(property.parent_at).widget.rect_scissor
  );

  /* start of outline */

  ekg::draw::rect(
    rect_abs,
    textbox.color_scheme.outline,
    ekg::draw::mode::outline,
    ekg::at_t::not_found
  );

  /* start of final tweaks */

  /**
   * Cursors should be rendered backward from text-render.
   * Textbox first try-render render the cursors, then at end of renderer-segment
   * check if latest size is diff, then re-call buffering once for render
   * the cursos (backward).
   * 
   * The EKG renderer engine does not allow doing strip on gpu resources (not yet).
   * May soon strip-resources should be implemented and this code part become deprecated.  
   **/
  size_t layers_select_size {textbox.widget.layers_select.size()};
  if (textbox.widget.last_layers_select_size != layers_select_size) {
    property.widget.should_buffering = true;
    textbox.widget.last_layers_select_size = layers_select_size;
    ekg::gui.ui.redraw = true;
  }

  /* end of pass */

  ekg_draw_allocator_pass();
}

void ekg::ui::unmap(
  ekg::textbox_t &textbox
) {

}
