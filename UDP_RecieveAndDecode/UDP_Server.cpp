#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include <array>
#include <unistd.h>
#include <cstring>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#define INPUT_URL "udp://[::]:12345"

// Holds FFmpeg-related decoding resources
struct DecoderContext {
    AVFormatContext* fmtCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    AVCodecParameters* codecPar = nullptr;
    AVFrame* frame = av_frame_alloc();
    AVPacket* packet = av_packet_alloc();
    SwsContext* swsCtx = nullptr;
    const AVCodec* decoder = nullptr;
    int videoStreamIndex = -1;
};

// Holds SDL rendering context
struct SDLContext {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
};

// Initialize SDL window and renderer
bool initSDL(SDLContext& sdlCtx) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("UDP MPEG-TS Receiver", 640, 480, SDL_WINDOW_RESIZABLE, &sdlCtx.window, &sdlCtx.renderer)) {
        std::cerr << "SDL_CreateWindowAndRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

// Initialize FFmpeg demuxer, find video stream, and prepare decoder
bool initFFmpegDecoder(DecoderContext& ctx) {
    AVDictionary* options = nullptr;

    if (avformat_open_input(&ctx.fmtCtx, INPUT_URL, nullptr, &options) < 0) {
        std::cerr << "Failed to open input from: " << INPUT_URL << std::endl;
        av_dict_free(&options);
        return false;
    }

    av_dict_free(&options); // Clean up dictionary

    if (avformat_find_stream_info(ctx.fmtCtx, nullptr) < 0) {
        std::cerr << "Failed to get stream info." << std::endl;
        return false;
    }

    // Locate first video stream
    for (unsigned int i = 0; i < ctx.fmtCtx->nb_streams; ++i) {
        if (ctx.fmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            ctx.videoStreamIndex = i;
            break;
        }
    }

    if (ctx.videoStreamIndex == -1) {
        std::cerr << "No video stream found." << std::endl;
        return false;
    }

    // Find decoder and set up codec context
    ctx.codecPar = ctx.fmtCtx->streams[ctx.videoStreamIndex]->codecpar;
    ctx.decoder = avcodec_find_decoder(ctx.codecPar->codec_id);
    if (!ctx.decoder) {
        std::cerr << "Decoder not found." << std::endl;
        return false;
    }

    ctx.codecCtx = avcodec_alloc_context3(ctx.decoder);
    if (!ctx.codecCtx || avcodec_parameters_to_context(ctx.codecCtx, ctx.codecPar) < 0) {
        std::cerr << "Failed to create codec context." << std::endl;
        return false;
    }

    if (avcodec_open2(ctx.codecCtx, ctx.decoder, nullptr) < 0) {
        std::cerr << "Failed to open codec." << std::endl;
        return false;
    }

    return true;
}


void decodeAndRender(DecoderContext& decoder, SDLContext& sdlCtx) {
    SDL_Event event;
    bool running = true;

    while (running) {
        // Check window close or other events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        int ret = av_read_frame(decoder.fmtCtx, decoder.packet);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            // No data available (yet) — wait a bit and continue
            SDL_Delay(10);
            continue;
        } else if (ret < 0) {
            std::cerr << "av_read_frame failed: " << ret << std::endl;
            continue; // or handle fatal error differently if needed
        }

        // Process packet if it's a video frame
        if (decoder.packet->stream_index == decoder.videoStreamIndex) {
            if (avcodec_send_packet(decoder.codecCtx, decoder.packet) >= 0) {
                while (avcodec_receive_frame(decoder.codecCtx, decoder.frame) == 0) {
                    if (!decoder.swsCtx) {
                        decoder.swsCtx = sws_getContext(
                                decoder.codecCtx->width, decoder.codecCtx->height, decoder.codecCtx->pix_fmt,
                                decoder.codecCtx->width, decoder.codecCtx->height, AV_PIX_FMT_RGBA,
                                SWS_BILINEAR, nullptr, nullptr, nullptr
                        );
                    }

                    std::vector<uint8_t> rgbaBuffer(decoder.codecCtx->width * decoder.codecCtx->height * 4);
                    uint8_t* rgbaData[4] = { rgbaBuffer.data(), nullptr, nullptr, nullptr };
                    int rgbaLinesize[4] = { decoder.codecCtx->width * 4, 0, 0, 0 };

                    sws_scale(decoder.swsCtx,
                              decoder.frame->data,
                              decoder.frame->linesize,
                              0,
                              decoder.codecCtx->height,
                              rgbaData,
                              rgbaLinesize);

                    if (!sdlCtx.texture) {
                        sdlCtx.texture = SDL_CreateTexture(
                                sdlCtx.renderer,
                                SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_STREAMING,
                                decoder.codecCtx->width,
                                decoder.codecCtx->height
                        );
                    }

                    SDL_UpdateTexture(sdlCtx.texture, nullptr, rgbaBuffer.data(), decoder.codecCtx->width * 4);
                    SDL_RenderClear(sdlCtx.renderer);
                    SDL_RenderTexture(sdlCtx.renderer, sdlCtx.texture, nullptr, nullptr);
                    SDL_RenderPresent(sdlCtx.renderer);
                }
            }
        }

        av_packet_unref(decoder.packet);  // always clean up
    }
}


// Free all allocated resources
void cleanup(DecoderContext& decoder, SDLContext& sdlCtx) {
    if (sdlCtx.texture) SDL_DestroyTexture(sdlCtx.texture);
    if (decoder.swsCtx) sws_freeContext(decoder.swsCtx);
    av_frame_free(&decoder.frame);
    av_packet_free(&decoder.packet);
    avcodec_free_context(&decoder.codecCtx);
    avformat_close_input(&decoder.fmtCtx);
    SDL_DestroyRenderer(sdlCtx.renderer);
    SDL_DestroyWindow(sdlCtx.window);
    SDL_Quit();
}

// Entrypoint: initializes systems, runs decoding/render loop, then cleans up
int main() {
    std::cout << "UDP MPEG-TS Receiver Started..." << std::endl;

    DecoderContext decoder;
    SDLContext sdl;

    if (!initSDL(sdl)) return 1;
    if (!initFFmpegDecoder(decoder)) return 1;

    decodeAndRender(decoder, sdl);
    cleanup(decoder, sdl);

    return 0;
}
