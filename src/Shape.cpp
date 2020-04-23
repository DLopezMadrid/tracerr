//
// Created by dani on 20/04/2020.
//
#include "Shape.h"

std::ostream &operator<<(std::ostream &o, const Shape &s) {
  o << " Shape ";
  return o;
}
Shape::Shape(xyz pos, rgb color) : pos_{pos}, color_{color} {
}
