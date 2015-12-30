#include <boost/make_shared.hpp>

#include "kriti/scene/Camera.h"
#include "kriti/math/ViewGenerator.h"
#include "kriti/math/Vector.h"

#include "Camera.h"

namespace Pineseed {
namespace Scene {

Camera::Camera() {
    setProjection(Kriti::Math::ViewGenerator().orthogonal(
        -0.6, 16/9 - 0.6, -0.5, 0.5, -10.0, 10.0));
}

}  // namespace Scene
}  // namespace Pineseed
