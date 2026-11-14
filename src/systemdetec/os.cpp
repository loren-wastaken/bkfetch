#include <bkfetch/os.hpp>

#include <fstream>
#include <string>

namespace bkfetch {

bool getOS(std::string& os) {
    std::ifstream file("/etc/os-release");

    if (!file.is_open()) {
        return false;
    }

    std::string line;

    while (std::getline(file, line)) {
        const std::string key = "PRETTY_NAME=";

        if (line.rfind(key, 0) == 0) {
            os = line.substr(key.size());

            // remove surrounding quotes
            if (os.size() >= 2 &&
                ((os.front() == '"' && os.back() == '"') ||
                 (os.front() == '\'' && os.back() == '\''))) {
                os = os.substr(1, os.size() - 2);
            }

            return !os.empty();
        }
    }

    return false;
}

}