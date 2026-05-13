//
// Created by divmone on 5/12/2026.
//

#include "Session.hpp"
#include <spdlog/spdlog.h>

Session::Session(std::shared_ptr<TcpConnection> a,
                 std::shared_ptr<TcpConnection> b) : connA(a), connB(b) {
}

boost::asio::awaitable<void> Session::run() {
    spdlog::info("[Session] started, forwarding A<->B");
    auto executor = co_await boost::asio::this_coro::executor;
    auto self = shared_from_this();

    boost::asio::co_spawn(executor,
                          [self]() { return self->forwardAtoB(); },
                          boost::asio::detached);

    boost::asio::co_spawn(executor,
                          [self]() { return self->forwardBtoA(); },
                          boost::asio::detached);
}

bool Session::filterPackets(const AccelPacket &packetA,
                            const AccelPacket &packetB) {
    constexpr double eps = 0.0001;
    auto isDublicate = [eps](double a, double b) -> bool {
        return std::abs(a - b) < eps;
    };

    return isDublicate(packetA.x, packetB.x)
           && isDublicate(packetA.y, packetB.y)
           && isDublicate(packetA.z, packetB.z);
}

boost::asio::awaitable<void> Session::forwardAtoB() {
    try {
        auto currentPacket = AccelPacket();
        auto lastPacket = AccelPacket();

        while (true) {
            const auto line = co_await connA->asyncReadLine();
            currentPacket = AccelPacket::fromString(line);

            if (filterPackets(currentPacket, lastPacket)) {
                spdlog::warn("[Session] duplicate packet dropped (ts={})", currentPacket.timestamp);
                continue;
            }

            co_await connB->asyncWriteLine(line + "\n");
            spdlog::info("[Session] A->B: {}", line);

            lastPacket = currentPacket;
        }
    } catch (const std::exception &e) {
        spdlog::error("[Session] A->B: {}", e.what());
    }
}

boost::asio::awaitable<void> Session::forwardBtoA() {
    try {
        while (true) {
            const auto line = co_await connB->asyncReadLine();
            co_await connA->asyncWriteLine(line + "\n");
            spdlog::info("[Session] B->A: {}", line);
        }
    } catch (const std::exception &e) {
        spdlog::error("[Session] B->A: {}", e.what());
    }
}
