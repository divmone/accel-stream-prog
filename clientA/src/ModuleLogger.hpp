//
// Created by divmone on 5/14/2026.
//

#pragma once

#include <fstream>
#include <string>
#include <spdlog/spdlog.h>
#include <Protocol.hpp>

class ModuleLogger {
public:
    ModuleLogger(const std::string &path) {
        file.open(path, std::ios::app);
        if (!file) {
            spdlog::error("[ModuleLogger] failed to open log file {}", path);
            throw std::runtime_error(
                "ModuleLogger: cannot open " + path);
        }

        spdlog::info("[ModuleLogger] writing to {}", path);
    }

    void log(const ModulePacket &packet) {
        file << "timestamp: " << packet.timestamp
                << " module: " << packet.module << '\n';
        file.flush();
    }

private:
    std::ofstream file;
};
