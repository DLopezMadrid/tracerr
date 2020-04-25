//
// Created by dani on 22/04/2020.
//

#ifndef TRACERR_CUBE_H
#define TRACERR_CUBE_H
#include "Shape.h"

class Cube : public Shape {
  public:
  Cube(xyz pos = {0, 0, 0}, float side_len = {1}, Eigen::Vector3f rot = {0, 0, 0}, Material material = Materials::red_rubber);
  bool RayIntersect(xyz const origin, xyz const direction, float &t0) const override;
  double GetSideLen() const { return side_len_; }
  Eigen::Vector3f GetCubeRot() const { return rot_; }

  private:
  float side_len_;
  Eigen::Vector3f rot_;
};


#endif//TRACERR_CUBE_H
