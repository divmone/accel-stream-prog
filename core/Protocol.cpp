//
// Created by divmone on 5/9/2026.
//

#include "Protocol.hpp"

AccelPacket AccelPacket::fromJSON(const json &j) {
    AccelPacket packet;
    packet.timestamp = j.at("timestamp").get<int64_t>();
    packet.x = j.at("x").get<double>();
    packet.y = j.at("y").get<double>();
    packet.z = j.at("z").get<double>();

    return packet;
}

json AccelPacket::toJSON() {
    return {
        {"timestamp", timestamp},
        {"x", x},
        {"y", y},
        {"z", z}
    };
}

AccelPacket AccelPacket::fromString(const std::string &s) {
    const json j = json::parse(s);
    const AccelPacket packet = fromJSON(j);
    return packet;
}

ModulePacket ModulePacket::fromJSON(const json &j) {
    ModulePacket packet;
    packet.timestamp = j.at("timestamp").get<int64_t>();
    packet.module = j.at("module").get<double>();
    return packet;
}

json ModulePacket::toJSON() {
    return {
        {"timestamp", timestamp},
        {"module", module}
    };
}

ModulePacket ModulePacket::fromString(const std::string &s) {
    const json j = json::parse(s);
    const ModulePacket packet = fromJSON(j);
    return packet;
}
