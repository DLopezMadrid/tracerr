//
// Created by dani on 26/04/2020.
//

#ifndef TRACERR_RECTANGLE_H
#define TRACERR_RECTANGLE_H
#include "Shape.h"
#include "Triangle.h"

// Example of a more complex shape made out of simpler ones (2 triangles)
class Rectangle : public Shape {
  public:
  Rectangle(xyz p0, xyz p1, xyz p2, Material material);
  Rectangle(xyz p0, xyz p1, xyz p2, xyz p3, Material material);
  bool RayIntersect(xyz const origin, xyz const direction, float &t0) const override;
  xyz GetNormal(const xyz &point) const override;
  shapeTypes GetType() const override;
  const xyz &GetP0_() const;
  const xyz &GetP1_() const;
  const xyz &GetP2_() const;
  const xyz &GetP3_() const;

  private:
  Triangle t1_;
  Triangle t2_;
  xyz p0_;
  xyz p1_;
  xyz p2_;
  xyz p3_;
};


#endif//TRACERR_RECTANGLE_H
