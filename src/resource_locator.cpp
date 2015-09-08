#include "resource_locator.hpp"

std::string resource_locator::locate(std::string name) {
    return "data/" + name;
}
