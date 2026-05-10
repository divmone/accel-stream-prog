//
// Created by divmone on 5/9/2026.
//

#include <fstream>
#include <iostream>
#include <filesystem>
#include <thread>

#include "AccelService.hpp"
#include "TcpClient.hpp"

namespace fs = std::filesystem;

int main() {
    try {
        TcpClient client("127.0.0.1", 9000);

        fs::path logPath = fs::path("logs") / "accel_module.log";
        fs::create_directories(logPath.parent_path());

        std::ofstream file(logPath, std::ios::app);
        if (!file)
            throw std::runtime_error("Failed to open log file");

        while (true) {
            Packet packet = AccelService::genRandomPacket();
            const auto stringPacket = packet.toJSON().dump(4) + "\n";
            client.send(stringPacket);
            const auto response = client.readLine();
            const json j = json::parse(response);;
            file << "timestamp: " << j.at("timestamp").get<int64_t>() << " "
                << "module: " << j.at("module").get<float>() << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}