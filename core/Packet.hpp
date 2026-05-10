//
// Created by divmone on 5/9/2026.
//

#pragma once

#include <json.hpp>

using json = nlohmann::json;

struct Packet {
    int64_t timestamp{0};
    float x{0.0};
    float y{0.0};
    float z{0.0};

    static Packet fromJSON(const json&);
    json toJSON();
};
