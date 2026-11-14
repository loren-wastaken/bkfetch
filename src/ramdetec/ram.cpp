#include <bkfetch/ram.hpp>

#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

namespace bkfetch {

bool getRAM(std::string& result) {
    std::ifstream file("/proc/meminfo");

    if (!file.is_open())
        return false;

    unsigned long long total = 0;
    unsigned long long available = 0;

    bool hasTotal = false;
    bool hasAvailable = false;

    std::string key;
    unsigned long long value;
    std::string unit;

    while (file >> key >> value >> unit) {
        if (key == "MemTotal:") {
            total = value;
            hasTotal = true;
        }
        else if (key == "MemAvailable:") {
            available = value;
            hasAvailable = true;
        }
    }

    if (!hasTotal || !hasAvailable || total == 0 ||
        available > total)
        return false;

    // convert kibibytes to gibibytes
    const double totalGB = total / 1024.0 / 1024.0;

    const double usedGB =
        (total - available) / 1024.0 / 1024.0;

    std::ostringstream output;

    output << std::fixed << std::setprecision(2)
           << usedGB << " GiB / "
           << totalGB << " GiB";

    result = output.str();

    return true;
}

}
