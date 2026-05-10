//
// Created by divmone on 5/10/2026.
//

#include "AccelService.hpp"
#include <random>
#include <chrono>

Packet AccelService::genRandomPacket() {
    return {
        .timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count(),
        .x = 1,
        .y = 2,
        .z = 3
    };
}
