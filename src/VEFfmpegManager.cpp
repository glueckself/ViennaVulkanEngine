#include "VEFfmpegManager.h"

FfmpegManager::FfmpegManager() {};

FfmpegManager::~FfmpegManager() {
    FinalizeFFmpeg();
}

bool FfmpegManager::isInitialized() {
    return m_ffmpegInitialized;
}

void FfmpegManager::InitFFmpegEncoder(int width, int height, int fps) {
    std::cout << "[FFmpeg] Initializing encoder..." << std::endl;

    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
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
    m_swsCtx = sws_getContext(width, height, AV_PIX_FMT_RGBA, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);

    m_frameWidth = width;
    m_frameHeight = height;
    m_frameCounter = 0;
    m_ffmpegInitialized = true;

    std::cout << "[FFmpeg] Encoder initialized successfully." << std::endl;
}




void FfmpegManager::PushFrameToFFmpeg(uint8_t* rgbaData) {
    if (!m_ffmpegInitialized) {
        std::cerr << "[FFmpeg] Warning: Encoder not initialized. Skipping frame." << std::endl;
        return;
    }

    AVFrame* rgbFrame = av_frame_alloc();
    rgbFrame->format = AV_PIX_FMT_RGBA;
    rgbFrame->width = m_frameWidth;
    rgbFrame->height = m_frameHeight;
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, rgbaData, AV_PIX_FMT_RGBA, m_frameWidth, m_frameHeight, 1);

    AVFrame* yuvFrame = av_frame_alloc();
    yuvFrame->format = AV_PIX_FMT_YUV420P;
    yuvFrame->width = m_frameWidth;
    yuvFrame->height = m_frameHeight;
    av_image_alloc(yuvFrame->data, yuvFrame->linesize, m_frameWidth, m_frameHeight, AV_PIX_FMT_YUV420P, 32);
    yuvFrame->pts = m_frameCounter++;

    sws_scale(m_swsCtx, rgbFrame->data, rgbFrame->linesize, 0, m_frameHeight, yuvFrame->data, yuvFrame->linesize);

    if (avcodec_send_frame(m_codecCtx, yuvFrame) >= 0) {
        while (avcodec_receive_packet(m_codecCtx, m_pkt) == 0) {
            if (m_rawOutFile.is_open()) {
                m_rawOutFile.write(reinterpret_cast<const char*>(m_pkt->data), m_pkt->size);
            }
            av_packet_unref(m_pkt);
        }
    }

    av_freep(&yuvFrame->data[0]);
    av_frame_free(&yuvFrame);
    av_frame_free(&rgbFrame);
}



void FfmpegManager::FinalizeFFmpeg() {
    if (!m_ffmpegInitialized) return;

    std::cout << "[FFmpeg] Finalizing stream..." << std::endl;

    if (m_codecCtx && m_pkt && avcodec_send_frame(m_codecCtx, nullptr) >= 0) {
        while (avcodec_receive_packet(m_codecCtx, m_pkt) == 0) {
            if (m_rawOutFile.is_open()) {
                m_rawOutFile.write(reinterpret_cast<const char*>(m_pkt->data), m_pkt->size);
            }
            av_packet_unref(m_pkt);
        }
    }

    if (m_rawOutFile.is_open()) {
        m_rawOutFile.close();
    }

    if (m_pkt) {
        av_packet_free(&m_pkt);
        m_pkt = nullptr;
    }

    if (m_codecCtx) {
        avcodec_free_context(&m_codecCtx);
        m_codecCtx = nullptr;
    }

    if (m_swsCtx) {
        sws_freeContext(m_swsCtx);
        m_swsCtx = nullptr;
    }

    m_ffmpegInitialized = false;
    std::cout << "[FFmpeg] Finalization complete. Raw stream saved." << std::endl;
}