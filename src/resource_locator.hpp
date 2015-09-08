#ifndef RESOURCE_LOCATOR_HPP
#define RESOURCE_LOCATOR_HPP

#include <string>

class resource_locator {
public:
    resource_locator();

    std::string locate(std::string name);
};

#endif
