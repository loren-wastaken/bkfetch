#include <bkfetch/kernel.hpp>

#include <sys/utsname.h>

namespace bkfetch {

bool getKernel(std::string& kernel) {
    struct utsname info {};

    if (uname(&info) != 0) {
        return false;
    }

    kernel = info.release;

    return !kernel.empty();
}

}