//
// Created by divmone on 5/10/2026.
//

#pragma once

#include "Protocol.hpp"

class AccelService {
public:
    AccelService(double frequency, double amplitude)
        : startTime_(std::chrono::steady_clock::now())
        , frequency_(frequency), amplitude_(amplitude){
    }

    AccelPacket next() {
        auto t = std::chrono::duration<double>(std::chrono::steady_clock::now() - startTime_).count();
        double v = amplitude_ * std::sin(2 * std::numbers::pi * frequency_ * t);
        return {nowMs(), v, v, v};
    }

private:
    std::chrono::steady_clock::time_point startTime_;
    double frequency_, amplitude_;
    static int64_t nowMs() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
};

