//
// Created by divmone on 5/9/2026.
//
#include <chrono>

#include "ModuleService.hpp"
#include "TcpClient.hpp"
#include <Protocol.hpp>
#include <CoroRuntime.hpp>
#include <spdlog/spdlog.h>

boost::asio::awaitable<void> runClientB(boost::asio::io_context& io) {
    auto handler = [](auto conn) -> boost::asio::awaitable<void> {
        while (true) {
            auto line = co_await conn->asyncReadLine();
            if (line.empty()) {
                spdlog::warn("[ClientB] received empty line, skipping");
                continue;
            }
            auto accelPacket  = AccelPacket::fromString(line);
            auto module = accel::calcModule(accelPacket);
            co_await conn->asyncWriteLine(module.toJSON().dump() + "\n");
            spdlog::info("[ClientB] accel ts={} -> module={}",
                         accelPacket.timestamp, module.module);
        }
    };

    return runTcpClient("ClientB", io, "127.0.0.1", 9001, handler);
}

int main() {
    return run("ClientB",
        [](auto& io) { return runClientB(io); });
}
