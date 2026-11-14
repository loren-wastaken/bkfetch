#include <bkfetch/shell.hpp>

#include <cstdlib>
#include <filesystem>

namespace bkfetch {

bool getShell(std::string& shell) {
    const char* envShell = std::getenv("SHELL");

    if (envShell == nullptr || *envShell == '\0') {
        return false;
    }

    shell = std::filesystem::path(envShell).filename().string();

    return !shell.empty();
}

}