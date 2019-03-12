#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.
  Vector3D o = r.o;
  Vector3D d = r.d;
  Vector3D center = this -> o;
  double radius = this -> r;

  double a = dot(d, d);
  double b = dot(2 * (o - center), d);
  double c = dot((o - center), (o - center)) - pow(radius, 2);

  if (pow(b, 2) - 4 * a * c < 0) { //Test if there are intersections
    return false;
  }

  double result1 = (-b + sqrt(pow(b, 2) - 4 * a * c))/(2 * a);
  double result2 = (-b - sqrt(pow(b, 2) - 4 * a * c))/(2 * a);

  if (result1 < 0 || result2 < 0) {//Test the ray direction is actually the same as the intersection
    return false;
  } else {
    t1 = result1 < result2 ? result1 : result2;
    t2 = t1 == result1 ? result2 : result1;

    if (t1 >= r.min_t && t1 <= r.max_t) {
      return true;
    } else {
      return false;
    }
  }
}

bool Sphere::intersect(const Ray& r) const {
  // TODO (Part 1.4):
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.
  double t1, t2;
  return test(r, t1, t2);
}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
  double t1, t2;
  bool isIntersected = test(r, t1, t2);

  if (isIntersected) {
    r.max_t = t1;
    i -> primitive = this;
    i -> bsdf = get_bsdf();
    i -> t = t1;
    i -> n = normal(r.o + t1 * r.d); // Normalize the vector from center of the sphere to the intersection point.
  }
  
  return isIntersected;
}

void Sphere::draw(const Color& c, float alpha) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c, float alpha) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CGL
