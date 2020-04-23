//
// Created by dani on 23/04/2020.
//

#ifndef TRACERR_MATERIAL_H
#define TRACERR_MATERIAL_H

#include <array>
#include <cmath>
//#include <eigen3/Eigen/Core>

typedef std::array<uint8_t, 3> rgb;

class Material {
  public:
  Material(const rgb &color) : color_(color) {}
  Material() : color_() {}
  rgb color_;


  rgb diffuse_color(float diffuse_light_intensity) {
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[0]) * diffuse_light_intensity)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[1]) * diffuse_light_intensity)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[2]) * diffuse_light_intensity)));
    return rgb({r, g, b});
  }
};

namespace Materials {
  static Material ivory({100, 100, 75});
  static Material red_rubber({150, 15, 15});
  static Material green_rubber({15, 150, 15});
  static Material blue_rubber({15, 15, 150});
}// namespace Materials


#endif//TRACERR_MATERIAL_H
