//
// Created by dani on 25/04/2020.
//

#ifndef TRACERR_TRIANGLE_H
#define TRACERR_TRIANGLE_H

#include "Shape.h"
#include <eigen3/Eigen/Geometry>
#include <vector>


class Triangle : public Shape {

  public:
  Triangle(xyz p0, xyz p1, xyz p2, Material material);
  bool RayIntersect(xyz const origin, xyz const direction, float &t0) const override;
  xyz GetNormal(xyz point) const override;

  private:
  xyz p0_;
  xyz p1_;
  xyz p2_;
};


#endif//TRACERR_TRIANGLE_H
