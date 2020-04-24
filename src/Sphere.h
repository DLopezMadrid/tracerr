//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_SPHERE_H
#define TRACERR_SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
  public:
      Sphere(xyz pos = {0, 0, 0}, float radius = {1}, Material material = Materials::red_rubber);
      bool RayIntersect(xyz const origin, xyz const direction, float &t0) const;
      double GetRadius() const { return radius_; }

  private:
  float radius_;
};


#endif//TRACERR_SPHERE_H
