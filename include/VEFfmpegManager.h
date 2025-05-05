#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "VEInclude.h"
#include "VESystem.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
}

namespace vve {

    class FfmpegManager : public System {
    public:
        FfmpegManager(std::string systemName, Engine& engine, std::string windowName);
        ~FfmpegManager();

    private:
        // --- Frame hook callback ---
        bool OnFrameEnd(Message message);

        // --- Encoder Initialization ---
        void InitFFmpegEncoder(int width, int height, int fps);
        bool CreateCodecContext(int width, int height, int fps);
        bool CreateOutputContext();
        bool CreateStream();
        bool OpenOutputStream();
        bool WriteStreamHeader();
        void InitializeConversionContext(int width, int height);

        // --- Frame Handling ---
        void SendImageToUDP(uint8_t* rgbaData);
        AVFrame* CreateRGBFrame(uint8_t* data);
        AVFrame* CreateYUVFrame();
        void ConvertRGBToYUV(AVFrame* rgb, AVFrame* yuv);
        void EncodeAndWriteFrame(AVFrame* frame);


        // --- Cleanup ---
        void FinalizeFFmpeg();

    private:
        std::string m_windowName;
        bool m_enableStreaming { true };
        bool m_streamToUDPServer { true };

        // FFmpeg state
        AVCodecContext* m_codecCtx = nullptr;
        SwsContext* m_swsCtx = nullptr;
        AVPacket* m_pkt = nullptr;
        AVFormatContext* m_formatCtx = nullptr;
        AVStream* m_stream = nullptr;

        int m_frameCounter = 0;
        int m_frameWidth = 0;
        int m_frameHeight = 0;
        bool m_ffmpegInitialized = false;

        // Sender
        const std::string url = "udp://[::1]:12345";

    };

} // namespace vve
