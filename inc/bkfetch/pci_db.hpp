#pragma once

#include <cstdint>
#include <string>

namespace bkfetch {

struct PCIDevice {
    std::string vendor;
    std::string model;
};

PCIDevice lookupPCIDevice(
    std::uint16_t vendorID,
    std::uint16_t deviceID
);

}
