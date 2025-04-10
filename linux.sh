#!/bin/bash
echo EKG building starting...

MODE_BUILD_AND_TEST="build-and-test"
MODE_BUILD_ONLY="build-only"
MODE_COMPILER_EMSCRIPTEN="emscripten"

echo -e "Building EKG in mode: $EKG_BUILD_MODE; with following compiler: $EKG_BUILD_COMPILER"

if [[ "$EKG_BUILD_MODE" ==  "$MODE_BUILD_AND_TEST" ]]; then

  cmake \
    -S . \
    -B ./cmake-build \
    -G Ninja \
    -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
    -D CMAKE_CXX_COMPILER=$EKG_BUILD_COMPILER \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=./ekg-cmake-install

  cmake \
    --build ./cmake-build \
  \
  && \
  \
  cmake \
    --install ./cmake-build \
    --prefix ./ekg-cmake-install \
  \
  && \
  \
  cd ./ekg-sandbox \
  \
  && \
  \
  cmake \
    -S . \
    -B ./cmake-build \
    -G Ninja \
    -D CMAKE_CXX_COMPILER=$EKG_BUILD_COMPILER \
    -D CMAKE_BUILD_TYPE=Release \
    -D EKG_DEVELOPER_MODE=1 \
  \
  && \
  \
  cmake \
    --build ./cmake-build \
    && cd ./bin && ./ekg-showcase
fi

if [[ "$EKG_BUILD_MODE" ==  "$MODE_BUILD_ONLY" ]]; then
  # #
  # Emscripten must be different compiled, due some specific flags
  # like 'EKG_EMSCRIPTEN_BUILD_TYPE'.
  # #
  if [[ "$EKG_BUILD_COMPILER" == "$MODE_COMPILER_EMSCRIPTEN" ]]; then
    emcmake cmake \
      -S . \
      -B ./cmake-build \
      -G Ninja \
      -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
      -D EKG_EMSCRIPTEN_BUILD_TYPE=1 \
      -D CMAKE_BUILD_TYPE=Release
  else
    cmake \
      -S . \
      -B ./cmake-build \
      -G Ninja \
      -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
      -D CMAKE_CXX_COMPILER=$EKG_BUILD_COMPILER \
      -D CMAKE_BUILD_TYPE=Release
  fi

  cmake \
    --build ./cmake-build
fi
