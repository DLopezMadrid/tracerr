//
// Created by dani on 23/04/2020.
//

#ifndef TRACERR_MATERIAL_H
#define TRACERR_MATERIAL_H

#include <array>
#include <cmath>
#include <eigen3/Eigen/Core>

typedef std::array<uint8_t, 3> rgb;
typedef Eigen::Vector2f albedo;
typedef Eigen::Vector3f eigen_rgb;

class Material {
  public:
  Material(const rgb &color, albedo alb, float spec) : color_{color}, albedo_{alb}, specular_comp_{spec} {}
  Material() : color_() {}
  rgb color_;
  albedo albedo_;
  float specular_comp_;
  //  uint8_t ambient_{25};


  rgb DiffuseColor(float diffuse_light_intensity) {
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[0]) * diffuse_light_intensity)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[1]) * diffuse_light_intensity)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[2]) * diffuse_light_intensity)));
    return rgb({r, g, b});
  }

  rgb diffuse_specular_color(float diffuse_light_intensity, float specular_light_intensity) {
    auto diffuse_color = DiffuseColor(diffuse_light_intensity);
    eigen_rgb new_color{diffuse_color[0], diffuse_color[1], diffuse_color[2]};

    new_color = new_color * albedo_(0) + Eigen::Vector3f{255.0, 255.0, 255.0} * specular_light_intensity * albedo_(1);

    uint8_t r = static_cast<uint8_t>(std::min(255.0f, new_color(0)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, new_color(1)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, new_color(2)));

    return rgb({r, g, b});
  }
};

namespace Materials {
  static Material ivory({100, 100, 75}, {0.6, 0.3}, 50.0);
  static Material red_rubber({150, 15, 15}, {0.9, 0.1}, 10.0);
  static Material green_rubber({15, 150, 15}, {0.9, 0.1}, 10.0);
  static Material blue_rubber({15, 15, 150}, {0.9, 0.1}, 10.0);
  static Material black_plastic({5, 5, 5}, {0.6, 0.3}, 65.0);
}// namespace Materials


#endif//TRACERR_MATERIAL_H
