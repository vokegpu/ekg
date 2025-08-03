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
#include "ekg/io/utf.hpp"

#include <cstring>
#include <sstream>
#include <ostream>

void ekg::utf8_sequence(
  uint8_t &uc8,
  char32_t &c32,
  std::string_view utf8_str,
  size_t &it
) {
  if (uc8 <= 0x7F) {
    c32 = static_cast<char32_t>(uc8);
  } else if ((uc8 & 0xE0) == 0xC0) {
    c32 = uc8 & 0x1F;
    c32 = (c32 << 6) | (utf8_str.at(++it) & 0x3F);
  } else if ((uc8 & 0xF0) == 0xE0) {
    c32 = uc8 & 0x0F;
    c32 = (c32 << 6) | (utf8_str.at(++it) & 0x3F);
    c32 = (c32 << 6) | (utf8_str.at(++it) & 0x3F);
  } else if ((uc8 & 0xF8) == 0xF0) {
    c32 = uc8 & 0x07;
    c32 = (c32 << 6) | (utf8_str.at(++it) & 0x3F);
    c32 = (c32 << 6) | (utf8_str.at(++it) & 0x3F);
    c32 = (c32 << 6) | (utf8_str.at(++it) & 0x3F);
  }
}

uint64_t ekg::utf8_check_sequence(
  uint8_t &char8,
  char32_t &char32,
  std::string &utf_string,
  std::string_view string,
  uint64_t index
) {
  if (char8 <= 0x7F) {
    utf_string = char8;
    char32 = static_cast<char32_t>(char8);
    return 0;
  } else if ((char8 & 0xE0) == 0xC0) {
    utf_string = string.substr(index, 2);
    char32 = ekg::utf8_to_utf32(utf_string);
    return 1;
  } else if ((char8 & 0xF0) == 0xE0) {
    utf_string = string.substr(index, 3);
    char32 = ekg::utf8_to_utf32(utf_string);
    return 2;
  } else if ((char8 & 0xF8) == 0xF0) {
    utf_string = string.substr(index, 4);
    char32 = ekg::utf8_to_utf32(utf_string);
    return 3;
  }

  return 0;
}

std::string ekg::utf32_to_string(char32_t char32) {
  std::string result {};

  if (char32 < 0x80) {
    result.reserve(1);
    result[0] = static_cast<char>(char32);
  } else if (char32 < 0x800) {
    result.reserve(2);
    result[0] += static_cast<char>(0xC0 | (char32 >> 6));
    result[1] += static_cast<char>(0x80 | (char32 & 0x3F));
  } else if (char32 < 0x10000) {
    result.reserve(3);
    result[0] += static_cast<char>(0xE0 | (char32 >> 12));
    result[1] += static_cast<char>(0x80 | ((char32 >> 6) & 0x3F));
    result[2] += static_cast<char>(0x80 | (char32 & 0x3F));
  } else if (char32 < 0x110000) {
    result.reserve(4);
    result[0] = static_cast<char>(0xF0 | (char32 >> 18));
    result[1] = static_cast<char>(0x80 | ((char32 >> 12) & 0x3F));
    result[2] = static_cast<char>(0x80 | ((char32 >> 6) & 0x3F));
    result[3] = static_cast<char>(0x80 | (char32 & 0x3F));
  }

  return result;
}

char32_t ekg::utf8_to_utf32(std::string_view string) {
  char32_t char32 {};

  uint64_t it {};
  uint8_t char8 {static_cast<uint8_t>(string.at(0))};

  if (char8 <= 0x7F) {
    char32 = char8;
  } else if (char8 <= 0xDF) {
    char32 = char8 & 0x1F;
    char32 = (char32 << 6) | (string.at(++it) & 0x3F);
  } else if (char8 <= 0xEF) {
    char32 = char8 & 0x0F;
    char32 = (char32 << 6) | (string.at(++it) & 0x3F);
    char32 = (char32 << 6) | (string.at(++it) & 0x3F);
  } else {
    char32 = char8 & 0x07;
    char32 = (char32 << 6) | (string.at(++it) & 0x3F);
    char32 = (char32 << 6) | (string.at(++it) & 0x3F);
    char32 = (char32 << 6) | (string.at(++it) & 0x3F);
  }

  return char32;
}

uint64_t ekg::utf8_length(std::string_view utf_string) {
  if (utf_string.empty()) {
    return 0;
  }

  uint64_t string_size {};
  uint8_t char8 {};

  for (uint64_t it {}; it < utf_string.size(); it++) {
    char8 = static_cast<uint8_t>(utf_string.at(it));
    if (char8 == '\n' || char8 == '\r') {
      continue;
    } else if (char8 <= 0x7F) {
      string_size++;
    } else if ((char8 & 0xE0) == 0xC0) {
      string_size++;
      it++;
    } else if ((char8 & 0xF0) == 0xE0) {
      string_size++;
      it += 2;
    } else if ((char8 & 0xF8) == 0xF0) {
      string_size++;
      it += 3;
    }
  }

  return string_size;
}

