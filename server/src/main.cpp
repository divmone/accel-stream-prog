#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

#include "AsyncServer.hpp"

using tcp = boost::asio::ip::tcp;

int main(int argc, const char* argv[]) {
    boost::asio::io_context io;

    uint16_t portA = argc > 1 ? std::stoi(argv[1]) : 9000;
    uint16_t portB = argc > 2 ? std::stoi(argv[2]) : 9001;

    boost::asio::co_spawn(io,
        [&]() -> boost::asio::awaitable<void> {
            AsyncServer server(io, portA, portB);
            co_await server.run();
        },
        boost::asio::detached);

    boost::asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&io](auto, auto) {
        io.stop();
    });

    spdlog::info("[Server] Starting on ports {} and {}", portA, portB);

    io.run();

    spdlog::info("[Server] Stopped");
}