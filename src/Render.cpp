//
// Created by dani on 22/04/2020.
//

#include "Render.h"
void Render::RenderScene(Sphere s) {
}
std::array<float, 3> Render::CastRay(std::array<float, 3> origin, std::array<float, 3> direction, Sphere s) {
  return std::array<float, 3>();
}
Render::Render(int size_x, int size_y) : image_(size_x, size_y) {
}
