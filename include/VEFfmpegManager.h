#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include "VEInclude.h"
#include "VESystem.h"
#include "UDPClient.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

namespace vve {
    class FfmpegManager : public System {
        public:
            FfmpegManager(std::string systemName, Engine& engine, std::string windowName);
            ~FfmpegManager();

        private:

            bool OnFrameEnd(Message message);
            void FinalizeFFmpeg();
            void InitFFmpegEncoder(int width, int height, int fps);
            void PushFrameToFFmpeg(uint8_t *rgbaData);

            std::string m_windowName;
            bool m_enableStreaming{ true };
            bool m_streamToUDPServer{ true };

            // FFmpeg state
            AVCodecContext *m_codecCtx = nullptr;
            SwsContext *m_swsCtx = nullptr;
            AVPacket *m_pkt = nullptr;
            std::ofstream m_rawOutFile;
            int m_frameCounter = 0;
            int m_frameWidth = 0;
            int m_frameHeight = 0;
            bool m_ffmpegInitialized = false;

            // UDPClient
            UDPClient *m_udpClient = nullptr;


        };
};
