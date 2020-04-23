//
// Created by dani on 21/04/2020.
//

#include "Sphere.h"

Sphere::Sphere(double &&x, double &&y, double &&z, double &&radius) : radius_{radius} {
  pos_ = {x, y, z};
  rgb_color_ = {100, 100, 100};
}
Sphere::Sphere() {
  pos_ = {10, 10, 10};
  radius_ = 1;
  rgb_color_ = {50, 50, 50};
}
bool Sphere::rayIntersect(std::array<float, 3> origin, std::array<float, 3> direction, float t0) const {
  return false;
}
