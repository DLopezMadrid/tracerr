//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_SPHERE_H
#define TRACERR_SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
  public:
  Sphere();
  Sphere(double &&x, double &&y, double &&z, double &&radius);
  bool rayIntersect(std::array<float, 3> origin, std::array<float, 3> direction, float t0) const override;

  private:
  double radius_;
};


#endif//TRACERR_SPHERE_H
