//
// Created by dani on 20/04/2020.
//
#include "Shape.h"

std::ostream &operator<<(std::ostream &o, const Shape &s) {
  o << " Shape ";
  return o;
}
Shape::Shape(xyz pos, Material material) : pos_{pos}, color_{material.diffuse_color}, material_{material} {
}
