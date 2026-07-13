#!/bin/bash
set -euo pipefail

sudo apt update
sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libfreetype-dev \
    libharfbuzz-dev \
    libmbedtls-dev \
    libssh2-1-dev
sudo apt install libglew-dev libglm-dev
