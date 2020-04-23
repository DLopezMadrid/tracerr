//
// Created by dani on 22/04/2020.
//

#include "Render.h"
Render::Render(int img_width, int img_height, xyz origin) : image_{img_width, img_height}, origin_{origin} {
}

xyz Render::CastRay(xyz direction, Sphere s) {
  return xyz();
}

void Render::RenderScene(Sphere s) {
}
xyz Render::GetRay(xyz direction) {
  return xyz();
}
