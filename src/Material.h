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
typedef Eigen::Vector3f eigen_rgb;


class Material {
  public:
  Material(const rgb &color, albedo alb, float spec, float refract) : color_{color}, albedo_{alb}, specular_comp_{spec}, refractive_index{refract} {}
  Material() : color_() {}
  rgb color_;
  albedo albedo_;
  float specular_comp_;
  float refractive_index;
  //  uint8_t ambient_{25};


  rgb DiffuseColor(float diffuse_light_intensity) {
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[0]) * diffuse_light_intensity)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[1]) * diffuse_light_intensity)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, (static_cast<float>(color_[2]) * diffuse_light_intensity)));

    return rgb({r, g, b});
  }

  rgb DiffuseSpecularColor(float diffuse_light_intensity, float specular_light_intensity) {
    auto diffuse_color = DiffuseColor(diffuse_light_intensity);
    eigen_rgb new_color{diffuse_color[0], diffuse_color[1], diffuse_color[2]};

    new_color = new_color * albedo_(0) + Eigen::Vector3f{255.0, 255.0, 255.0} * specular_light_intensity * albedo_(1);

    uint8_t r = static_cast<uint8_t>(std::min(255.0f, new_color(0)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, new_color(1)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, new_color(2)));

    return rgb({r, g, b});
  }

  rgb DiffuseSpecularReflectionColor(float diffuse_light_intensity, float specular_light_intensity, rgb reflect_color) {
    rgb diffuse_specular_color = DiffuseSpecularColor(diffuse_light_intensity, specular_light_intensity);

    //    uint8_t m_max {0xFF};
    //    diffuse_specular_color[0] = std::min(m_max, static_cast<uint8_t>(diffuse_specular_color[0] + static_cast<uint8_t>(reflect_color[0]*albedo_(2))));
    //    diffuse_specular_color[1] = std::min(m_max, static_cast<uint8_t>(diffuse_specular_color[1] + static_cast<uint8_t>(reflect_color[1]*albedo_(2))));
    //    diffuse_specular_color[2] = std::min(m_max, static_cast<uint8_t>(diffuse_specular_color[2] + static_cast<uint8_t>(reflect_color[2]*albedo_(2))));

    uint8_t r = static_cast<uint8_t>(std::min(255.0f, (float) diffuse_specular_color[0] + (float) reflect_color[0] * albedo_(2)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, (float) diffuse_specular_color[1] + (float) reflect_color[1] * albedo_(2)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, (float) diffuse_specular_color[2] + (float) reflect_color[2] * albedo_(2)));

    return rgb({r, g, b});
    //    return diffuse_specular_color;
  }

  rgb DiffuseSpecularReflectionRefractionColor(float diffuse_light_intensity, float specular_light_intensity, rgb reflect_color, rgb refract_color) {
    rgb diffuse_specular_reflection_color = DiffuseSpecularReflectionColor(diffuse_light_intensity, specular_light_intensity, reflect_color);
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, (float) diffuse_specular_reflection_color[0] + (float) refract_color[0] * albedo_(3)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, (float) diffuse_specular_reflection_color[1] + (float) refract_color[1] * albedo_(3)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, (float) diffuse_specular_reflection_color[2] + (float) refract_color[2] * albedo_(3)));
    return rgb({r, g, b});
  }

  rgb DSRRColor(float diffuse_light_intensity, float specular_light_intensity, rgb reflect_color, rgb refract_color) {
    float rf_col = static_cast<float>(color_[0]);
    float gf_col = static_cast<float>(color_[1]);
    float bf_col = static_cast<float>(color_[2]);
    Eigen::Vector3f rgb_f{rf_col, gf_col, bf_col};

    float rf_refract = static_cast<float>(refract_color[0]);
    float gf_refract = static_cast<float>(refract_color[1]);
    float bf_refract = static_cast<float>(refract_color[2]);
    Eigen::Vector3f rgb_f_refract{rf_refract, gf_refract, bf_refract};

    float rf_reflect = static_cast<float>(reflect_color[0]);
    float gf_reflect = static_cast<float>(reflect_color[1]);
    float bf_reflect = static_cast<float>(reflect_color[2]);
    Eigen::Vector3f rgb_f_reflect{rf_reflect, gf_reflect, bf_reflect};

    Eigen::Vector3f ret_f{0.0, 0.0, 0.0};
    Eigen::Vector3f unit_f{255.0, 255.0, 255.0};
    //    Eigen::Vector3f unit_f{1.0, 1.0, 1.0};

    //    ret_f = rgb_f *diffuse_light_intensity * albedo_(0) + unit_f * specular_light_intensity * albedo_(1) + rgb_f_reflect * albedo_(2) + rgb_f_refract * albedo_(3);
    ret_f = rgb_f * diffuse_light_intensity * albedo_(0) + unit_f * specular_light_intensity * albedo_(1) + rgb_f_reflect * albedo_(2) + rgb_f_refract * albedo_(3);

    uint8_t r = static_cast<uint8_t>(std::min(255.0f, ret_f(0)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, ret_f(1)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, ret_f(2)));

    return rgb{r, g, b};
  }

  rgb DSRRColor2(float diffuse_light_intensity, float specular_light_intensity, rgb reflect_color, rgb refract_color) {

    Eigen::Vector3f rgb_f = rgb2vec(color_);
    Eigen::Vector3f rgb_f_refract = rgb2vec(refract_color);
    Eigen::Vector3f rgb_f_reflect = rgb2vec(reflect_color);

    Eigen::Vector3f ret_f{0.0, 0.0, 0.0};
    Eigen::Vector3f unit_f{1.0, 1.0, 1.0};

    ret_f = rgb_f * diffuse_light_intensity * albedo_(0) + unit_f * specular_light_intensity * albedo_(1) + rgb_f_reflect * albedo_(2) + rgb_f_refract * albedo_(3);

    rgb rgbret = vec2rgb(ret_f);
    return rgbret;
  }

  static rgb vec2rgb(Eigen::Vector3f v) {
    uint8_t r = static_cast<uint8_t>(std::min(255.0f, 255.0f * v(0)));
    uint8_t g = static_cast<uint8_t>(std::min(255.0f, 255.0f * v(1)));
    uint8_t b = static_cast<uint8_t>(std::min(255.0f, 255.0f * v(2)));

    return rgb({r, g, b});
  }

  static Eigen::Vector3f rgb2vec(rgb m_rgb) {
    float rf = std::min(1.0f, static_cast<float>(m_rgb[0]) / 255.0f);
    float gf = std::min(1.0f, static_cast<float>(m_rgb[1]) / 255.0f);
    float bf = std::min(1.0f, static_cast<float>(m_rgb[2]) / 255.0f);
    Eigen::Vector3f rgb_f{rf, gf, bf};
    return rgb_f;
  }
};


namespace Materials {
  static Material ivory({100, 100, 75}, {0.6, 0.30, 0.15, 0.0}, 50.0, 1.0);
  static Material red_rubber({150, 15, 15}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material green_rubber({15, 150, 15}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material blue_rubber({15, 15, 150}, {0.9, 0.10, 0.00, 0.0}, 10.0, 1.0);
  static Material black_plastic({0, 0, 0}, {0.0, 0.60, 0.15, 0.0}, 300.0, 1.0);
  static Material mirror({255, 255, 255}, {0.0, 10.0, 0.80, 0.0}, 1500.0, 1.0);
  static Material glass({150, 180, 205}, {0.0, 0.50, 0.10, 0.8}, 125.0, 1.5);
}// namespace Materials


#endif//TRACERR_MATERIAL_H
