//
// Created by dani on 26/04/2020.
//

#include "Rectangle.h"

Rectangle::Rectangle(xyz p0, xyz p1, xyz p2, Material material) : Shape(p0, material), t1_(), t2_(), p0_{std::move(p0)}, p1_{std::move(p1)}, p2_{std::move(p2)} {
  p3_ = p0_ + (p2_ - p1_);
  t1_ = Triangle(p0_, p1_, p2_, material);
  t2_ = Triangle(p0_, p3_, p2_, material);
}

xyz Rectangle::GetNormal(const xyz &point) const {
  xyz normal = (p1_ - p0_).cross((p2_ - p0_));
  normal.normalize();
  return normal;
}

bool Rectangle::RayIntersect(xyz origin, xyz direction, float &t0) const {
  float auxt0_1{0};
  float auxt0_2{0};

  bool t1_intersect = t1_.RayIntersect(origin, direction, auxt0_1);
  bool t2_intersect = t2_.RayIntersect(origin, direction, auxt0_2);

  t0 = auxt0_1 >= auxt0_2 ? auxt0_1 : auxt0_2;

  return (t1_intersect || t2_intersect);
}