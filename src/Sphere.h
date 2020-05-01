//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_SPHERE_H
#define TRACERR_SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
  public:
  Sphere(xyz pos = {0, 0, 0}, float radius = {1}, Material material = Materials::red_rubber);
  bool RayIntersect(xyz const origin, xyz const direction, float &t0) const override;
  double GetRadius() const { return radius_; }
  xyz GetNormal(const xyz &point) const override;
  shapeTypes GetType() const override;


  private:
  float radius_;
};


#endif//TRACERR_SPHERE_H
