#include <bkfetch/cpu.hpp>
#include <bkfetch/gpu.hpp>
#include <bkfetch/ram.hpp>
#include <bkfetch/os.hpp>
#include <bkfetch/kernel.hpp>
#include <bkfetch/disk.hpp>
#include <bkfetch/identity.hpp>
#include <bkfetch/shell.hpp>
#include <bkfetch/desktop.hpp>
#include <bkfetch/uptime.hpp>
#include <bkfetch/battery.hpp>
#include <bkfetch/logo.hpp>
#include <bkfetch/pci_db.hpp>

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {
    void printHelp() {
        std::cout << "Usage: bkfetch [options]\n\n"
                  << "Options:\n"
                  << "  -h, --help                         show this help message\n"
                  << "  -v, --version                      show version info\n"
                  << "  --no-logo                          hide ascii logo\n"
                  << "  --no-gpu                           hide gpu info\n"
                  << "  --no-disks                         hide disk info\n"
                  << "  --no-battery                       hide battery info\n"
                  << "  --test-gpumodels <vendor> <device> test pci gpu lookup (e.g. 0x10DE 0x28E0)\n";
    }

    bool parseHex16(const std::string& str, std::uint16_t& out) {
        try {
            std::size_t idx = 0;
            const unsigned long val = std::stoul(str, &idx, 16);
            if (idx != str.size() || val > 0xFFFF) {
                return false;
            }
            out = static_cast<std::uint16_t>(val);
            return true;
        } catch (...) {
            return false;
        }
    }
}

int main(int argc, char* argv[]) {
    bool showLogo = true;
    bool showGPU = true;
    bool showDisks = true;
    bool showBattery = true;

    // parse cli arguments
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printHelp();
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "bkfetch 0.1.0\n";
            return 0;
        } else if (arg == "--no-logo") {
            showLogo = false;
        } else if (arg == "--no-gpu") {
            showGPU = false;
        } else if (arg == "--no-disks") {
            showDisks = false;
        } else if (arg == "--no-battery") {
            showBattery = false;
        } else if (arg == "--test-gpumodels") {
            if (i + 2 >= argc) {
                std::cerr << "bkfetch: --test-gpumodels requires <vendor_id> and <device_id>\n";
                return 1;
            }

            std::uint16_t vendorId = 0;
            std::uint16_t deviceId = 0;

            if (!parseHex16(argv[i + 1], vendorId) || !parseHex16(argv[i + 2], deviceId)) {
                std::cerr << "bkfetch: invalid hex id (expected format like 0x10DE 0x28E0)\n";
                return 1;
            }

            const bkfetch::PCIDevice gpu = bkfetch::lookupPCIDevice(vendorId, deviceId);

            std::cout << std::hex << std::uppercase << std::setfill('0')
                      << "Vendor: 0x" << std::setw(4) << vendorId << " (" << gpu.vendor << ")\n"
                      << "Device: 0x" << std::setw(4) << deviceId << " (" << gpu.model << ")\n";
            return 0;
        } else if (arg == "--test-all-gpumodels") {
            const auto allDevices = bkfetch::getAllPCIDevices();
            bool failed = false;

            for (const auto& [vendorId, deviceId] : allDevices) {
                const bkfetch::PCIDevice gpu = bkfetch::lookupPCIDevice(vendorId, deviceId);

                std::cout << std::hex << std::uppercase << std::setfill('0')
                          << "0x" << std::setw(4) << vendorId << " : "
                          << "0x" << std::setw(4) << deviceId << " -> "
                          << gpu.vendor << " " << gpu.model << '\n';

                if (gpu.vendor == "Unknown vendor" || gpu.model == "Unknown device") {
                    failed = true;
                }
            }

            return failed ? 1 : 0;
        } else {
            std::cerr << "bkfetch: unknown option '" << arg << "'\n"
                      << "Try 'bkfetch --help' for more information.\n";
            return 1;
        }
    }

    std::string username;
    std::string hostname;
    std::string cpu;
    std::string gpu;
    std::string ram;
    std::string os;
    std::string kernel;
    std::vector<bkfetch::DiskInfo> disks;
    std::string shell;
    std::string desktop;
    std::string uptime;
    std::string battery;

    // required detectors
    const bool cpuOK = bkfetch::getCPU(cpu);
    const bool gpuOK = showGPU && bkfetch::getGPU(gpu);
    const bool ramOK = bkfetch::getRAM(ram);
    const bool osOK = bkfetch::getOS(os);
    const bool kernelOK = bkfetch::getKernel(kernel);
    const bool diskOK = showDisks && bkfetch::getDisks(disks);

    // optional detectors
    const bool usernameOK = bkfetch::getUsername(username);
    const bool hostnameOK = bkfetch::getHostname(hostname);
    const bool shellOK = bkfetch::getShell(shell);
    const bool desktopOK = bkfetch::getDEWM(desktop);
    const bool uptimeOK = bkfetch::getUptime(uptime);
    const bool batteryOK = showBattery && bkfetch::getBattery(battery);

    const auto& logo = bkfetch::getLogo();

    std::vector<std::string> info = {
        "=== bkfetch ===",
        "",
        (usernameOK ? username : "unknown") + "@" +
            (hostnameOK ? hostname : "unknown"),
        "",
        "OS: " + (osOK ? os : "N/A"),
        "Kernel: " + (kernelOK ? kernel : "N/A"),
        "CPU: " + (cpuOK ? cpu : "N/A")
    };

    if (showGPU) {
        info.push_back("GPU: " + (gpuOK ? gpu : "N/A"));
    }

    info.push_back("RAM: " + (ramOK ? ram : "N/A"));

    if (showDisks) {
        if (diskOK) {
            for (const auto& disk : disks) {
                info.push_back(
                    "Disk (" + disk.mountPoint + "): " + disk.usage
                );
            }
        } else {
            info.push_back("Disk: N/A");
        }
    }

    info.push_back("Shell: " + (shellOK ? shell : "N/A"));
    info.push_back("DE/WM: " + (desktopOK ? desktop : "Not detected"));
    info.push_back("Uptime: " + (uptimeOK ? uptime : "N/A"));

    if (showBattery) {
        info.push_back("Battery: " + (batteryOK ? battery : "Not detected"));
    }

    std::cout << '\n';

    if (!showLogo) {
        for (const auto& line : info) {
            std::cout << line << '\n';
        }
        return 0;
    }

    const std::size_t lines = std::max(logo.size(), info.size());

    for (std::size_t i = 0; i < lines; ++i) {
        const std::string left = i < logo.size() ? logo[i] : "";
        const std::string right = i < info.size() ? info[i] : "";

        std::cout << left << "   " << right << '\n';
    }
}
