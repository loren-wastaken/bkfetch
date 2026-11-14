#include <bkfetch/cpu.hpp>

#include <fstream>
#include <string>

namespace bkfetch {

bool getCPU(std::string& result) {
    std::ifstream file("/proc/cpuinfo");

    if (!file.is_open())
        return false;

    std::string line;

    while (std::getline(file, line)) {
        const std::string key = "model name";

        if (line.compare(0, key.size(), key) != 0)
            continue;

        const auto pos = line.find(':');

        if (pos == std::string::npos)
            continue;

        result = line.substr(pos + 1);

        const auto start = result.find_first_not_of(" \t");

        if (start == std::string::npos)
            return false;

        result.erase(0, start);

        return !result.empty();
    }

    return false;
}

}