std::string ekg::utf8_substr(std::string_view string, uint64_t offset, uint64_t size) {
  if (string.empty() || size == 0) {
    return "";
  }

  uint64_t string_size {string.size()};
  uint64_t utf_text_size {};       

  //offset = offset > string_size ? string_size : offset;
  size += offset;

  uint64_t index {};
  uint64_t utf_sequence_size {};
  uint64_t begin {UINT64_MAX};
  uint8_t char8 {};
  bool at_last_index {};

  /*
   * This function implementation checks the amount of bytes per char for UTF-8.
   * There is no support for UTF-16 or directly UTF-32.
   */

  while (index < string_size) {
    char8 = static_cast<uint8_t>(string.at(index));

    utf_sequence_size = 1;
    utf_sequence_size += ((char8 & 0xE0) == 0xC0);
    utf_sequence_size += 2 * ((char8 & 0xF0) == 0xE0);
    utf_sequence_size += 3 * ((char8 & 0xF8) == 0xF0);

    at_last_index = index + utf_sequence_size == string_size;

    if ((at_last_index || utf_text_size >= offset) && begin == UINT64_MAX) {
      begin = ekg::clamp_max(index, string_size);
    }

    index += utf_sequence_size;
    utf_text_size++;

    if (
        /**
         * OBS:
         *  If the `offset` parameter is equals to the last UTF-8 string size,
         *  then it continue without substring process.
         **/
        (at_last_index && begin != UINT64_MAX && offset != utf_text_size) ||
        (utf_text_size >= size)
       ) {
      string = string.substr(begin, (index - begin));
      return std::string {string.begin(), string.end()};
    }
  }

  return "";
}

size_t ekg::utf8_split_endings(
  std::string_view line,
  std::vector<std::string> &splitted
) {
  if (line.empty()) {
    return 0;
  }

  size_t new_lines_count {};
  size_t str_size {line.size()};
  size_t start_pos {};
  size_t pos {};

  while ((pos = line.find('\n', start_pos)) != std::string::npos) {
    std::string &split {splitted.emplace_back()};
    split = line.substr(start_pos, pos - start_pos);
    if (split.back() == '\r') split.pop_back();
    start_pos = pos + 1;
    new_lines_count++;
  }
  
  if (start_pos < str_size) {
    splitted.emplace_back() = line.substr(start_pos, str_size);
    new_lines_count++;
  }

  return new_lines_count;
}

void ekg::text::swizzle(
  size_t chunk_index,
  size_t line_index,
  std::vector<std::string> &to_swizzle,
  bool skip_first_line
) {
  ekg::io::chunk_t &chunk {this->loaded_chunks.at(chunk_index)};
  if (skip_first_line) {
    chunk.at(line_index) = to_swizzle.at(0);
  }

  chunk.insert(
    chunk.begin() + line_index + 1,
    to_swizzle.begin() + skip_first_line,
    to_swizzle.end()
  );

  if (chunk.size() < this->lines_per_chunk_limit) {
    return;
  }

  std::vector<std::string> to_swizzle_chunk {
    chunk.begin() + this->lines_per_chunk_limit,
    chunk.end()
  };

  chunk.erase(
    chunk.begin() + this->lines_per_chunk_limit,
    chunk.end()
  );

  size_t to_swizzle_chunk_size {to_swizzle_chunk.size()};
  size_t newly_chunks {
    to_swizzle_chunk_size / this->lines_per_chunk_limit          
  };

  for (size_t jt {}; jt < newly_chunks; jt++) {
    this->loaded_chunks.insert(
      this->loaded_chunks.begin() + chunk_index + jt + 1,
      ekg::io::chunk_t {
        to_swizzle_chunk.begin() + (this->lines_per_chunk_limit * (jt + 0)),
        to_swizzle_chunk.begin() + (this->lines_per_chunk_limit * (jt + 1))
      }
    );
  }

  size_t rest {to_swizzle_chunk_size - (this->lines_per_chunk_limit * newly_chunks)};
  if (rest > 0) {
    this->loaded_chunks.insert(
      this->loaded_chunks.begin() + chunk_index + newly_chunks + 1,
      ekg::io::chunk_t {
        to_swizzle_chunk.begin() + (this->lines_per_chunk_limit * (newly_chunks + 0)),
        to_swizzle_chunk.end()
      }
    );
  }
}

void ekg::text::set(size_t index, std::string_view line) {
  size_t current_lines {};
  size_t previous_lines {};
  size_t chunk_size {};

  std::vector<std::string> ending_splitted {};
  ekg::utf8_split_endings(line, ending_splitted);

  bool ok {};
  for (size_t it {}; it < this->loaded_chunks.size(); it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

    previous_lines = current_lines;
    current_lines += (chunk_size = chunk.size());

    if (
      !ok
      &&
      index < current_lines
      &&
      (index - previous_lines) < chunk_size
    ) {
      this->swizzle(it, (index - previous_lines), ending_splitted, true);
      this->was_audited = true;
      ok = true;
    }
  }

  if (!ok) throw std::out_of_range("ekg::text::set -> lines length: " + std::to_string(current_lines));
  this->total_lines = current_lines;
}

