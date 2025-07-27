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

void ekg::utf8_split_new_line(std::string_view string, std::vector<std::string> &utf8_read) {
  if (string.empty()) {
    return;
  }

  /* if the first element is empty, then the decode must start from it */
  if (utf8_read.size() == 1 && utf8_read.at(0).empty()) {
    utf8_read.clear();
  }

  uint64_t index {};
  uint64_t start_index {};

  while ((index = string.find('\n', start_index)) != std::string_view::npos) {
    utf8_read.emplace_back(
      string.substr(
        start_index,
        (index - start_index) - (index > 0 && string.at(index - 1) == '\r')
      )
    );

    start_index = index + 1;
  }

  if (!string.empty() &&
      !(
        string = string.substr(start_index, string.find('\0', start_index))
      ).empty()
    ) {
    // `meow\n\0`, so the decode wont emplace a empty string.
    utf8_read.emplace_back(string);
  }
}

bool ekg::utf8_split(
  std::vector<std::string> &splitted,
  const std::string &string,
  char find_char
) {
  std::stringstream ss(string);
  std::string find_string {};

  bool found_flag {};

  while (std::getline(ss, find_string, find_char)) {
    splitted.push_back(find_string);
    found_flag = true;
  }

  return found_flag;
}

std::string ekg::text::line_not_found {'\x1B'};

std::string &ekg::text::write(size_t index) {
  size_t chunks_size {this->loaded_chunks.size()};
  size_t total_lines {};
  size_t previous_total_lines {};
  size_t chunk_size {};

  this->was_audited = true;

  for (size_t it {}; it < chunks_size; it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

    previous_total_lines = total_lines;
    total_lines += (chunk_size = chunk.size());

    if (index < total_lines && (index - previous_total_lines) < chunk_size) {
      return chunk.at(index - previous_total_lines);
    }
  }

  return ekg::text::line_not_found;
}

std::string ekg::text::read(size_t index) {
  bool was_audited_before {this->was_audited};
  std::string &line {this->write(index)};
  this->was_audited = was_audited_before;
  return line;
}

void ekg::text::insert(
  size_t index,
  ekg::io::chunk_t &to_insert_chunk
) {
  if (this->loaded_chunks.empty()) {
    if (index == 0) {
      this->loaded_chunks.emplace_back();
      this->insert(index, to_insert_chunk);
    }
    return;
  }

  bool ok {};

  size_t previous_total_lines {};
  size_t chunk_size {};
  size_t current_total_lines {};

  ekg::io::chunk_t to_swizzle_chunk {};

  this->total_lines = 0;
  for (size_t it {}; it < this->loaded_chunks.size(); it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

    previous_total_lines = this->total_lines;
    current_total_lines += (chunk_size = chunk.size());

    if (
      !ok
      &&
      index < current_total_lines
      &&
      (index - previous_total_lines) < chunk_size
    ) {
      chunk.insert(
        chunk.begin() + (index - previous_total_lines),
        to_insert_chunk.begin(),
        to_insert_chunk.end()
      );

      if (chunk.size() < this->lines_per_chunk_limit) {
        ok = true;
        continue;
      }

      to_swizzle_chunk.insert(
        to_swizzle_chunk.begin(),
        chunk.begin() + this->lines_per_chunk_limit,
        chunk.end()
      );

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
          this->loaded_chunks.begin() + it + jt + 1,
          ekg::io::chunk_t {
            to_swizzle_chunk.begin() + (this->lines_per_chunk_limit * (jt + 0)),
            to_swizzle_chunk.begin() + (this->lines_per_chunk_limit * (jt + 1))
          }
        );
      }

      size_t rest {to_swizzle_chunk_size - (this->lines_per_chunk_limit * newly_chunks)};
      if (rest > 0) {
        this->loaded_chunks.insert(
          this->loaded_chunks.begin() + it + newly_chunks + 1,
          ekg::io::chunk_t {
            to_swizzle_chunk.begin() + (this->lines_per_chunk_limit * (newly_chunks + 0)),
            to_swizzle_chunk.end()
          }
        );
      }

      ok = true;
    }

    this->total_lines += this->loaded_chunks.at(it).size();
  }
}

void ekg::text::insert(
  size_t index,
  const std::string &line
) {
  ekg::io::chunk_t chunk {};
  chunk.push_back(line);
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

  if (begin >= this->total_lines || end >= this->total_lines || end <= begin) {
    throw std::out_of_range("ekg::text::erase -> begin or end out of index");
    return;
  }

  this->total_lines -= end - begin;

  size_t previous_total_lines {};
  size_t chunk_size {};
  size_t total_lines {};
  size_t remains_lines {};

  bool empty_chunk {};
  bool goto_next_chunk {};

  for (size_t it {}; it < this->loaded_chunks.size(); it++) {
    ekg::io::chunk_t &chunk {this->loaded_chunks.at(it)};

    previous_total_lines = total_lines;
    total_lines += (chunk_size = chunk.size());

    if (begin < total_lines) {
      remains_lines = end - begin;
      begin = begin - previous_total_lines;
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

void ekg::text::push_back(const std::string &line) {
  if (this->loaded_chunks.empty()) {
    this->loaded_chunks.emplace_back().push_back(line);
    return;
  }

  this->total_lines++;

  size_t chunks_size {this->loaded_chunks.size()};
  ekg::io::chunk_t &chunk {this->loaded_chunks.at(chunks_size - (chunks_size != 0))};
  if (chunk.size() >= this->lines_per_chunk_limit) {
    this->loaded_chunks.emplace_back().push_back(line);
    return;
  }

  this->loaded_chunks.emplace_back().push_back(line);
}

std::string &ekg::text::emplace_back() {
  if (this->loaded_chunks.empty()) {
    return this->loaded_chunks.emplace_back().emplace_back();
  }

  this->total_lines++;

  size_t chunks_size {this->loaded_chunks.size()};
  ekg::io::chunk_t &chunk {this->loaded_chunks.at(chunks_size - (chunks_size != 0))};
  if (chunk.size() >= this->lines_per_chunk_limit) {
    return this->loaded_chunks.emplace_back().emplace_back();
  }

  return chunk.emplace_back();
}

size_t ekg::text::lines() {
  return this->total_lines;
}

size_t ekg::text::chunks() {
  return this->loaded_chunks.size();
}

bool ekg::text::audited() {
  bool should {this->was_audited};
  this->was_audited = false;
  return should;
}

std::vector<ekg::io::chunk_t> &ekg::text::data() {
  return this->loaded_chunks;
}
