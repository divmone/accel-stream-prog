//
// Created by divmone on 5/11/2026.
//

#pragma once

#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;

class AsyncServer {
public:
    AsyncServer(boost::asio::io_context&, uint16_t, uint16_t);
    boost::asio::awaitable<void> run();

private:
    tcp::acceptor acceptorA, acceptorB;
};
