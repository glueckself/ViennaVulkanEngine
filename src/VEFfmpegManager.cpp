#include "VHInclude.h"
#include "VEInclude.h"

namespace vve {

    FfmpegManager::FfmpegManager(std::string systemName, vve::Engine &engine, std::string windowName)
            : System(systemName, engine), m_windowName(windowName) {
        std::cout << "[FFmpeg] Manager created" << std::endl;
        m_engine.RegisterCallbacks({
                                           {this, 0, "FRAME_END_FFMPEG", [this](Message& message){ return OnFrameEnd(message); }}
                                   });
    }

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
        m_codecCtx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

        if (avcodec_open2(m_codecCtx, codec, nullptr) < 0) {
            std::cerr << "[FFmpeg] Error: Could not open codec." << std::endl;
            return;
        }

        avformat_alloc_output_context2(&m_formatCtx, nullptr, "mpegts", nullptr);
        if (!m_formatCtx) {
            std::cerr << "[FFmpeg] Error: Could not create MPEG-TS output context." << std::endl;
            return;
        }

        m_stream = avformat_new_stream(m_formatCtx, nullptr);
        if (!m_stream) {
            std::cerr << "[FFmpeg] Error: Failed to create stream." << std::endl;
            return;
        }

        m_stream->time_base = m_codecCtx->time_base;
        if (avcodec_parameters_from_context(m_stream->codecpar, m_codecCtx) < 0) {
            std::cerr << "[FFmpeg] Error: Failed to copy codec parameters to stream." << std::endl;
            return;
        }

        std::string url = "udp://[::1]:12345";
        if (avio_open(&m_formatCtx->pb, url.c_str(), AVIO_FLAG_WRITE) < 0) {
            std::cerr << "[FFmpeg] Error: Could not open UDP stream at " << url << std::endl;
            return;
        }

        if (avformat_write_header(m_formatCtx, nullptr) < 0) {
            std::cerr << "[FFmpeg] Error: Failed to write header." << std::endl;
            return;
        }

        m_pkt = av_packet_alloc();
        m_swsCtx = sws_getContext(width, height, AV_PIX_FMT_RGBA, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR,
                                  nullptr, nullptr, nullptr);
        m_frameWidth = width;
        m_frameHeight = height;
        m_frameCounter = 0;
        m_ffmpegInitialized = true;

        std::cout << "[FFmpeg] Encoder initialized and streaming to " << url << std::endl;
    }

    void FfmpegManager::PushFrameToFFmpeg(uint8_t *rgbaData) {
        if (!m_ffmpegInitialized) return;

        AVFrame *rgbFrame = av_frame_alloc();
        rgbFrame->format = AV_PIX_FMT_RGBA;
        rgbFrame->width = m_frameWidth;
        rgbFrame->height = m_frameHeight;
        av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbaData, AV_PIX_FMT_RGBA, m_frameWidth, m_frameHeight, 1);

        AVFrame *yuvFrame = av_frame_alloc();
        yuvFrame->format = AV_PIX_FMT_YUV420P;
        yuvFrame->width = m_frameWidth;
        yuvFrame->height = m_frameHeight;
        av_image_alloc(yuvFrame->data, yuvFrame->linesize, m_frameWidth, m_frameHeight, AV_PIX_FMT_YUV420P, 32);
        yuvFrame->pts = m_frameCounter++;

        sws_scale(m_swsCtx, rgbFrame->data, rgbFrame->linesize, 0, m_frameHeight, yuvFrame->data, yuvFrame->linesize);

        if (avcodec_send_frame(m_codecCtx, yuvFrame) >= 0) {
            while (avcodec_receive_packet(m_codecCtx, m_pkt) == 0) {
                m_pkt->stream_index = m_stream->index;
                av_interleaved_write_frame(m_formatCtx, m_pkt);
                av_packet_unref(m_pkt);
            }
        }

        av_freep(&yuvFrame->data[0]);
        av_frame_free(&yuvFrame);
        av_frame_free(&rgbFrame);
    }

    void FfmpegManager::FinalizeFFmpeg() {
        if (!m_ffmpegInitialized) return;

        avcodec_send_frame(m_codecCtx, nullptr);
        while (avcodec_receive_packet(m_codecCtx, m_pkt) == 0) {
            m_pkt->stream_index = m_stream->index;
            av_interleaved_write_frame(m_formatCtx, m_pkt);
            av_packet_unref(m_pkt);
        }

        av_write_trailer(m_formatCtx);

        if (m_pkt) av_packet_free(&m_pkt);
        if (m_codecCtx) avcodec_free_context(&m_codecCtx);
        if (m_formatCtx) {
            if (m_formatCtx->pb) avio_close(m_formatCtx->pb);
            avformat_free_context(m_formatCtx);
        }
        if (m_swsCtx) sws_freeContext(m_swsCtx);

        m_ffmpegInitialized = false;
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
    }

} // namespace vve

