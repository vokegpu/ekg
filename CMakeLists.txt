# MIT License
# 
# Copyright (c) 2022-2025 Rina Wilk / vokegpu@gmail.com
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.25)

include(cmake/io.cmake)
include(cmake/properties.cmake)

project(
  ekg
  VERSION ${EKG_VERSION}
  DESCRIPTION "Modern descriptor-based GPU-accelerated GUI library"
  LANGUAGES CXX
)

add_compile_options(
  ${COMPILE_OPTIMIZATION_NUMBER}
  #-Wall
  #-Wextra
  #-g
)

message(STATUS "Found compiler ID: ${CMAKE_CXX_COMPILER_ID}")

include_directories(./include)

# It is a common issue related by the Linux-community.
if (EKG_LINUX_NOT_FOUND_FREETYPE)
  include_directories(/usr/include/freetype2)
  message(
    STATUS
    "Using var EKG_LINUX_NOT_FOUND_FREETYPE to direct include `/usr/include/freetype2`"
  )
endif()

message(STATUS "Building EKG user interface library version ${EKG_VERSION}")
message(STATUS "Generating EKG ${PLATFORM} native library")

file(
  GLOB_RECURSE
  SOURCE_FILES
  "src/*.cpp"
)

exclude_sources_by_regex(SOURCE_FILES "/ui_|/util/")
add_library(ekg STATIC ${SOURCE_FILES})

set_target_properties(
  ekg
  PROPERTIES
  CXX_STANDARD 17
)

if(EKG_EMSCRIPTEN_BUILD_TYPE)
  set_target_properties(
    ekg
    PROPERTIES
    LINK_FLAGS -sALLOW_MEMORY_GROWTH=1
  )
endif()

message(STATUS "EKG native library building done")

if(LINUX OR EKG_FORCE_LINUX)
  install(
    TARGETS ekg
    LIBRARY DESTINATION /usr/local/lib
  )

  install(
    DIRECTORY ./include DESTINATION /usr/local
  )

  message(STATUS "EKG install on Linux ok!")
endif()
