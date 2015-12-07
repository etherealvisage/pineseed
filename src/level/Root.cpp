#include <cstring>

#include <boost/make_shared.hpp>

#include "kriti/ResourceRegistry.h"
#include "kriti/XMLResource.h"

#include "Root.h"
#include "Platform.h"

namespace Pineseed {
namespace Level {

bool Root::loadFrom(std::string name) {
    auto xml =
        Kriti::ResourceRegistry::get<Kriti::XMLResource>("levels/" + name);

    if(!xml) return false;

    return true;
}

}  // namespace Level
}  // namespace Pineseed
