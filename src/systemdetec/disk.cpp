#include <bkfetch/disk.hpp>

#include <mntent.h>
#include <sys/statvfs.h>

#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace bkfetch {

namespace {

std::string decodeMountPath(const std::string& path) {
    std::string result;

    for (std::size_t i = 0; i < path.size(); ++i) {
        if (path[i] == '\\' && i + 3 < path.size() &&
            path[i + 1] >= '0' && path[i + 1] <= '7' &&
            path[i + 2] >= '0' && path[i + 2] <= '7' &&
            path[i + 3] >= '0' && path[i + 3] <= '7') {

            const int value =
                (path[i + 1] - '0') * 64 +
                (path[i + 2] - '0') * 8 +
                (path[i + 3] - '0');

            result += static_cast<char>(value);
            i += 3;
        } else {
            result += path[i];
        }
    }

    return result;
}

bool isVirtualFilesystem(const std::string& type) {
    static const std::unordered_set<std::string> virtualFilesystems = {
        "proc",
        "sysfs",
        "devtmpfs",
        "devpts",
        "tmpfs",
        "cgroup",
        "cgroup2",
        "pstore",
        "securityfs",
        "debugfs",
        "tracefs",
        "configfs",
        "fusectl",
        "mqueue",
        "hugetlbfs",
        "rpc_pipefs",
        "autofs",
        "efivarfs",
        "bpf",
        "binfmt_misc"
    };
    return virtualFilesystems.count(type) > 0;
}

}

bool getDisks(std::vector<DiskInfo>& disks) {
    FILE* mounts = setmntent("/proc/self/mounts", "r");

    if (mounts == nullptr) {
        return false;
    }

    disks.clear();

    struct mntent* entry;

    while ((entry = getmntent(mounts)) != nullptr) {
        const std::string filesystem = entry->mnt_type;
        const std::string mountPoint = decodeMountPath(entry->mnt_dir);

        if (isVirtualFilesystem(filesystem)) {
            continue;
        }

        struct statvfs info {};

        if (statvfs(mountPoint.c_str(), &info) != 0) {
            continue;
        }

        if (info.f_blocks == 0 || info.f_frsize == 0) {
            continue;
        }

        const long double total =
            static_cast<long double>(info.f_blocks) * info.f_frsize;

        const long double free =
            static_cast<long double>(info.f_bfree) * info.f_frsize;

        const long double used = total - free;

        constexpr long double gib =
            1024.0L * 1024.0L * 1024.0L;

        std::ostringstream output;

        output << std::fixed << std::setprecision(2)
               << used / gib << " GiB / "
               << total / gib << " GiB";

        disks.push_back({
            mountPoint,
            output.str()
        });
    }

    endmntent(mounts);

    // keep the root filesystem at the top
    std::sort(disks.begin(), disks.end(),
        [](const DiskInfo& a, const DiskInfo& b) {
            if (a.mountPoint == "/") return true;
            if (b.mountPoint == "/") return false;

            return a.mountPoint < b.mountPoint;
        });

    return !disks.empty();
}

}