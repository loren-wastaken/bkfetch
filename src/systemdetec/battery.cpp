#include <bkfetch/battery.hpp>

#include <filesystem>
#include <fstream>
#include <string>

namespace bkfetch {

bool getBattery(std::string& battery) {
    const std::filesystem::path powerSupply("/sys/class/power_supply");

    if (!std::filesystem::exists(powerSupply)) {
        return false;
    }

    for (const auto& entry :
         std::filesystem::directory_iterator(powerSupply)) {

        std::ifstream typeFile(entry.path() / "type");

        if (!typeFile.is_open()) {
            continue;
        }

        std::string type;
        std::getline(typeFile, type);

        if (type != "Battery") {
            continue;
        }

        std::ifstream capacityFile(entry.path() / "capacity");

        if (!capacityFile.is_open()) {
            continue;
        }

        int capacity = 0;

        if (!(capacityFile >> capacity) || capacity < 0 || capacity > 100) {
            continue;
        }

        std::string status = "Unknown";

        std::ifstream statusFile(entry.path() / "status");

        if (statusFile.is_open()) {
            std::getline(statusFile, status);
        }

        battery = std::to_string(capacity) + "% (" + status + ")";

        return true;
    }

    return false;
}

}