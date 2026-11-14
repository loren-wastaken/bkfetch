#include <bkfetch/pci_db.hpp>

#include <unordered_map>
#include <string>
#include <cstdint>

namespace bkfetch {

struct PCIVendor {
    std::string name;
    std::unordered_map<std::uint16_t, std::string> devices;
};

// our local pci database :)
static const std::unordered_map<std::uint16_t, PCIVendor> pciDatabase = {
    {
        0x10DE,
        {
            "NVIDIA",
            {
                {0x2882, "GeForce RTX 4060"},
                {0x2684, "GeForce RTX 4090"}
            }
        }
    },
    {
        0x1002,
        {
            "AMD",
            {
                {0x744C, "Radeon RX 7900 XTX"}
            }
        }
    },
    {
        0x8086,
        {
            "Intel",
            {
                {0x56A0, "Arc A770"},
                {0x56A1, "Arc A750"}
            }
        }
    }
};

PCIDevice lookupPCIDevice(
    std::uint16_t vendorID,
    std::uint16_t deviceID
) {
    const auto vendorIt = pciDatabase.find(vendorID);

    if (vendorIt == pciDatabase.end()) {
        return {
            "Unknown vendor",
            "Unknown device"
        };
    }

    const auto& vendor = vendorIt->second;

    const auto deviceIt = vendor.devices.find(deviceID);

    if (deviceIt == vendor.devices.end()) {
        return {
            vendor.name,
            "Unknown device"
        };
    }

    return {
        vendor.name,
        deviceIt->second
    };
}

}
