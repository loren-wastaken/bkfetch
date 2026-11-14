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

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

#include <iostream>
#include <string>
#include <vector>

int main() {
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
    const bool gpuOK = bkfetch::getGPU(gpu);
    const bool ramOK = bkfetch::getRAM(ram);
    const bool osOK = bkfetch::getOS(os);
    const bool kernelOK = bkfetch::getKernel(kernel);
    const bool diskOK = bkfetch::getDisks(disks);

    // optional detectors
    const bool usernameOK = bkfetch::getUsername(username);
    const bool hostnameOK = bkfetch::getHostname(hostname);
    const bool shellOK = bkfetch::getShell(shell);
    const bool desktopOK = bkfetch::getDEWM(desktop);
    const bool uptimeOK = bkfetch::getUptime(uptime);
    const bool batteryOK = bkfetch::getBattery(battery);

const auto& logo = bkfetch::getLogo();

    std::vector<std::string> info = {
        "=== bkfetch ===",
        "",
        (usernameOK ? username : "unknown") + "@" +
            (hostnameOK ? hostname : "unknown"),
        "",
        "OS: " + (osOK ? os : "N/A"),
        "Kernel: " + (kernelOK ? kernel : "N/A"),
        "CPU: " + (cpuOK ? cpu : "N/A"),
        "GPU: " + (gpuOK ? gpu : "N/A"),
        "RAM: " + (ramOK ? ram : "N/A")
    };

    if (diskOK) {
        for (const auto& disk : disks) {
            info.push_back(
                "Disk (" + disk.mountPoint + "): " + disk.usage
            );
        }
    } else {
        info.push_back("Disk: N/A");
    }

    info.push_back("Shell: " + (shellOK ? shell : "N/A"));
    info.push_back("DE/WM: " + (desktopOK ? desktop : "Not detected"));
    info.push_back("Uptime: " + (uptimeOK ? uptime : "N/A"));
    info.push_back("Battery: " + (batteryOK ? battery : "Not detected"));

    std::cout << '\n';

    const std::size_t lines = std::max(logo.size(), info.size());

    for (std::size_t i = 0; i < lines; ++i) {
        const std::string left = i < logo.size() ? logo[i] : "";

        const std::string right = i < info.size() ? info[i] : "";

        std::cout << left << "   " << right << '\n';
    }
}