#pragma once

#include <vector>
#include <string>

extern "C" {
#include <libavcodec/avcodec.h>
}

void encodeAndSaveFrames(const std::vector<AVFrame*>& frames, const std::string& filename, int width, int height, int fps);
