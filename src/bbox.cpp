#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL {

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {

  // TODO (Part 2.2):
  // Implement ray - bounding box intersection test
  // If the ray intersected the bouding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.

  //Compute tx for yz slabs
  double tx1 = (min[0] - r.o[0]) / r.d[0];
  double tx2 = (max[0] - r.o[0]) / r.d[0];

  //Compute ty for xz slabs
  double ty1 = (min[1] - r.o[1]) / r.d[1];
  double ty2 = (max[1] - r.o[1]) / r.d[1];

  //Compute tz for xy slabs
  double tz1 = (min[2] - r.o[2]) / r.d[2];
  double tz2 = (max[2] - r.o[2]) / r.d[2];

  double tx_min = std::min(tx1, tx2);
  double tx_max = std::max(tx1, tx2);

  double ty_min = std::min(ty1, ty2);
  double ty_max = std::max(ty1, ty2);

  double tz_min = std::min(tz1, tz2);
  double tz_max = std::max(tz1, tz2);

  double t_min = std::max(std::max(tx_min, ty_min), tz_min);
  double t_max = std::min(std::min(tx_max, ty_max), tz_max);

  if (t_min <= t_max && (t_min >= t0 || t_max <= t1)) {
    if (t_min > t0) {
      t0 = t_min;
    }
    if (t_max < t1) {
      t1 = t_max;
    }
    return true;
  } else {
    return false;
  }
}

void BBox::draw(Color c, float alpha) const {

  glColor4f(c.r, c.g, c.b, alpha);

  // top
  glBegin(GL_LINE_STRIP);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
  glEnd();

  // bottom
  glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glEnd();

  // side
  glBegin(GL_LINES);
  glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
  glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CGL
