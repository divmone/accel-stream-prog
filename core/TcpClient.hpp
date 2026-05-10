//
// Created by divmone on 5/9/2026.
//

#pragma once

#include <cstdint>
#include <string>

#include "Socket.hpp"

class TcpClient {
public:
    TcpClient(const std::string& ip, std::uint16_t port);

    void send(const std::string&);
    std::string readLine();

private:
    Socket socket;
    std::string buffer;
};


