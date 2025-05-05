#!/bin/bash

# Define FFmpeg paths
FFMPEG_DIR="$HOME/Uni/Cloud_Gaming/ffmpeg_build"
INCLUDE_DIR="$FFMPEG_DIR/include"
LIB_DIR="$FFMPEG_DIR/lib"

# Output binary name
OUTPUT="udp_server"

# Compile and link with debug information
g++ -g UDP_Server.cpp -o $OUTPUT \
  -I"$INCLUDE_DIR" \
  -L"$LIB_DIR" \
  `pkg-config --cflags --libs sdl3` \
  `pkg-config --cflags --libs libavformat libavcodec libavutil libswscale` \
  -lz -liconv -lx265 -ldrm -llzma -lpthread \
  -lwayland-client -lwayland-egl -lX11

# Final status
if [ $? -eq 0 ]; then
  echo "Build successful: ./$OUTPUT"
else
  echo "Build failed."
fi

