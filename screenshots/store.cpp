#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // Make sure this is included

#include <iostream>
#include <fstream>
#include <vector>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/opt.h>
    #include <libswscale/swscale.h>
}

#include <string>
#include <cstring>

// Function to encode and save frames as a video
void encodeAndSaveFrames(const std::vector<AVFrame*>& frames, const char* outputFilename, int width, int height, int fps) {
    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Codec not found" << std::endl;
        return;
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        std::cerr << "Could not allocate video codec context" << std::endl;
        return;
    }

    codecCtx->bit_rate = 800000;
    codecCtx->width = width;
    codecCtx->height = height;
    codecCtx->time_base = {1, fps};  // Time base for the codec (1/fps)
    codecCtx->framerate = {fps, 1};
    codecCtx->gop_size = 10;
    codecCtx->max_b_frames = 1;
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        std::cerr << "Could not open codec" << std::endl;
        avcodec_free_context(&codecCtx);
        return;
    }

    std::ofstream outFile(outputFilename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Could not open output file" << std::endl;
        avcodec_free_context(&codecCtx);
        return;
    }

    AVPacket* pkt = av_packet_alloc();
    if (!pkt) {
        std::cerr << "Could not allocate AVPacket" << std::endl;
        avcodec_free_context(&codecCtx);
        return;
    }

    // Set up the conversion context from RGBA to YUV420P
    SwsContext* swsCtx = sws_getContext(width, height, AV_PIX_FMT_RGBA, width, height, AV_PIX_FMT_YUV420P, SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!swsCtx) {
        std::cerr << "Could not allocate SwsContext" << std::endl;
        av_packet_free(&pkt);
        avcodec_free_context(&codecCtx);
        return;
    }

    for (size_t i = 0; i < frames.size(); ++i) {
        AVFrame* rgbaFrame = frames[i];

        // Allocate and set up the YUV frame
        AVFrame* yuvFrame = av_frame_alloc();
        yuvFrame->format = AV_PIX_FMT_YUV420P;
        yuvFrame->width = width;
        yuvFrame->height = height;

        // Allocate memory for the YUV frame
        if (av_image_alloc(yuvFrame->data, yuvFrame->linesize, width, height, AV_PIX_FMT_YUV420P, 32) < 0) {
            std::cerr << "Could not allocate memory for YUV frame" << std::endl;
            av_frame_free(&yuvFrame);
            break;
        }

        // Convert RGBA frame to YUV frame
        sws_scale(swsCtx, rgbaFrame->data, rgbaFrame->linesize, 0, height, yuvFrame->data, yuvFrame->linesize);

        // Set the PTS (presentation timestamp) for this frame
        yuvFrame->pts = i;  // This is a simple way to set it based on the frame number, but make sure it matches the codec time base

        // Encode the YUV frame
        if (avcodec_send_frame(codecCtx, yuvFrame) < 0) {
            std::cerr << "Error sending frame to codec" << std::endl;
            break;
        }

        while (avcodec_receive_packet(codecCtx, pkt) == 0) {
            // Set packet PTS, DTS based on the codec time base
            pkt->pts = pkt->dts = av_rescale_q(yuvFrame->pts, codecCtx->time_base, codecCtx->time_base);
            outFile.write(reinterpret_cast<const char*>(pkt->data), pkt->size);
            av_packet_unref(pkt);
        }

        // Free the YUV frame
        av_freep(&yuvFrame->data[0]);
        av_frame_free(&yuvFrame);
    }

    // Flush the encoder
    avcodec_send_frame(codecCtx, nullptr);
    while (avcodec_receive_packet(codecCtx, pkt) == 0) {
        pkt->pts = pkt->dts = av_rescale_q(pkt->pts, codecCtx->time_base, codecCtx->time_base);
        outFile.write(reinterpret_cast<const char*>(pkt->data), pkt->size);
        av_packet_unref(pkt);
    }

    outFile.close();
    av_packet_free(&pkt);
    avcodec_free_context(&codecCtx);
    sws_freeContext(swsCtx);
}

// Function to capture the screenshot and convert it to an AVFrame
bool captureScreenshotToFrame(const std::string& screenshotPath, AVFrame* frame, int& width, int& height) {
    // Use stb_image to load the screenshot into the frame
    int channels;
    uint8_t* data = stbi_load(screenshotPath.c_str(), &width, &height, &channels, 4);  // 4 channels: RGBA

    if (!data) {
        std::cerr << "Failed to load image: " << screenshotPath << std::endl;
        return false;
    }

    // Fill the AVFrame with the image data (make sure it is in the correct format: RGBA)
    frame->format = AV_PIX_FMT_RGBA;
    frame->width = width;
    frame->height = height;

    // Allocate memory for the frame
    if (av_image_alloc(frame->data, frame->linesize, width, height, AV_PIX_FMT_RGBA, 32) < 0) {
        std::cerr << "Could not allocate memory for frame" << std::endl;
        stbi_image_free(data);
        return false;
    }

    memcpy(frame->data[0], data, width * height * 4);

    // Clean up
    stbi_image_free(data);
    return true;
}

int main() {
    int width = 1200;
    int height = 600;
    int fps = 30;

    std::vector<AVFrame*> frames;

    // Simulate loading multiple screenshots and converting them to AVFrames
    for (int i = 1; i < fps * 4; ++i) { // 4 seconds of video
        AVFrame* frame = av_frame_alloc();
        std::string screenshotPath = "screenshot" + std::to_string(i) + ".jpg";  // Assuming screenshots are in PNG format

        // Capture the screenshot into the frame
        if (captureScreenshotToFrame(screenshotPath, frame, width, height)) {
            frames.push_back(frame);
        } else {
            std::cerr << "Failed to capture screenshot " << i << std::endl;
            av_frame_free(&frame);
        }
    }

    // Encode and save frames as video
    encodeAndSaveFrames(frames, "output.h264", width, height, fps);

    // Free allocated frames
    for (AVFrame* frame : frames) {
        av_freep(&frame->data[0]);
        av_frame_free(&frame);
    }

    return 0;
}
