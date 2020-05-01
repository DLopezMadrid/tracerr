//
// Created by dani on 25/04/2020.
//

#include "Triangle.h"

//#include <utility>

// from https://github.com/VityasZV/tinyraytracer
bool Triangle::RayIntersect(xyz const origin, xyz const direction, float &t0) const {
  const float EPSILON = 0.0000001;
  xyz vertex0 = p0_;
  xyz vertex1 = p1_;
  xyz vertex2 = p2_;
  xyz edge1, edge2, h, s, q;
  float a, f, u, v;
  edge1 = vertex1 - vertex0;
  edge2 = vertex2 - vertex0;
  h = direction.cross(edge2);
  a = edge1.dot(h);
  if (a > -EPSILON && a < EPSILON)
    return false;// This ray is parallel to this triangle.
  f = 1.0f / a;
  s = origin - vertex0;
  u = f * (s.dot(h));
  if (u < 0.0 || u > 1.0)
    return false;
  q = s.cross(edge1);
  v = f * (direction.dot(q));
  if (v < 0.0 || u + v > 1.0)
    return false;
  // At this stage we can compute t to find out where the intersection point is on the line.
  float t = f * (edge2.dot(q));
  if (t > EPSILON)// ray intersection
  {
    t0 = t;

    return true;
  } else {// This means that there is a line intersection but not a ray intersection.
    return false;
  }
}
Triangle::Triangle(xyz p0, xyz p1, xyz p2, Material material) : Shape((p0), std::move(material)), p0_{std::move(p0)}, p1_{std::move(p1)}, p2_{std::move(p2)} {
}
xyz Triangle::GetNormal(const xyz &point) const {
  xyz normal = (p1_ - p0_).cross((p2_ - p0_));
  normal.normalize();
  if (normal(2) < 0) {
    normal(2) = -1.0f * normal(2);
  }
  return normal;
}
Triangle::Triangle() : Shape({0, 0, 0}, Materials::red_rubber) {
}
shapeTypes Triangle::GetType() const {
  return shapeTypes::tri;
}
const xyz &Triangle::GetP0_() const {
  return p0_;
}

const xyz &Triangle::GetP1_() const {
  return p1_;
}

const xyz &Triangle::GetP2_() const {
  return p2_;
}
