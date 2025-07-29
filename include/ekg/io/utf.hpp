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
   * @brief:
   *  check utf8 sequence and add to `it` index.
   *
   * @param `uc8` the current char to be checked.
   * @param `c32` the post utf32 char converted.
   * @param `utf8_str` the utf8 encoded text.
   * @param `it` the current `utf8_str` index.
   *
   * @description:
   *  this function check the first byte of a char utf8 encoded:
   *  if first char fit less than 127; 1 byte utf8 char.
   *  if first char fit equals 192~224; 2 byte utf8 char.
   *  if first char fit equals 224~240; 3 byte utf8 char.
   *  if first char fit equals 240~248; 4 byte utf8 char.
   * 
   *  by the amount of bytes per-char it is added to `it` e.g:
   *  if 224~240; then `it + 2`  (`it` already counts as 1 byte)
   **/
  void utf8_sequence(
    uint8_t &uc8,
    char32_t &c32,
    std::string_view utf8_str,
    size_t &it
  );

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

  /**   * Returns the `string` length considering UTF chars.
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
    std::vector<ekg::io::chunk_t> loaded_chunks {};
    size_t lines_per_chunk_limit {10};
    size_t total_chars {};
    size_t prev_total_chars {};
    bool was_audited {};
    bool should_count {};
  public:
    static std::string line_not_found;
  public:
    std::string read(size_t index);
    std::string &write(size_t index);

    void insert(
      size_t index,
      ekg::io::chunk_t &to_insert_chunk
    );

    void insert(
      size_t index,
      const std::string &line
    );

    void erase(
      size_t begin,
      size_t end
    );

    void push_back(const std::string &line);
    std::string &emplace_back();

    std::vector<ekg::io::chunk_t> &data();

    size_t size();
    size_t lines();
    size_t chunks();

    void unset_audited();
    bool audited();
  };
}

#endif
