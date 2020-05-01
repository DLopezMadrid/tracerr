//
// Created by dani on 25/04/2020.
//

#ifndef TRACERR_TRIANGLE_H
#define TRACERR_TRIANGLE_H

#include "Shape.h"
#include <eigen3/Eigen/Geometry>
#include <vector>

// The most basic 2d shape
// Can be used to render more complex figures (like .obj models)
class Triangle : public Shape {

  public:
  Triangle(xyz p0, xyz p1, xyz p2, Material material);
  Triangle();
  bool RayIntersect(xyz const origin, xyz const direction, float &t0) const override;
  xyz GetNormal(const xyz &point) const override;
  shapeTypes GetType() const override;
  const xyz &GetP0_() const;
  const xyz &GetP1_() const;
  const xyz &GetP2_() const;

  private:
  xyz p0_;
  xyz p1_;
  xyz p2_;
};


#endif//TRACERR_TRIANGLE_H
