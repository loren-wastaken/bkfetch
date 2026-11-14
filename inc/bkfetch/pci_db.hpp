#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace bkfetch {

std::vector<std::pair<std::uint16_t, std::uint16_t>> getAllPCIDevices();

struct PCIDevice {
    std::string vendor;
    std::string model;
};

PCIDevice lookupPCIDevice(
    std::uint16_t vendorID,
    std::uint16_t deviceID
);

}
