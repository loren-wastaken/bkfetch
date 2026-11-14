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

    std::cout << "=== bkfetch ===\n\n";

    std::cout
        << (usernameOK ? username : "unknown")
        << "@"
        << (hostnameOK ? hostname : "unknown")
        << "\n\n";

    std::cout << "OS: " << (osOK ? os : "N/A") << '\n';
    std::cout << "Kernel: " << (kernelOK ? kernel : "N/A") << '\n';
    std::cout << "CPU: " << (cpuOK ? cpu : "N/A") << '\n';
    std::cout << "GPU: " << (gpuOK ? gpu : "N/A") << '\n';
    std::cout << "RAM: " << (ramOK ? ram : "N/A") << '\n';
    if (diskOK) {
        for (const auto& disk : disks) {
            std::cout << "Disk (" << disk.mountPoint << "): "
                    << disk.usage << '\n';
        }
    } else {
        std::cout << "Disk: N/A\n";
    }
    std::cout << "Shell: " << (shellOK ? shell : "N/A") << '\n';
    std::cout << "DE/WM: " << (desktopOK ? desktop : "Not detected") << '\n';
    std::cout << "Uptime: " << (uptimeOK ? uptime : "N/A") << '\n';
    std::cout << "Battery: " << (batteryOK ? battery : "Not detected") << '\n';

    return (
        cpuOK &&
        gpuOK &&
        ramOK &&
        osOK &&
        kernelOK &&
        diskOK
    ) ? 0 : 1;
}