//
// Created by divmone on 5/9/2026.
//


#include "Packet.hpp"

Packet Packet::fromJSON(const json &j) {
    Packet packet;
    packet.timestamp = j.at("timestamp").get<int64_t>();
    packet.x = j.at("x").get<float>();
    packet.y = j.at("y").get<float>();
    packet.z = j.at("z").get<float>();

    return packet;
}

json Packet::toJSON() {
    return {
        {"timestamp", timestamp},
        {"x", x},
        {"y", y},
        {"z", z}
    };
}
