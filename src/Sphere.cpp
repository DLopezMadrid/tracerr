//
// Created by dani on 21/04/2020.
//

#include "Sphere.h"

Sphere::Sphere(xyz pos, double radius, rgb color) : Shape(pos, color), radius_{radius} {
}
bool Sphere::RayIntersect(xyz origin, xyz direction, float t0) const {
  return false;
}
