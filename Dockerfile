FROM ubuntu:24.04
# Install Needs
RUN apt update && \
apt upgrade -y && \
apt install -y libsdl2-dev \
libglfw3-dev \
cmake \
gcc \
git \
libxft-dev && \
git clone https://github.com/vokegpu/ekg-ui-library.git