std::string ekg::text::at(size_t index) {
  size_t chunks_size {this->loaded_chunks.size()};
  size_t current_lines {};
  size_t previous_lines {};
  size_t chunk_size {};

  for (size_t it {}; it < chunks_size; it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};
    previous_lines = current_lines;
    current_lines += (chunk_size = chunk.size());

    if (
      index < current_lines
      &&
      (index - previous_lines) < chunk_size
    ) {
      return chunk.at(index - previous_lines);
    }
  }

  throw std::out_of_range("ekg::text::at -> lines length: " + std::to_string(current_lines));
}

void ekg::text::insert(
  size_t index,
  const ekg::io::chunk_t &to_insert_chunk
) {
  if (this->loaded_chunks.empty()) {
    if (index == 0) {
      this->loaded_chunks.emplace_back();
      this->insert(index, to_insert_chunk);
    }
    return;
  }

  size_t previous_lines {};
  size_t chunk_size {};
  size_t current_lines {};
  size_t to_insert_chunk_size {to_insert_chunk.size()};

  ekg::io::chunk_t splitted {};
  for (size_t it {}; it < to_insert_chunk_size; it++) {
    const std::string &lines {to_insert_chunk.at(it)};
    ekg::utf8_split_endings(lines, splitted);
  }

  for (size_t it {}; it < this->loaded_chunks.size(); it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

    previous_lines = current_lines;
    current_lines += (chunk_size = chunk.size());

    if (
      index < current_lines
      &&
      (index - previous_lines) < chunk_size
    ) {
      this->swizzle(it, (index - previous_lines), splitted, false);
      this->was_audited = true;
      this->should_count = true;
      this->total_lines += splitted.size();
      return;
    }
  }

  throw std::out_of_range("ekg::text::insert -> lines length: " + std::to_string(current_lines));
}

void ekg::text::insert(
  size_t index,
  std::string_view line
) {
  ekg::io::chunk_t chunk {};
  chunk.push_back(std::string(line));
  this->insert(index, chunk);
}

void ekg::text::erase(
  size_t begin,
  size_t end
) {
  if (this->loaded_chunks.empty()) {
    throw std::out_of_range("ekg::text::erase -> text empty");
    return;
  }

  if (begin > this->total_lines) {
    throw std::out_of_range("ekg::text::erase: " + std::to_string(begin) + ", " + std::to_string(this->total_lines));
    return;
  }

  end = ekg::min(begin + end, this->total_lines);
  this->total_lines -= end - begin;

  this->was_audited = true;
  this->should_count = true;

  size_t previous_lines {};
  size_t chunk_size {};
  size_t lines {};
  size_t remains_lines {};

  bool empty_chunk {};
  bool goto_next_chunk {};

  for (size_t it {}; it < this->loaded_chunks.size(); it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

    previous_lines = lines;
    lines += (chunk_size = chunk.size());

    if (begin < lines) {
      remains_lines = end - begin;
      begin = begin - previous_lines;
      while (remains_lines != 0) {
        ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

        chunk_size = chunk.size();
        goto_next_chunk = begin + remains_lines > chunk.size();

        chunk.erase(
          chunk.begin() + begin,
          chunk.begin() + begin + (!goto_next_chunk * remains_lines)
        );

        empty_chunk = chunk.empty();
        if (empty_chunk) {
          this->loaded_chunks.erase(
            this->loaded_chunks.begin() + it
          );
        }

        if (goto_next_chunk) {
          it += !empty_chunk;
          remains_lines -= chunk_size - begin;
          begin = 0;
          continue;
        }

        remains_lines = 0; 
      }

      break;
    }
  }
}

void ekg::text::push_back(std::string_view line) {
  ekg::io::chunk_t splitted {};
  ekg::utf8_split_endings(line, splitted);

  this->should_count = true;
  this->total_lines += splitted.size();

  if (this->loaded_chunks.empty()) {
    this->loaded_chunks.emplace_back().emplace_back();
    this->swizzle(0, 0, splitted, true);
    return;
  }

  ekg::io::chunk_t &last_chunk {
    this->loaded_chunks.back()
  };

  this->swizzle(
    this->loaded_chunks.size() - 1,
    last_chunk.size() - !last_chunk.empty(),
    splitted,
    true
  );
}

std::vector<ekg::io::chunk_t> &ekg::text::chunks_data() {
  return this->loaded_chunks;
}

size_t ekg::text::length_of_chunks() {
  return this->loaded_chunks.size();
}

size_t ekg::text::length_of_lines() {
  return this->total_lines;
}

size_t ekg::text::length_of_chars() {
  if (this->should_count) {
    this->total_chars = 0;
    for (ekg::io::chunk_t &chunk : this->loaded_chunks) {
      for (std::string &lines : chunk) {
        this->total_chars += ekg::utf8_length(lines);
      }
    }

    this->should_count = false;
  }

  return this->total_chars;
}

bool ekg::text::audited() {
  this->was_audited = this->was_audited || (this->prev_lines != this->total_lines);
  this->prev_lines = this->total_lines;
  bool was_audited {this->was_audited};
  this->was_audited = false;
  return was_audited;
}
