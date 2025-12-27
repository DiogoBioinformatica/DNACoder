#pragma once
#include <string>
#include <cstdint>

namespace engine {

struct Analysis {
    uint64_t length = 0;
    uint64_t checksum = 0;
    double gccontent = 0.0;
};

Analysis analyze_base(const std::string& bases);

}
