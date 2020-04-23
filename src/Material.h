//
// Created by dani on 23/04/2020.
//

#ifndef TRACERR_MATERIAL_H
#define TRACERR_MATERIAL_H

#include <array>


typedef std::array<uint8_t, 3> rgb;


class Material {
  public:
  Material(const rgb &color) : diffuse_color(color) {}
  Material() : diffuse_color() {}
  rgb diffuse_color;
};

namespace Materials {
  static Material ivory({100, 100, 75});
  static Material red_rubber({150, 15, 15});
  static Material green_rubber({15, 150, 15});
  static Material blue_rubber({15, 15, 150});
}// namespace Materials


#endif//TRACERR_MATERIAL_H
