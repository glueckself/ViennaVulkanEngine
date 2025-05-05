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
        if (!CreateCodecContext(width, height, fps)) return;
        if (!CreateOutputContext()) return;
        if (!CreateStream()) return;
        if (!OpenOutputStream()) return;
        if (!WriteStreamHeader()) return;

        InitializeConversionContext(width, height);

        m_frameWidth = width;
        m_frameHeight = height;
        m_frameCounter = 0;
        m_ffmpegInitialized = true;

        std::cout << "[FFmpeg] Encoder initialized." << std::endl;
    }

// --- Initialization helpers ---
    bool FfmpegManager::CreateCodecContext(int width, int height, int fps) {
        const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!codec) {
            std::cerr << "[FFmpeg] Error: H264 codec not found!" << std::endl;
            return false;
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
            return false;
        }

        return true;
    }

    bool FfmpegManager::CreateOutputContext() {
        avformat_alloc_output_context2(&m_formatCtx, nullptr, "mpegts", nullptr);
        if (!m_formatCtx) {
            std::cerr << "[FFmpeg] Error: Could not create output context." << std::endl;
            return false;
        }
        return true;
    }

    bool FfmpegManager::CreateStream() {
        m_stream = avformat_new_stream(m_formatCtx, nullptr);
        if (!m_stream) {
            std::cerr << "[FFmpeg] Error: Failed to create stream." << std::endl;
            return false;
        }

        m_stream->time_base = m_codecCtx->time_base;

        if (avcodec_parameters_from_context(m_stream->codecpar, m_codecCtx) < 0) {
            std::cerr << "[FFmpeg] Error: Failed to copy codec parameters." << std::endl;
            return false;
        }

        return true;
    }

    bool FfmpegManager::OpenOutputStream() {
        if (avio_open(&m_formatCtx->pb, url.c_str(), AVIO_FLAG_WRITE) < 0) {
            std::cerr << "[FFmpeg] Error: Could not open output stream at " << url << std::endl;
            return false;
        }
        return true;
    }

    bool FfmpegManager::WriteStreamHeader() {
        if (avformat_write_header(m_formatCtx, nullptr) < 0) {
            std::cerr << "[FFmpeg] Error: Failed to write stream header." << std::endl;
            return false;
        }

        m_pkt = av_packet_alloc();
        return true;
    }

    void FfmpegManager::InitializeConversionContext(int width, int height) {
        m_swsCtx = sws_getContext(width, height, AV_PIX_FMT_RGBA,
                                  width, height, AV_PIX_FMT_YUV420P,
                                  SWS_BILINEAR, nullptr, nullptr, nullptr);
    }

// --- Frame Sending (Main new function) ---
    void FfmpegManager::SendImageToUDP(uint8_t* rgbaData) {
        AVFrame* rgb = CreateRGBFrame(rgbaData);
        AVFrame* yuv = CreateYUVFrame();
        ConvertRGBToYUV(rgb, yuv);
        EncodeAndWriteFrame(yuv);

        av_freep(&yuv->data[0]);
        av_frame_free(&yuv);
        av_frame_free(&rgb);
    }

    AVFrame* FfmpegManager::CreateRGBFrame(uint8_t* data) {
        AVFrame* frame = av_frame_alloc();
        frame->format = AV_PIX_FMT_RGBA;
        frame->width = m_frameWidth;
        frame->height = m_frameHeight;
        av_image_fill_arrays(frame->data, frame->linesize, data, AV_PIX_FMT_RGBA, m_frameWidth, m_frameHeight, 1);
        return frame;
    }

    AVFrame* FfmpegManager::CreateYUVFrame() {
        AVFrame* frame = av_frame_alloc();
        frame->format = AV_PIX_FMT_YUV420P;
        frame->width = m_frameWidth;
        frame->height = m_frameHeight;
        av_image_alloc(frame->data, frame->linesize, m_frameWidth, m_frameHeight, AV_PIX_FMT_YUV420P, 32);
        frame->pts = m_frameCounter++;
        return frame;
    }

    void FfmpegManager::ConvertRGBToYUV(AVFrame* rgb, AVFrame* yuv) {
        sws_scale(m_swsCtx, rgb->data, rgb->linesize, 0, m_frameHeight, yuv->data, yuv->linesize);
    }

    void FfmpegManager::EncodeAndWriteFrame(AVFrame* frame) {
        if (avcodec_send_frame(m_codecCtx, frame) >= 0) {
            while (avcodec_receive_packet(m_codecCtx, m_pkt) == 0) {
                m_pkt->stream_index = m_stream->index;
                av_interleaved_write_frame(m_formatCtx, m_pkt);
                av_packet_unref(m_pkt);
            }
        }
    }



// --- Callback Entry ---
    bool FfmpegManager::OnFrameEnd(Message message) {
        if (!m_enableStreaming) {
            return false;
        }

        auto vstate = std::get<1>(Renderer::GetState(m_registry));
        auto wstate = std::get<1>(Window::GetState(m_registry, m_windowName));

        VkExtent2D extent = { (uint32_t)wstate().m_width, (uint32_t)wstate().m_height };
        uint32_t imageSize = extent.width * extent.height * 4;
        VkImage image = vstate().m_swapChain.m_swapChainImages[vstate().m_imageIndex];

        uint8_t* dataImage = new uint8_t[imageSize];

        vh::ImgCopyImageToHost(
                vstate().m_device,
                vstate().m_vmaAllocator,
                vstate().m_graphicsQueue,
                vstate().m_commandPool,
                image,
                VK_FORMAT_R8G8B8A8_UNORM,
                VK_IMAGE_ASPECT_COLOR_BIT,
                VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                dataImage,
                extent.width,
                extent.height,
                imageSize,
                2, 1, 0, 3
        );

        if (!m_ffmpegInitialized) {
            InitFFmpegEncoder(extent.width, extent.height, 30); // 30 FPS default
        }

        SendImageToUDP(dataImage);
        delete[] dataImage;

        return false;
    }


// --- Cleanup ---
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

} // namespace vve
