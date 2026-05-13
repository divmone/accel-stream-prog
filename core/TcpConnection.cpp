//
// Created by divmone on 5/12/2026.
//

#include "TcpConnection.hpp"

TcpConnection::TcpConnection(tcp::socket socket)
    : socket(std::move(socket)) {
}

TcpConnection::~TcpConnection(){
    boost::system::error_code ec;
    socket.shutdown(tcp::socket::shutdown_both, ec);
    socket.close(ec);
}

boost::asio::awaitable<std::string> TcpConnection::asyncReadLine() {
    co_await boost::asio::async_read_until(socket, buffer, '\n', boost::asio::use_awaitable);
    std::istream is(&buffer);
    std::string line;
    std::getline(is, line);
    co_return line;
}

boost::asio::awaitable<void> TcpConnection::asyncWriteLine(const std::string& data) {
    auto local = data;
    co_await boost::asio::async_write(socket, boost::asio::buffer(local), boost::asio::use_awaitable);
}
