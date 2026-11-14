#include <bkfetch/desktop.hpp>

#include <cstdlib>
#include <string>
#include <vector>

namespace bkfetch {

bool getDEWM(std::string& desktop) {
    auto getEnv = [](const char* name) -> std::string {
        const char* value = std::getenv(name);

        return value != nullptr ? value : "";
    };

    // detect common window managers and compositors
    if (!getEnv("HYPRLAND_INSTANCE_SIGNATURE").empty()) {
        desktop = "Hyprland";
        return true;
    }

    if (!getEnv("SWAYSOCK").empty()) {
        desktop = "Sway";
        return true;
    }

    if (!getEnv("WAYFIRE_SOCKET").empty()) {
        desktop = "Wayfire";
        return true;
    }

    if (!getEnv("I3SOCK").empty()) {
        desktop = "i3";
        return true;
    }

    // detect desktop environments
    const std::string current = getEnv("XDG_CURRENT_DESKTOP");
    const std::string session = getEnv("DESKTOP_SESSION");

    const std::vector<std::pair<std::string, std::string>> desktops = {
        {"KDE", "KDE Plasma"},
        {"GNOME", "GNOME"},
        {"XFCE", "Xfce"},
        {"LXQt", "LXQt"},
        {"LXDE", "LXDE"},
        {"Cinnamon", "Cinnamon"},
        {"MATE", "MATE"},
        {"COSMIC", "COSMIC"}
    };

    for (const auto& [identifier, name] : desktops) {
        if (current.find(identifier) != std::string::npos ||
            session.find(identifier) != std::string::npos) {
            desktop = name;
            return true;
        }
    }

    return false;
}

}