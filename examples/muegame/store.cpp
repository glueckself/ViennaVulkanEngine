#include <vector>
#include <string>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

// Function to encode and save frames to a video file
void encodeAndSaveFrames(const std::vector<AVFrame*>& frames, const std::string& filename, int width, int height, int fps) {
    avcodec_register_all();
    avformat_network_init();

    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "H264 codec not found\n";
        return;
    }

    AVCodecContext* codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        std::cerr << "Failed to allocate codec context\n";
        return;
    }

    codecCtx->bit_rate = 400000;
    codecCtx->width = width;
    codecCtx->height = height;
    codecCtx->time_base = AVRational{1, fps};
    codecCtx->framerate = AVRational{fps, 1};
    codecCtx->gop_size = 10;
    codecCtx->max_b_frames = 1;
    codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264)
        av_opt_set(codecCtx->priv_data, "preset", "fast", 0);

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        std::cerr << "Could not open codec\n";
        return;
    }

    AVFormatContext* fmtCtx = nullptr;
    avformat_alloc_output_context2(&fmtCtx, nullptr, nullptr, filename.c_str());
    if (!fmtCtx) {
        std::cerr << "Could not allocate format context\n";
        return;
    }

    AVStream* stream = avformat_new_stream(fmtCtx, nullptr);
    stream->time_base = codecCtx->time_base;
    avcodec_parameters_from_context(stream->codecpar, codecCtx);

    if (!(fmtCtx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&fmtCtx->pb, filename.c_str(), AVIO_FLAG_WRITE) < 0) {
            std::cerr << "Could not open output file\n";
            return;
        }
    }

    avformat_write_header(fmtCtx, nullptr);

    for (size_t i = 0; i < frames.size(); ++i) {
        AVPacket* pkt = av_packet_alloc();
        AVFrame* frame = frames[i];
        frame->pts = static_cast<int64_t>(i);

        int ret = avcodec_send_frame(codecCtx, frame);
        if (ret < 0) {
            std::cerr << "Error sending frame\n";
            continue;
        }

        while (ret >= 0) {
            ret = avcodec_receive_packet(codecCtx, pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else if (ret < 0) {
                std::cerr << "Error encoding frame\n";
                break;
            }

            av_interleaved_write_frame(fmtCtx, pkt);
            av_packet_unref(pkt);
        }

        av_packet_free(&pkt);
    }

    av_write_trailer(fmtCtx);

    if (!(fmtCtx->oformat->flags & AVFMT_NOFILE))
        avio_closep(&fmtCtx->pb);

    avcodec_free_context(&codecCtx);
    avformat_free_context(fmtCtx);

    std::cout << "Video saved to: " << filename << std::endl;
}
