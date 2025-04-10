FROM ubuntu:24.04
# Install Needs
RUN apt update && \
apt upgrade -y && \
apt install -y libsdl2-dev \
libglfw3-dev \
cmake \
gcc \
git \
make \
g++ \
build-essential \
ninja-build \
libglew-dev \
libxft-dev && \
git clone https://github.com/vokegpu/ekg-ui-library.git && \
cd ekg-ui-library && \
git clone https://github.com/vokegpu/ekg-sandbox.git

# Build 
RUN cd /ekg-ui-library && \
cmake \
    -S . \
    -B ./cmake-build \
    -G Ninja \
    -D CMAKE_EXPORT_COMPILE_COMMANDS=1 \
    -D CMAKE_CXX_COMPILER="/usr/bin/g++"\
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=./ekg-cmake-install && \
cmake \
    --build ./cmake-build && \
cmake \
    --install ./cmake-build \
    --prefix ./ekg-cmake-install && \
cd ./ekg-sandbox && \
cmake \
-S . \
-B ./cmake-build \
-G Ninja \
-D CMAKE_CXX_COMPILER="/usr/bin/g++" \
-D CMAKE_BUILD_TYPE=Release \
-D EKG_DEVELOPER_MODE=1 && \
cmake \
    --build ./cmake-build 