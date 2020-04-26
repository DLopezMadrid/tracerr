//
// Created by dani on 20/04/2020.
//

#ifndef TRACERR_SHAPE_H
#define TRACERR_SHAPE_H
#include "Material.h"
#include <array>
#include <eigen3/Eigen/Core>
#include <iostream>
#include <tuple>

//typedef std::array<float, 3> xyz;
typedef Eigen::Vector3f xyz;
typedef Eigen::Vector3f Vec3f;
typedef Eigen::Vector4f Vec4f;

typedef std::array<uint8_t, 3> rgb;

// Pure virtual class to define the interface for all other shapes
// For dynamic polymorphism
class Shape {
  public:
  Shape(xyz pos = {0, 0, 0}, Material material = Materials::red_rubber) : pos_{pos}, material_{material} {};

  virtual bool RayIntersect(xyz const origin, xyz const direction, float &t0) const = 0;
  virtual xyz GetNormal(const xyz &point) const = 0;

  xyz GetPos() const { return pos_; }
  Material GetMaterial() const { return material_; }

  // Since it is a pure virtual function, the destructor will also need to be virtual
  virtual ~Shape() = default;
  // Due to us declaring the destructor, the compiler will disable the creation of the default move and copy ctors and assignment operators
  // We define them manually as default
  Shape() = default;
  Shape(Shape const &) = default;
  Shape(Shape &&) = default;
  Shape &operator=(Shape const &) = default;
  Shape &operator=(Shape &&) = default;

  protected:
  xyz pos_;
  Material material_;
};


#endif//TRACERR_SHAPE_H
