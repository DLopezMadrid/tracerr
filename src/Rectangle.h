//
// Created by dani on 26/04/2020.
//

#ifndef TRACERR_RECTANGLE_H
#define TRACERR_RECTANGLE_H
#include "Shape.h"
#include "Triangle.h"
class Rectangle : public Shape {
  public:
  Rectangle(xyz p0, xyz p1, xyz p2, Material material);
  bool RayIntersect(xyz const origin, xyz const direction, float &t0) const override;
  xyz GetNormal(const xyz &point) const override;

  private:
  Triangle t1_;
  Triangle t2_;
  xyz p0_;
  xyz p1_;
  xyz p2_;
  xyz p3_;
};


#endif//TRACERR_RECTANGLE_H
