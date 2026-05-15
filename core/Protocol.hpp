//
// Created by divmone on 5/9/2026.
//

#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct AccelPacket {
    int64_t timestamp{0};
    double x{0.0};
    double y{0.0};
    double z{0.0};
    
    static AccelPacket fromJSON(const json&);
    json toJSON();
    static AccelPacket fromString(const std::string&);
};

struct ModulePacket{
    int64_t timestamp{0};
    double module{0.0};

    static ModulePacket fromJSON(const json&);
    json toJSON();
    static ModulePacket fromString(const std::string&);
};
