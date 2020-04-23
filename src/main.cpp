#include "Image.h"
#include "Material.h"
#include "Render.h"
#include "Sphere.h"
#include <eigen3/Eigen/Core>
#include <iostream>
#include <memory>

//X positive to right
//Y positive up
//Z negative means far away
//Z positive is behind projection plane

int main() {


  Render render(800, 600, {0, 0, 0});
  Sphere sphere({-5, -5, -15}, 2, Materials::red_rubber);
  Sphere sphere2({0, 0, -15}, 2, Materials::green_rubber);
  Sphere sphere3({5, 5, -15}, 2, Materials::blue_rubber);
  render.image_.DrawGradientBackground();
  render.RenderSphere(sphere);
  render.RenderSphere(sphere2);
  render.RenderSphere(sphere3);

  render.SaveImage("RenderTest.png");

  return 0;
}
