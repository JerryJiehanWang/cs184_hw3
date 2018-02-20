#ifndef CGL_DYNAMICSCENE_DIRECTIONALLIGHT_H
#define CGL_DYNAMICSCENE_DIRECTIONALLIGHT_H

#include "scene.h"
#include "../static_scene/light.h"

namespace CGL { namespace DynamicScene {

class DirectionalLight : public SceneLight {
 public:

  DirectionalLight(const Collada::LightInfo& light_info, 
                   const Matrix4x4& transform) {
    this->spectrum = light_info.spectrum;
    this->direction = -(transform * Vector4D(light_info.direction, 1)).to3D();
    this->direction.normalize();
  }

  StaticScene::SceneLight *get_static_light() const {
    StaticScene::DirectionalLight* l = 
      new StaticScene::DirectionalLight(spectrum, direction);
    return l;
  }

 private:

  Spectrum spectrum;
  Vector3D direction;

};

} // namespace DynamicScene
} // namespace CGL

#endif //CGL_DYNAMICSCENE_DIRECTIONALLIGHT_H
