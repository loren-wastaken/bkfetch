#include <bkfetch/uptime.hpp>

#include <fstream>
#include <iomanip>
#include <sstream>

namespace bkfetch {

bool getUptime(std::string& uptime) {
    std::ifstream file("/proc/uptime");

    if (!file.is_open()) {
        return false;
    }

    double seconds = 0.0;

    if (!(file >> seconds) || seconds < 0.0) {
        return false;
    }

    const long long totalSeconds =
        static_cast<long long>(seconds);

    const long long days = totalSeconds / 86400;
    const long long hours = (totalSeconds % 86400) / 3600;
    const long long minutes = (totalSeconds % 3600) / 60;

    std::ostringstream output;

    if (days > 0) {
        output << days << (days == 1 ? " day" : " days") << ", ";
    }

    if (hours > 0 || days > 0) {
        output << hours << (hours == 1 ? " hour" : " hours") << ", ";
    }

    output << minutes << (minutes == 1 ? " minute" : " minutes");

    uptime = output.str();

    return true;
}

}