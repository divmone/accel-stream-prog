//
// Created by divmone on 5/11/2026.
//

#include "AsyncServer.hpp"

#include <spdlog/spdlog.h>

#include "Session.hpp"
#include "TcpConnection.hpp"

AsyncServer::AsyncServer(boost::asio::io_context& io, uint16_t portA, uint16_t portB)
    : acceptorA(io, {boost::asio::ip::tcp::v4(), portA}), acceptorB(io, {boost::asio::ip::tcp::v4(), portB}){

    spdlog::info("[Server] Create on ports {} and {}", portA, portB);
}

boost::asio::awaitable<void> AsyncServer::run() {
    spdlog::info("[Server] accepting connections");
    while (true) {
        spdlog::info("[Server] waiting for client A on port {}", acceptorA.local_endpoint().port());
        auto sockA = co_await acceptorA.async_accept(boost::asio::use_awaitable);
        auto peerA = sockA.remote_endpoint();
        spdlog::info("[Server] client A connected from {}:{}", peerA.address().to_string(), peerA.port());

        spdlog::info("[Server] waiting for client B on port {}", acceptorB.local_endpoint().port());
        auto sockB = co_await acceptorB.async_accept(boost::asio::use_awaitable);
        auto peerB = sockB.remote_endpoint();
        spdlog::info("[Server] client B connected from {}:{}", peerB.address().to_string(), peerB.port());

        auto conn_a = std::make_shared<TcpConnection>(std::move(sockA));
        auto conn_b = std::make_shared<TcpConnection>(std::move(sockB));

        co_await std::make_shared<Session>(conn_a, conn_b)->run();
    }
}