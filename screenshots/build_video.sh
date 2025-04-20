#!/bin/bash

# Define FFmpeg paths
FFMPEG_DIR="$HOME/Uni/Cloud_Gaming/ffmpeg_build"
INCLUDE_DIR="$FFMPEG_DIR/include"
LIB_DIR="$FFMPEG_DIR/lib"

# Output binary name
OUTPUT="store"

# Compile and link
g++ store.cpp -o $OUTPUT \
  -I"$INCLUDE_DIR" \
  -L"$LIB_DIR" \
  -lavcodec -lavformat -lavutil -lswscale \
  -lm -lpthread

# Final status
if [ $? -eq 0 ]; then
  echo "✅ Build successful: ./$OUTPUT"
else
  echo "❌ Build failed."
fi

# Reminder to convert the output file
echo ""
echo "To convert output.h264 to MP4, run:"
echo "ffmpeg -i output.h264 -c:v copy -bsf:v h264_mp4toannexb -c:a copy output.mp4"