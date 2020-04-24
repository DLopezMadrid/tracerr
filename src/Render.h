//
// Created by dani on 22/04/2020.
//

#ifndef TRACERR_RENDER_H
#define TRACERR_RENDER_H
#include "Image.h"
#include "Light.h"
#include "Sphere.h"
#include <array>
#include <vector>

class Render {
  public:
      //explicit to avoid implicit conversions
      explicit Render(int img_width = 800, int img_height = 600, xyz origin = {0, 0, 0}, std::vector<Light> = {});
      void RenderSphere(Sphere s);
      void RenderScene(std::vector<Sphere> spheres);
  Image image_;

  void SetOrigin(xyz origin) { origin_ = origin; };
  xyz GetOrigin() { return origin_; }
  xyz GetRay(xyz direction);
  void SaveImage(std::string fname) const;

  private:
      float fov_{3.14159 / 3.0};
      xyz origin_;
      rgb CastRay(xyz direction, Sphere sphere, PixPos pixel);
      rgb CastRay(xyz origin, xyz direction, std::vector<Sphere> spheres, PixPos pixel, int recursive_depth);
      std::vector<Sphere> spheres_;
  bool SceneIntersect(xyz origin, xyz direction, std::vector<Sphere> spheres, xyz &hit, xyz &normal, Material &mat);
  //  Light light_;
  std::vector<Light> lights_;
  xyz Reflect(const xyz &I, const xyz &N);
  xyz Refract(const xyz &I, const xyz &N, const float eta_t, const float eta_i);
};


#endif//TRACERR_RENDER_H
