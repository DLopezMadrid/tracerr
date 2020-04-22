//
// Created by dani on 21/04/2020.
//

#include "Sphere.h"
bool Sphere::rayIntersect(int pix_x, int pix_y) {
  return false;
}
Sphere::Sphere(double &&x, double &&y, double &&z, double &&radius) : radius_{radius} {
  pos_ = {x, y, z};
  rgb_color_ = {100, 100, 100};
}
Sphere::Sphere() {
  pos_ = {10, 10, 10};
  radius_ = 1;
  rgb_color_ = {50, 50, 50};
}
