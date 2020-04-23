//
// Created by dani on 22/04/2020.
//

#include "Render.h"
Render::Render(int img_width, int img_height, xyz origin) : image_{img_width, img_height}, origin_{origin} {
}

bool Render::SceneIntersect(xyz direction, std::vector<Sphere> spheres, xyz &hit, xyz &normal, Material &mat) {
}

rgb Render::CastRay(xyz direction, Sphere s, PixPos pixel) {
  float sphere_dist = std::numeric_limits<float>::max();
  if (!s.RayIntersect(origin_, direction, sphere_dist)) {
    return image_.GetPixelColor(pixel);
  } else {
    return s.GetColor();
  }
}

void Render::RenderSphere(Sphere s) {
  int width{image_.GetImageWidth()};
  int height{image_.GetImageHeight()};
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      float x = (2 * (col + 0.5) / (float) width - 1) * tan(fov_ / 2.) * width / (float) height;
      float y = -(2 * (row + 0.5) / (float) height - 1) * tan(fov_ / 2.);
      xyz dir{x, y, -1};
      dir.normalize();
      PixPos pixel{col, row};
      image_.SetPixelColor(pixel, CastRay(dir, s, pixel));
    }
  }
}
xyz Render::GetRay(xyz direction) {
  return xyz();
}
void Render::SaveImage(std::string fname) const {
  image_.SaveImage(fname);
}
void Render::RenderScene(std::vector<Sphere> spheres) {
  int width{image_.GetImageWidth()};
  int height{image_.GetImageHeight()};
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      float x = (2 * (col + 0.5) / (float) width - 1) * tan(fov_ / 2.) * width / (float) height;
      float y = -(2 * (row + 0.5) / (float) height - 1) * tan(fov_ / 2.);
      xyz dir{x, y, -1};
      dir.normalize();
      PixPos pixel{col, row};
      //      image_.SetPixelColor(pixel, CastRay(dir, spheres, pixel));
    }
  }
}
