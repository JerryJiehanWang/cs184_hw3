#include "environment_light.h"

namespace CGL { namespace StaticScene {

EnvironmentLight::EnvironmentLight(const HDRImageBuffer* envMap)
    : envMap(envMap) {
  // TODO: initialize things here as needed
}

Spectrum EnvironmentLight::sample_L(const Vector3D& p, Vector3D* wi,
                                    float* distToLight,
                                    float* pdf) const {
  // TODO: Implement
  return Spectrum(0, 0, 0);
}

Spectrum EnvironmentLight::sample_dir(const Ray& r) const {
  // TODO: Implement
  return Spectrum(0, 0, 0);
}

bool EnvironmentLight::point_on_light(const Vector3D& p, Spectrum* rad) const {
  // TODO: Implement
  
  return false;
}

} // namespace StaticScene
} // namespace CGL
