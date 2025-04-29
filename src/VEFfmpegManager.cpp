#include "VHInclude.h"
#include "VEInclude.h"


namespace vve {

    FfmpegManager::FfmpegManager(std::string systemName, vve::Engine &engine, std::string windowName)  :
            System(systemName, engine), m_windowName(windowName) {
            std::cout << "[FFmpeg] Manager created" << std::endl;
            m_engine.RegisterCallbacks( {
                    {this, 0, "FRAME_END_FFMPEG", [this](Message& message){ return OnFrameEnd(message); } }
            } );
    };

    FfmpegManager::~FfmpegManager() {
        FinalizeFFmpeg();
    }


    void FfmpegManager::InitFFmpegEncoder(int width, int height, int fps) {
        std::cout << "[FFmpeg] Initializing encoder..." << std::endl;

        const AVCodec *codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!codec) {
            std::cerr << "[FFmpeg] Error: H264 codec not found!" << std::endl;
            return;
        }

        m_codecCtx = avcodec_alloc_context3(codec);
        m_codecCtx->bit_rate = 800000;
        m_codecCtx->width = width;
        m_codecCtx->height = height;
        m_codecCtx->time_base = {1, fps};
        m_codecCtx->framerate = {fps, 1};
        m_codecCtx->gop_size = 10;
        m_codecCtx->max_b_frames = 1;
        m_codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

        if (avcodec_open2(m_codecCtx, codec, nullptr) < 0) {
            std::cerr << "[FFmpeg] Error: Could not open codec." << std::endl;
            return;
        }

        std::ostringstream rawFilename;
        rawFilename << "stream/raw_output_" << m_codecCtx->bit_rate << ".h264";

        m_rawOutFile.open(rawFilename.str(), std::ios::binary);
        if (!m_rawOutFile.is_open()) {
            std::cerr << "[FFmpeg] Error: Could not open raw output file: " << rawFilename.str() << std::endl;
        }

        m_pkt = av_packet_alloc();
        m_swsCtx = sws_getContext(width, height, AV_PIX_FMT_RGBA, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR,
                                  nullptr, nullptr, nullptr);
        m_frameWidth = width;
        m_frameHeight = height;
        m_frameCounter = 0;
        m_ffmpegInitialized = true;
        std::cout << "[FFmpeg] Encoder initialized successfully." << std::endl;
    }


    void FfmpegManager::PushFrameToFFmpeg(uint8_t *rgbaData) {
        if (!m_ffmpegInitialized) {
            std::cerr << "[FFmpeg] Warning: Encoder not initialized. Skipping frame." << std::endl;
            return;
        }

        AVFrame *rgbFrame = av_frame_alloc();
        rgbFrame->format = AV_PIX_FMT_RGBA;
        rgbFrame->width = m_frameWidth;
        rgbFrame->height = m_frameHeight;
        av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbaData, AV_PIX_FMT_RGBA, m_frameWidth, m_frameHeight,1);
        AVFrame *yuvFrame = av_frame_alloc();
        yuvFrame->format = AV_PIX_FMT_YUV420P;
        yuvFrame->width = m_frameWidth;
        yuvFrame->height = m_frameHeight;
        av_image_alloc(yuvFrame->data, yuvFrame->linesize, m_frameWidth, m_frameHeight, AV_PIX_FMT_YUV420P, 32);
        yuvFrame->pts = m_frameCounter++;
        sws_scale(m_swsCtx, rgbFrame->data, rgbFrame->linesize, 0, m_frameHeight, yuvFrame->data, yuvFrame->linesize);

        if (avcodec_send_frame(m_codecCtx, yuvFrame) >= 0) {
            while (avcodec_receive_packet(m_codecCtx, m_pkt) == 0) {
                if (m_rawOutFile.is_open()) {
                    m_rawOutFile.write(reinterpret_cast<const char *>(m_pkt->data), m_pkt->size);
                }
                av_packet_unref(m_pkt);
                if (!m_pkt) {
                    std::cerr << "[FFmpeg] Warning: packet became nullptr after unref!" << std::endl;
                    break;  // Do not continue to free or use it
                }

            }
        }
        av_freep(&yuvFrame->data[0]);
        av_frame_free(&yuvFrame);
        av_frame_free(&rgbFrame);
    }


    void FfmpegManager::FinalizeFFmpeg() {
        if (!m_ffmpegInitialized) {
            std::cout << "[FFmpeg] Not initialized, skipping finalization." << std::endl;
            return;
        }

        if (m_codecCtx) {
            int ret = avcodec_send_frame(m_codecCtx, nullptr);
            if (ret < 0) {
            } else if (m_pkt) {
                while (true) {
                    ret = avcodec_receive_packet(m_codecCtx, m_pkt);
                    if (ret == 0) {
                        if (m_rawOutFile.is_open()) {
                            m_rawOutFile.write(reinterpret_cast<const char*>(m_pkt->data), m_pkt->size);
                        }
                        av_packet_unref(m_pkt);
                    } else if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
                        break;
                    } else {
                        std::cerr << "[FFmpeg] Error receiving packet: " << ret << std::endl;
                        break;
                    }
                }
            } else {
                std::cerr << "[FFmpeg] ERROR: m_pkt is nullptr! Cannot receive packets." << std::endl;
            }
        } else {
            std::cerr << "[FFmpeg] ERROR: m_codecCtx is nullptr! Cannot flush encoder." << std::endl;
        }

        if (m_rawOutFile.is_open()) {
            std::cout << "[FFmpeg] Closing raw output file..." << std::endl;
            m_rawOutFile.close();
        }
        if (m_pkt) {
            av_packet_free(&m_pkt);
            m_pkt = nullptr;
        }
        if (m_codecCtx) {
            avcodec_free_context(&m_codecCtx);
            if (m_codecCtx) {
                std::cerr << "[FFmpeg] ERROR: m_codecCtx is still valid after freeing!" << std::endl;
            }
        }
        if (m_swsCtx) {
            sws_freeContext(m_swsCtx);
            m_swsCtx = nullptr;
        }
        m_ffmpegInitialized = false;
        std::cout << "[FFmpeg] Finalization complete. Raw stream saved." << std::endl;
    }



    bool FfmpegManager::OnFrameEnd(Message message) {
        if (m_enableStreaming) {
            auto vstate = std::get<1>(Renderer::GetState(m_registry));
            auto wstate = std::get<1>(Window::GetState(m_registry, m_windowName));

            VkExtent2D extent = {(uint32_t) wstate().m_width, (uint32_t) wstate().m_height};
            uint32_t imageSize = extent.width * extent.height * 4;
            VkImage image = vstate().m_swapChain.m_swapChainImages[vstate().m_imageIndex];

            uint8_t *dataImage = new uint8_t[imageSize];

            vh::ImgCopyImageToHost(
                    vstate().m_device, vstate().m_vmaAllocator, vstate().m_graphicsQueue,
                    vstate().m_commandPool, image, VK_FORMAT_R8G8B8A8_UNORM,
                    VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    dataImage, extent.width, extent.height, imageSize, 2, 1, 0, 3
            );

            if (!m_ffmpegInitialized) {
               InitFFmpegEncoder(extent.width, extent.height, 30);
            }

            PushFrameToFFmpeg(dataImage);
            delete[] dataImage;
        }

        return false;
    };

};

