//
// Created by dani on 22/04/2020.
//

#ifndef TRACERR_RENDER_H
#define TRACERR_RENDER_H
#include "Image.h"
#include "Sphere.h"
#include <array>

class Render {
  Render(int size_x, int size_y);
  std::array<float, 3> CastRay(std::array<float, 3> origin, std::array<float, 3> direction, Sphere s);
  void RenderScene(Sphere s);

  private:
  Image image_;
};


#endif//TRACERR_RENDER_H
