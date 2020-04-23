#include "Render.h"
#include "Sphere.h"


// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

int main() {


  Render render(800, 600, {0, 0, 0});
  Render render2(800, 600, {0, 0, 0});
  Sphere sphere({-1, -1, -10}, 2, Materials::red_rubber);
  Sphere sphere2({0, 0, -15}, 2, Materials::green_rubber);
  Sphere sphere3({1, 1, -20}, 2, Materials::blue_rubber);

  std::vector<Sphere> spheres;
  spheres.push_back(sphere);
  spheres.push_back(sphere2);
  spheres.push_back(sphere3);

  render.image_.DrawGradientBackground();
  render.RenderSphere(sphere);
  render.RenderSphere(sphere2);
  render.RenderSphere(sphere3);
  render.SaveImage("RenderSingleTest.png");

  render2.image_.DrawGradientBackground();
  render2.RenderScene(spheres);
  render2.SaveImage("RenderMultiTest.png");

  return 0;
}
