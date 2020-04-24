//
// Created by dani on 24/04/2020.
//

#ifndef TRACERR_RENDER_H
#define TRACERR_RENDER_H

#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "Sphere.h"
#include <eigen3/Eigen/Core>

typedef Eigen::Vector3f Vec3f;
typedef Eigen::Vector4f Vec4f;

typedef std::array<uint8_t, 3> rgb;


class Render {
  public:
  explicit Render(int img_width = 800, int img_height = 600, xyz origin = {0, 0, 0}, std::vector<Light> lights = {});
  void SaveImage(std::string fname) const;
  void RenderScene(std::vector<Sphere> &&spheres);
  void SetOrigin(xyz origin) { original_origin_ = origin; };
  xyz GetOrigin() { return original_origin_; }

  private:
  Vec3f reflect(const Vec3f &I, const Vec3f &N);
  Vec3f refract(const Vec3f &I, const Vec3f &N, const float eta_t, const float eta_i = 1.f);
  bool scene_intersect(Vec3f origin, Vec3f direction, Vec3f &hit, Vec3f &normal, Material &mat);
  Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, size_t depth = 0, int ray_type = 0);
  Image image_;
  float fov_{M_PI / 3.0};
  Vec3f original_origin_;
  std::vector<Sphere> spheres_;
  Vec3f ambient_light_;
  std::vector<Light> lights_;
};


#endif//TRACERR_RENDER_H
