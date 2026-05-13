#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>

#include "AccelService.hpp"
#include "CoroRuntime.hpp"
#include "TcpClient.hpp"
#include "spdlog/spdlog.h"

namespace fs = std::filesystem;

boost::asio::awaitable<void> runClientA(boost::asio::io_context &io) {
    spdlog::info("[ClientA] starting");
    auto conn = co_await TcpClient::connect(io, "127.0.0.1", 9000);
    auto ex = co_await boost::asio::this_coro::executor;

    fs::path logPath = fs::path("logs") / "accel_module.log";
    fs::create_directories(logPath.parent_path());
    spdlog::info("[ClientA] writing modules to {}", logPath.string());

    boost::asio::co_spawn(ex,
        runCoro("NodeA reader", [conn, logPath]() -> boost::asio::awaitable<void> {
            std::ofstream file(logPath, std::ios::app);
            if (!file) {
                spdlog::error("[ClientA] failed to open log file {}", logPath.string());
                throw std::runtime_error("Failed to open log file");
            }
            spdlog::info("[ClientA] reader started");

            while (true) {
                auto response = co_await conn->asyncReadLine();
                ModulePacket packet = ModulePacket::fromString(response);
                spdlog::info("[ClientA] received module ts={} value={}", packet.timestamp, packet.module);
                file << "timestamp: " << packet.timestamp
                     << " module: "   << packet.module << '\n';
                file.flush();
            }
        }),
    boost::asio::detached);

    boost::asio::co_spawn(ex,
        runCoro("NodeA writer", [conn, ex]() -> boost::asio::awaitable<void> {
            constexpr auto INTERVAL = std::chrono::milliseconds(20);
            boost::asio::steady_timer timer(ex);
            spdlog::info("[ClientA] writer started, interval {}ms", INTERVAL.count());

            while (true) {
                auto packet = AccelService::genSinPacket();
                co_await conn->asyncWriteLine(packet.toJSON().dump() + "\n");
                spdlog::debug("[ClientA] sent accel ts={} x={} y={} z={}",
                              packet.timestamp, packet.x, packet.y, packet.z);
                timer.expires_after(INTERVAL);
                co_await timer.async_wait(boost::asio::use_awaitable);
            }
        }),
    boost::asio::detached);
}

int main() {
    return run("ClientA", [](auto &io) {
        return runClientA(io);
    });
}