#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class UDPClient {
private:
    void sendFrameOverUDP();

    std::string receiverIP;
    int receiverPort;
    int fps;
    size_t frameSize ;
    int sockfd;
    struct sockaddr_in6 serverAddr;

public:
    UDPClient(const std::string &receiverIP, int receiverPort, int fps);
    ~UDPClient();
    void sendFrame(uint8_t *frameBuffer, size_t bufferSize);
};


