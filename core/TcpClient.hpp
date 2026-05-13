//
// Created by divmone on 5/9/2026.
//

#pragma once

#include <boost/asio.hpp>
#include <spdlog/spdlog.h>
#include <cstdint>
#include <string>

#include "TcpConnection.hpp"

using tcp = boost::asio::ip::tcp;

class TcpClient {
public:
    static boost::asio::awaitable<std::shared_ptr<TcpConnection> >
    connect(boost::asio::io_context &io, const std::string &ip, uint16_t port) {
        spdlog::info("[TcpClient] connecting to {}:{}", ip, port);
        tcp::resolver resolver(io);
        tcp::socket socket(io);
        auto endpoints = co_await resolver.async_resolve(
            ip, std::to_string(port), boost::asio::use_awaitable);

        co_await boost::asio::async_connect(socket, endpoints,
                                            boost::asio::use_awaitable);
        auto peer = socket.remote_endpoint();
        spdlog::info("[TcpClient] connected to {}:{}", peer.address().to_string(), peer.port());
        co_return std::make_shared<TcpConnection>(std::move(socket));
    }
};
