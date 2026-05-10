//
// Created by divmone on 5/9/2026.
//

#include "TcpClient.hpp"

#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>

TcpClient::TcpClient(const std::string& ip, std::uint16_t port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
        throw std::invalid_argument("Invalid IP: " + ip);

    if (connect(socket.getFd(), (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Connection failed to " + ip);

}

void TcpClient::send(const std::string &data) {
    ssize_t total = 0;
    while (total < data.size()) {
        const auto res = ::send(socket.getFd(), data.data() + total, data.length() - total, 0);
        if (res < 0) {
            throw std::runtime_error("Failed to send");
        }
        total += res;
    }
}

std::string TcpClient::readLine() {
    while (true) {
        auto pos = buffer.find('\n');
        if (pos != std::string::npos) {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            return line;
        }

        char buff[4096];
        const auto n = recv(socket.getFd(), buff, sizeof(buff), 0);
        if (n < 0) {
            throw std::runtime_error("Recv failed");
        }
        if (n == 0) {
            throw std::runtime_error("Connection closed");
        }

        buffer.append(buff, n);
    }
}
