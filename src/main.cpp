#include "Image.h"
#include "Render.h"
#include "Sphere.h"
#include <eigen3/Eigen/Core>
#include <iostream>
#include <memory>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "../include/stb_image_write.h"


int main() {

  Render render(800, 600, {0, 0, 0});
  Sphere sphere({-3, 0, -16}, 2);
  render.image_.DrawGradientBackground();
  render.RenderScene(sphere);

  render.SaveImage("RenderTest.png");

  return 0;
}
