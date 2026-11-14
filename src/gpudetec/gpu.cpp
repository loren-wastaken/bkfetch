#include <bkfetch/gpu.hpp>
#include <bkfetch/pci_db.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdint>

namespace bkfetch {

namespace fs = std::filesystem;

// read a pci id from sysfs
static bool readPCIValue(
    const fs::path& path,
    std::uint16_t& result
) {
    std::ifstream file(path);
    std::string value;

    if (!(file >> value))
        return false;

    try {
        const auto parsed = std::stoul(value, nullptr, 0);

        if (parsed > 0xFFFF)
            return false;

        result = static_cast<std::uint16_t>(parsed);

        return true;
    }
    catch (...) {
        return false;
    }
}

// format pci ids for unknown devices :)
static std::string formatPCI(std::uint16_t id) {
    std::ostringstream output;

    output << "0x"
           << std::uppercase
           << std::hex
           << std::setfill('0')
           << std::setw(4)
           << id;

    return output.str();
}

bool getGPU(std::string& result) {
    const fs::path pciPath = "/sys/bus/pci/devices";

    std::error_code ec;

    fs::directory_iterator it(pciPath, ec);
    const fs::directory_iterator end;

    if (ec)
        return false;

    std::ostringstream output;
    bool found = false;

    for (; it != end; it.increment(ec)) {
        if (ec)
            return false;

        const fs::path devicePath = it->path();

        // check if this is a display controller
        std::ifstream classFile(devicePath / "class");
        std::string pciClass;

        if (!(classFile >> pciClass))
            continue;

        if (pciClass.size() < 4 ||
            pciClass.substr(0, 4) != "0x03")
            continue;

        // read vendor and device ids
        std::uint16_t vendorID;
        std::uint16_t deviceID;

        if (!readPCIValue(devicePath / "vendor", vendorID) ||
            !readPCIValue(devicePath / "device", deviceID))
            continue;

        // look up the device in our database :)
        const PCIDevice gpu =
            lookupPCIDevice(vendorID, deviceID);

        if (found)
            output << ", ";

        output << gpu.vendor << " " << gpu.model;

        if (gpu.model == "Unknown device") {
            output << " (" << formatPCI(deviceID) << ")";
        }

        if (gpu.vendor == "Unknown vendor") {
            output << " (" << formatPCI(vendorID) << ")";
        }

        found = true;
    }

    if (!found) {
        result = "No PCI graphics device detected";
        return true;
    }

    result = output.str();

    return true;
}

}
