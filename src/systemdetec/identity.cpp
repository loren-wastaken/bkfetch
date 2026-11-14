#include <bkfetch/identity.hpp>

#include <unistd.h>
#include <pwd.h>

#include <array>
#include <string>

namespace bkfetch {

bool getUsername(std::string& username) {
    passwd* user = getpwuid(getuid());

    if (user == nullptr || user->pw_name == nullptr) {
        return false;
    }

    username = user->pw_name;

    return !username.empty();
}

bool getHostname(std::string& hostname) {
    std::array<char, 256> buffer{};

    if (gethostname(buffer.data(), buffer.size() - 1) != 0) {
        return false;
    }

    hostname = buffer.data();

    return !hostname.empty();
}

}