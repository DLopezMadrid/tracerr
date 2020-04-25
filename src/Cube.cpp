//
// Created by dani on 22/04/2020.
//

#include "Cube.h"


Cube::Cube(xyz pos, float side_len, Eigen::Vector3f rot, Material material) : Shape(std::move(pos), std::move(material)), side_len_{side_len}, rot_{std::move(rot)} {
}
bool Cube::RayIntersect(xyz origin, xyz direction, float &t0) const {
  return false;
}
