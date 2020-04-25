//
// Created by dani on 21/04/2020.
//

#include "Sphere.h"

Sphere::Sphere(xyz pos, float radius, Material material) : Shape(pos, material), radius_{radius} {
}
bool Sphere::RayIntersect(xyz const origin, xyz const direction, float &t0) const {
  //  https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
  //
  xyz dist_origin_center = pos_ - origin;
  float tca = dist_origin_center.dot(direction);
  float d2 = dist_origin_center.dot(dist_origin_center) - tca * tca;
  if (d2 > radius_ * radius_) return false;
  float thc = sqrtf(radius_ * radius_ - d2);
  t0 = tca - thc;
  float t1 = tca + thc;
  if (t0 < 0) {
    t0 = t1;
  }


  return t0 >= 0;
}
xyz Sphere::GetNormal(xyz point) const {
  xyz normal = (point - pos_);
  normal.normalize();
  return normal;
}
