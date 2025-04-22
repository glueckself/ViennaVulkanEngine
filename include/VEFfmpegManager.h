#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

class FfmpegManager {
    public:
        FfmpegManager();
        ~FfmpegManager();

        void InitFFmpegEncoder(int width, int height, int fps);
        void PushFrameToFFmpeg(uint8_t* rgbaData);
        bool isInitialized();

    private:

        void FinalizeFFmpeg();

        // FFmpeg state
        AVCodecContext* m_codecCtx = nullptr;
        SwsContext* m_swsCtx = nullptr;
        AVPacket* m_pkt = nullptr;
        std::ofstream m_rawOutFile;
        int m_frameCounter = 0;
        int m_frameWidth = 0;
        int m_frameHeight = 0;
        bool m_ffmpegInitialized = false;


};
