#include <boost/make_shared.hpp>

#include "kriti/scene/Camera.h"
#include "kriti/math/ViewGenerator.h"
#include "kriti/math/Vector.h"

#include "Camera.h"

namespace Pineseed {
namespace Scene {

Camera::Camera() {
    m_camera = boost::make_shared<Kriti::Scene::Camera>();
    // TODO: check R value to make sure aspect ratio is correct!
    m_camera->setProjection(
        Kriti::Math::ViewGenerator().orthogonal(
            -0.5, 1.00, -0.5, 0.5,
            -10.0, 10.0));
}

void Camera::updateCamera() {
    m_camera->setTarget(m_tracking->position(), Kriti::Math::Quaternion());
    //m_camera->position() = m_tracking->position();
    m_camera->step(0.0f);
}

}  // namespace Scene
}  // namespace Pineseed
