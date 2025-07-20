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
#ifndef EKG_IO_UTF_HPP
#define EKG_IO_UTF_HPP

#include "ekg/math/geometry.hpp"

#include <stdint.h>
#include <string>
#include <vector>

namespace ekg {
  /**
   * Returns a UTF string by `char32` converting
   * the UTF-32 unique char into a sequence of UTF-8
   * chars.
   */
  std::string utf32_to_string(
    char32_t char32
  );

  /**
   * Returns a string subtracted by stride begin `offset`
   * and end index `size`. If string is empty, return
   * empty.
   */
  std::string utf8_substr(
    std::string_view string,
    uint64_t offset,
    uint64_t size
  );

  /**
   * Returns the `string` length considering UTF chars.
   */
  uint64_t utf8_length(
    std::string_view string
  );

  /**
   * Returns index size that represent an UTF-8 char.
   * Possibles:
   * 3, 2, 1, and 0.
   */
  uint64_t utf8_check_sequence(
    uint8_t &char8,
    char32_t &char32,
    std::string &utf_string,
    std::string_view string,
    uint64_t index
  );

  /**
   * Returns a UTF-32 char32 based on UTF-8 sequence.
   */
  char32_t utf8_to_utf32(
    std::string_view string
  );

  /**
   * Fast splitter specialized in `\n` or `\r\n` (non OS unix-based).
   * UTF to sinalize the string unicode-like suggested by EKG. 
   */
  void utf8_split_new_line(
    std::string_view string,
    std::vector<std::string> &utf8_split_new_lined
  );

  /**
   * Return true if `string` contains `find_char`,
   * then it must allocate and insert elements to
   * `p_string_split_list` ptr.
   */
  bool utf8_split(
    std::vector<std::string> &string_split_list,
    const std::string &string,
    char find_char
  );

  template<typename t>
  void utf8_number_precision(
    std::string &number_to_string,
    t number,
    size_t precision
  ) {
    number_to_string = std::to_string(number);
    number_to_string = (
      number_to_string.substr(
        0,
        ekg::clamp_max<t>(
          number_to_string.find('.') + precision + (1 * precision),
          number_to_string.size()
        )
      )
    );  
  }
}

namespace ekg::io {
  typedef std::vector<std::string> chunk_t;
}

namespace ekg {
  class text {
  protected:
    std::vector<ekg::io::chunk_t> chunks {};
    size_t chunk_limit {10};
  protected:
    std::string &at_by_index(size_t index, bool swizzle = false) {
      size_t total_lines {};
      size_t chunks_size {this->chunks.size()};
      for (size_t it {}; it < chunks_size; it++) {
        chunk_t &chunk {this->chunks.at(it)};
        total_lines += chunk.size();

        if (swizzle && total_lines == index) {
          this
          break;
        }
      }
    }

    ekg::io::chunk_t &chunk_at(size_t chunk_index) {
      return this-
    }
  public:
    static std::string not_found;
  public:
    std::string &at(size_t index) {
      size_t chunks_size {this->chunks.size()};
      size_t total_lines {};
      size_t previous_total_lines {};
      size_t index_of {};
      size_t chunk_size {};

      for (size_t it {}; it < chunks_size; it++) {
        ekg::io::chunk_t &chunk {this->chunks.at(it)};

        previous_total_lines = total_lines;
        total_lines += (chunk_size = chunk.size());
        index_of = index - previous_total_lines;

        if (total_lines < index && index_of < chunk_size) {
          return chunk.at(index - previous_total_lines);
        }
      }
    }

    void push_back(const std::string &line) {
      if (this->chunks.empty()) {
        this->chunks.emplace_back().push_back(line);
        return;
      }

      size_t chunks_size {this->chunks.size()};
      ekg::io::chunk_t &chunk {this->chunks.at(chunks_size - (chunks_size != 0))};
      if (chunk.size() >= this->chunk_limit) {
        this->chunks.emplace_back().push_back(line);
        return;
      }

      this->chunks.emplace_back().push_back(line);
    }

    std::string &emplace_back() {
      if (this->chunks.empty()) {
        return this->chunks.emplace_back().emplace_back();
      }

      size_t chunks_size {this->chunks.size()};
      ekg::io::chunk_t &chunk {this->chunks.at(chunks_size - (chunks_size != 0))};
      if (chunk.size() >= this->chunk_limit) {
        return this->chunks.emplace_back().emplace_back();
      }

      return chunk.emplace_back();
    }
  };
}

#endif
