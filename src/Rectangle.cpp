//
// Created by dani on 26/04/2020.
//

#include "Rectangle.h"

Rectangle::Rectangle(xyz p0, xyz p1, xyz p2, Material material) : Shape(p0, material), t1_(), t2_(), p0_{std::move(p0)}, p1_{std::move(p1)}, p2_{std::move(p2)} {
  p3_ = p0_ + (p2_ - p1_);
  t1_ = Triangle(p0_, p1_, p2_, material);
  t2_ = Triangle(p0_, p3_, p2_, material);
}

Rectangle::Rectangle(xyz p0, xyz p1, xyz p2, xyz p3, Material material) : Shape(p0, material), t1_(), t2_(), p0_{std::move(p0)}, p1_{std::move(p1)}, p2_{std::move(p2)}, p3_{std::move(p3)} {
  t1_ = Triangle(p0_, p1_, p2_, material);
  t2_ = Triangle(p0_, p3_, p2_, material);
}


xyz Rectangle::GetNormal(const xyz &point) const {
  xyz normal = (p1_ - p0_).cross((p2_ - p0_));
  normal.normalize();
  if (normal(2) < 0) {
    normal(2) = -1.0f * normal(2);
  }
  //  std::cout << normal  << '\n';
  return normal;
}

bool Rectangle::RayIntersect(xyz origin, xyz direction, float &t0) const {
  float auxt0_1{0};
  float auxt0_2{0};

  bool t1_intersect{false};
  bool t2_intersect{false};

  t1_intersect = t1_.RayIntersect(origin, direction, auxt0_1);
  t0 = auxt0_1;
  if (!t1_intersect) {
    t2_intersect = t2_.RayIntersect(origin, direction, auxt0_2);
    t0 = auxt0_1 >= auxt0_2 ? auxt0_1 : auxt0_2;
  }

  return (t1_intersect || t2_intersect);
}