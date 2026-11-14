#pragma once

#include <string>
#include <vector>

namespace bkfetch {

struct DiskInfo {
    std::string mountPoint;
    std::string usage;
};

bool getDisks(std::vector<DiskInfo>& disks);

}