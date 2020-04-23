//
// Created by dani on 20/04/2020.
//

#ifndef TRACERR_SHAPE_H
#define TRACERR_SHAPE_H
#include <array>
#include <iostream>
#include <tuple>


class Shape {
  public:
      virtual bool rayIntersect(std::array<float, 3> origin, std::array<float, 3> direction, float t0) const = 0;

      friend std::ostream &operator<<(std::ostream &o, Shape const &s);
      virtual ~Shape() = default;
      Shape() = default;
  Shape(Shape const &) = default;
  Shape(Shape &&) = default;
  Shape &operator=(Shape const &) = default;
  Shape &operator=(Shape &&) = default;

  protected:
  std::array<double, 3> pos_;
  float mat_reflectivity_;
  std::array<int, 3> rgb_color_;
};


#endif//TRACERR_SHAPE_H
