//
// Created by divmone on 5/12/2026.
//

#pragma once

#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;

class TcpConnection {
public:
    explicit TcpConnection(tcp::socket socket);
    ~TcpConnection();
    boost::asio::awaitable<std::string> asyncReadLine();
    boost::asio::awaitable<void> asyncWriteLine(const std::string&);
private:
    tcp::socket socket;
    boost::asio::streambuf buffer;
};
