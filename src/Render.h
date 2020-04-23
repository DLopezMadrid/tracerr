//
// Created by dani on 22/04/2020.
//

#ifndef TRACERR_RENDER_H
#define TRACERR_RENDER_H
#include "Image.h"
#include "Sphere.h"
#include <array>
#include <vector>

class Render {
  public:
  //explicit to avoid implicit conversions
  explicit Render(int img_width = 800, int img_height = 600, xyz origin = {-1, 0, 0});
  void RenderSphere(Sphere s);
  void RenderScene(std::vector<Sphere> spheres);
  Image image_;

  void SetOrigin(xyz origin) { origin_ = origin; };
  xyz GetOrigin() { return origin_; }
  xyz GetRay(xyz direction);
  void SaveImage(std::string fname) const;

  private:
  float fov_{45};
  xyz origin_;
  rgb CastRay(xyz direction, Sphere s, PixPos pixel);
  std::vector<Sphere> spheres_;
  bool SceneIntersect(xyz direction, std::vector<Sphere> spheres, xyz &hit, xyz &normal, Material &mat);
};


#endif//TRACERR_RENDER_H
