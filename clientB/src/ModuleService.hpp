//
// Created by divmone on 5/12/2026.
//

#pragma once

#include <Protocol.hpp>
#include <chrono>

namespace accel {
    ModulePacket calcModule(const AccelPacket& p) {
        return {
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(),
            std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z)};
    }
};


