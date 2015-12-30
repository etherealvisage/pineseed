#ifndef PINESEED_SCENE__CAMERA_H
#define PINESEED_SCENE__CAMERA_H

#include <boost/shared_ptr.hpp>

#include "../level/Pos.h"

#include "kriti/scene/Camera.h"
#include "kriti/render/UniformHook.h"

namespace Pineseed {
namespace Scene {

class Camera : public Kriti::Scene::Camera {
public:
    Camera();
};

}  // namespace Scene
}  // namespace Pineseed

#endif
