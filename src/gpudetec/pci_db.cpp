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
                // GeForce RTX 50 Series (Blackwell)
                {0x2B85, "GeForce RTX 5090"},
                {0x2C02, "GeForce RTX 5080"},
                {0x2C05, "GeForce RTX 5070 Ti"},
                {0x2F04, "GeForce RTX 5070"},
                {0x2D04, "GeForce RTX 5060 Ti"},
                {0x2D05, "GeForce RTX 5060"},

                // GeForce RTX 40 Series (Desktop)
                {0x2684, "GeForce RTX 4090"},
                {0x2685, "GeForce RTX 4090 D"},
                {0x2702, "GeForce RTX 4080 Super"},
                {0x2704, "GeForce RTX 4080"},
                {0x2705, "GeForce RTX 4070 Ti Super"},
                {0x2689, "GeForce RTX 4070 Ti Super"},
                {0x2782, "GeForce RTX 4070 Ti"},
                {0x2783, "GeForce RTX 4070 Super"},
                {0x2786, "GeForce RTX 4070"},
                {0x2709, "GeForce RTX 4070"},
                {0x2803, "GeForce RTX 4060 Ti"},
                {0x2805, "GeForce RTX 4060 Ti"},
                {0x2788, "GeForce RTX 4060 Ti"},
                {0x2882, "GeForce RTX 4060"},

                // GeForce RTX 40 Series (Mobile)
                {0x2717, "GeForce RTX 4090 Max-Q / Mobile"},
                {0x2757, "GeForce RTX 4090 Max-Q / Mobile"},
                {0x27A0, "GeForce RTX 4080 Max-Q / Mobile"},
                {0x27E0, "GeForce RTX 4080 Max-Q / Mobile"},
                {0x2820, "GeForce RTX 4070 Max-Q / Mobile"},
                {0x2860, "GeForce RTX 4070 Max-Q / Mobile"},
                {0x28A0, "GeForce RTX 4060 Max-Q / Mobile"},
                {0x28E0, "GeForce RTX 4060 Max-Q / Mobile"},
                {0x28A1, "GeForce RTX 4050 Max-Q / Mobile"},
                {0x28E1, "GeForce RTX 4050 Max-Q / Mobile"}
            }
        }
    },
    {
        0x1002,
        {
            "AMD",
            {
                // Radeon RX 9000 Series (RDNA 4)
                {0x7550, "Radeon RX 9070 / 9070 XT"},
                {0x7590, "Radeon RX 9060 XT"},

                // Radeon RX 7000 Series (RDNA 3)
                {0x744C, "Radeon RX 7900 XTX / XT / GRE"},
                {0x747E, "Radeon RX 7800 XT / 7700 XT"},
                {0x7480, "Radeon RX 7600 / 7600 XT"},
                {0x7483, "Radeon RX 7600M"},

                // Integrated RDNA 3 / 3.5 Mobile Graphics
                {0x15BF, "Radeon 780M / 760M"},
                {0x1900, "Radeon 780M / 760M"},
                {0x150E, "Radeon 890M / 880M"}
            }
        }
    },
    {
        0x8086,
        {
            "Intel",
            {
                // Arc B-Series (Battlemage)
                {0xE20B, "Arc B580"},
                {0xE20C, "Arc B570"},
                {0xE211, "Arc Pro B60"},
                {0xE212, "Arc Pro B50"},

                // Arc A-Series (Alchemist)
                {0x56A0, "Arc A770"},
                {0x56A1, "Arc A750"},
                {0x56A2, "Arc A580"},
                {0x56A5, "Arc A380"},
                {0x56A6, "Arc A310"},

                // Arc Mobile / Integrated (Meteor Lake & Lunar Lake)
                {0x7D55, "Arc Graphics (Meteor Lake)"},
                {0x64A0, "Arc Graphics (Lunar Lake)"}
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
std::vector<std::pair<std::uint16_t, std::uint16_t>> getAllPCIDevices() {
    std::vector<std::pair<std::uint16_t, std::uint16_t>> list;

    for (const auto& [vendorId, vendor] : pciDatabase) {
        for (const auto& [deviceId, model] : vendor.devices) {
            list.emplace_back(vendorId, deviceId);
        }
    }

    return list;
}

}
