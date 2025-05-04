#include "UDPClient.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

UDPClient::UDPClient(const std::string& receiverIP, int receiverPort, int fps)
        : receiverIP(receiverIP), receiverPort(receiverPort), fps(fps) {

    std::cout << "[UDPClient] Initializing with target " << receiverIP << ":" << receiverPort << " at " << fps << " FPS." << std::endl;

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "[UDPClient] Error opening socket." << std::endl;
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(receiverPort);
    if (inet_pton(AF_INET6, receiverIP.c_str(), &serverAddr.sin6_addr) <= 0) {
        std::cerr << "[UDPClient] Invalid IPv6 address: " << receiverIP << std::endl;
        exit(1);
    }

    std::cout << "[UDPClient] Socket initialized successfully." << std::endl;
}

UDPClient::~UDPClient() {
    std::cout << "[UDPClient] Closing socket." << std::endl;
    close(sockfd);
}

void UDPClient::sendFrame(uint8_t *frameBuffer, size_t bufferSize) {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastTime;

    double frameDuration = 1.0 / fps;
    if (elapsed.count() >= frameDuration) {
        ssize_t sentBytes = sendto(sockfd, frameBuffer, bufferSize, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        lastTime = currentTime;
    } else {
        auto sleepTime = std::chrono::duration<double>(frameDuration - elapsed.count());
        std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(sleepTime));
    }
}
