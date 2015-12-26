#ifndef PINESEED_SCENE__CAMERA_H
#define PINESEED_SCENE__CAMERA_H

#include <boost/shared_ptr.hpp>

#include "../level/Pos.h"

#include "kriti/scene/Camera.fwd"
#include "kriti/render/UniformHook.h"

namespace Pineseed {
namespace Scene {

class Camera {
private:
    boost::shared_ptr<Kriti::Scene::Camera> m_camera;
public:
    Camera();

    void updateCamera(const Level::Pos &pos);

    boost::shared_ptr<Kriti::Render::UniformHook> hook()
        { return boost::static_pointer_cast<Kriti::Render::UniformHook>(
            m_camera); }
};

}  // namespace Scene
}  // namespace Pineseed

#endif
