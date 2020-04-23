//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_SPHERE_H
#define TRACERR_SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
  public:
  Sphere(xyz pos = {0, 0, 0}, double radius = {1}, rgb color = {100, 100, 100});
  bool RayIntersect(xyz origin, xyz direction, float t0) const;
  double GetRadius() const { return radius_; }

  private:
  double radius_;
};


#endif//TRACERR_SPHERE_H
