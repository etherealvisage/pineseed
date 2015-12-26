#include <boost/make_shared.hpp>

#include "kriti/scene/Camera.h"
#include "kriti/math/ViewGenerator.h"
#include "kriti/math/Vector.h"

#include "Camera.h"

namespace Pineseed {
namespace Scene {

Camera::Camera() {
    m_camera = boost::make_shared<Kriti::Scene::Camera>();
    m_camera->setProjection(
        Kriti::Math::ViewGenerator().orthogonal(
            0.0, 1.0, 0.0, 1.0,
            -10.0, 10.0));
}

void Camera::updateCamera(const Level::Pos &pos) {
    m_camera->position() = pos;
}

}  // namespace Scene
}  // namespace Pineseed
