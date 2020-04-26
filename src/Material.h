//
// Created by dani on 23/04/2020.
//

#ifndef TRACERR_MATERIAL_H
#define TRACERR_MATERIAL_H

#include <array>
#include <cmath>
#include <eigen3/Eigen/Core>

typedef std::array<uint8_t, 3> rgb;
typedef Eigen::Vector4f albedo;
typedef Eigen::Vector3f rgb_f;


class Material {
  public:
  Material(const rgb_f &color, albedo alb, float spec, float refract) : color_f_{color / 255.0}, albedo_{alb}, specular_comp_{spec}, refractive_index{refract} {}
  Material() : color_f_() {}
  //  rgb color_;
  rgb_f color_f_;
  albedo albedo_;
  float specular_comp_;
  float refractive_index;
  static float ambient_light;

  Eigen::Vector3f unit_f_1{1.0, 1.0, 1.0};

  rgb_f DSRRColor3(float const &diffuse_light_intensity, float const &specular_light_intensity, rgb_f const &reflect_color, rgb_f const &refract_color) {
    return ambient_light * color_f_ + color_f_ * diffuse_light_intensity * albedo_(0) + unit_f_1 * specular_light_intensity * albedo_(1) + reflect_color * albedo_(2) + refract_color * albedo_(3);
  }

  static rgb vec2rgb(Eigen::Vector3f const &v) {
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, 255.0f * v(0)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, 255.0f * v(1)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, 255.0f * v(2)));

    return rgb({r, g, b});
  }

  static Eigen::Vector3f rgb2vec(rgb const &m_rgb) {
    float rf = std::min(1.0f, static_cast<float>(m_rgb[0]) / 255.0f);
    float gf = std::min(1.0f, static_cast<float>(m_rgb[1]) / 255.0f);
    float bf = std::min(1.0f, static_cast<float>(m_rgb[2]) / 255.0f);

    return Eigen::Vector3f{rf, gf, bf};
    ;
  }
};


namespace Materials {
  static Material ivory({100, 100, 75}, {0.6, 0.30, 0.15, 0.0}, 50.0, 1.0);
  static Material red_plastic({150, 15, 15}, {0.6, 0.60, 0.15, 0.0}, 250.0, 1.0);
  static Material red_rubber({150, 15, 15}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material green_rubber({15, 150, 15}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material blue_rubber({15, 15, 150}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material orange_rubber({220, 165, 0}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material pink_rubber({200, 50, 100}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material black_plastic({0, 0, 0}, {0.0, 0.60, 0.15, 0.0}, 300.0, 1.0);
  static Material mirror({255, 255, 255}, {0.0, 10.0, 0.80, 0.0}, 1500.0, 1.0);
  static Material glass({150, 180, 205}, {0.0, 0.50, 0.10, 0.8}, 125.0, 1.5);
  static Material chessboard_bright({120, 80, 80}, {1.9, 0.1, 0.0, 0.0}, 10.0, 1.0);
  static Material chessboard_dark({80, 50, 25}, {0.9, 0.1, 0.0, 0.0}, 10.0, 1.0);
}// namespace Materials


#endif//TRACERR_MATERIAL_H
