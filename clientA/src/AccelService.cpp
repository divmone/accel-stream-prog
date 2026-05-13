//
// Created by divmone on 5/10/2026.
//

#include "AccelService.hpp"
#include <random>
#include <chrono>

AccelPacket AccelService::genRandomPacket() {
    return {
        .timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count(),
        .x = 1,
        .y = 2,
        .z = 3
    };
}

AccelPacket AccelService::genSinPacket() {
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    static auto startTime = std::chrono::system_clock::now();

    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration<double>(now - startTime);
    double t = duration.count();

    auto x = std::sin(2 * std::numbers::pi * 0.3 * t);
    auto y = std::sin(2 * std::numbers::pi * 0.3 * t);
    auto z = std::sin(2 * std::numbers::pi * 0.3 * t);

    return {
        .timestamp = timestamp,
        .x = x,
        .y = y,
        .z = z
    };
}


