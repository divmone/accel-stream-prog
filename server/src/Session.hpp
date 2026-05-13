//
// Created by divmone on 5/12/2026.
//

#pragma once
#include <memory>

#include "TcpConnection.hpp"
#include <Protocol.hpp>

class Session : public std::enable_shared_from_this<Session>{
public:
    Session(std::shared_ptr<TcpConnection> a,
            std::shared_ptr<TcpConnection> b);

    boost::asio::awaitable<void> run();

private:
    bool filterPackets(const AccelPacket&, const AccelPacket&);
    boost::asio::awaitable<void> forwardAtoB();
    boost::asio::awaitable<void> forwardBtoA();

    std::shared_ptr<TcpConnection> connA, connB;
};
