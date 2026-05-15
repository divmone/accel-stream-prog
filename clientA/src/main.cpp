#include <fstream>
#include <filesystem>
#include <chrono>

#include <boost/asio/experimental/awaitable_operators.hpp>

#include "AccelService.hpp"
#include "CoroRuntime.hpp"
#include "TcpClient.hpp"
#include "spdlog/spdlog.h"
#include "ModuleLogger.hpp"

namespace fs = std::filesystem;
using namespace boost::asio::experimental::awaitable_operators;

boost::asio::awaitable<void> runClientA(boost::asio::io_context &io) {
    fs::path logPath = fs::path("logs") / "accel_module.log";
    fs::create_directories(logPath.parent_path());

    auto handler = [logPath, &io](auto conn) -> boost::asio::awaitable<void> {
            auto reader = [conn, logPath]() -> boost::asio::awaitable<void> {
                spdlog::info("[ClientA] reader started");
                ModuleLogger logger(logPath);
                while (true) {
                    auto response = co_await conn->asyncReadLine();
                    if (response.empty()) continue;
                    ModulePacket packet = ModulePacket::fromString(response);
                    spdlog::info("[ClientA] received module ts={} value={}", packet.timestamp, packet.module);
                    logger.log(packet);
                }
            };

            auto writer = [conn, &io]() -> boost::asio::awaitable<void> {
                auto accelService = AccelService(10, 2);
                constexpr auto INTERVAL = std::chrono::milliseconds(20);
                auto ex = co_await boost::asio::this_coro::executor;
                boost::asio::steady_timer timer(io);

                spdlog::info("[ClientA] writer started, interval {}ms", INTERVAL.count());
                while (true) {
                    auto packet = accelService.next();
                    co_await conn->asyncWriteLine(packet.toJSON().dump() + "\n");
                    spdlog::debug("[ClientA] sent accel ts={} x={} y={} z={}",
                                  packet.timestamp, packet.x, packet.y, packet.z);
                    timer.expires_after(INTERVAL);
                    co_await timer.async_wait(boost::asio::use_awaitable);
                }
            };

            co_await (reader() || writer());
    };

    return runTcpClient("ClientA", io, "127.0.0.1", 9000, handler);
}
int main() {
    return run("ClientA", [](auto &io) {
        return runClientA(io);
    });
}