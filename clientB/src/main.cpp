//
// Created by divmone on 5/9/2026.
//
#include <iostream>
#include <fstream>
#include <thread>
#include <filesystem>
#include "ModuleService.hpp"
#include "TcpClient.hpp"
#include <Protocol.hpp>
#include <filesystem>
#include <CoroRuntime.hpp>
#include "ModuleService.hpp"
#include "CoroRuntime.hpp"
#include "TcpClient.hpp"
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

boost::asio::awaitable<void> runClientB(boost::asio::io_context& io) {
    spdlog::info("[ClientB] starting");
    auto conn = co_await TcpClient::connect(io, "127.0.0.1", 9001);
    spdlog::info("[ClientB] processing loop started");
    co_await runCoro("NodeB reader/writer",
        [conn]() -> boost::asio::awaitable<void> {
            while (true) {
                auto line = co_await conn->asyncReadLine();
                if (line.empty()) {
                    spdlog::warn("[ClientB] received empty line, skipping");
                    continue;
                }
                auto accel  = AccelPacket::fromString(line);
                auto module = ModuleService::calcModule(accel);
                co_await conn->asyncWriteLine(module.toJSON().dump() + "\n");
                spdlog::info("[ClientB] accel ts={} -> module={}",
                             accel.timestamp, module.module);
            }
        });
}

int main() {
    return run("ClientB",
        [](auto& io) { return runClientB(io); });
}
