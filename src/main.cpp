#include "Render.h"


// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

int main() {

  std::vector<Sphere> spheres;
  spheres.push_back(Sphere(Vec3f({-3, 0, -16}), 2, Materials::ivory));
  spheres.push_back(Sphere(Vec3f({-2.0, 2, -12}), 1.5, Materials::glass));
  spheres.push_back(Sphere(Vec3f({1.5, -0.5, -18}), 3, Materials::red_rubber));
  spheres.push_back(Sphere(Vec3f({7, 5, -18}), 4, Materials::mirror));

  std::vector<Light> lights;
  lights.push_back(Light(Vec3f({-20, 20, 10}), 1.5));
  lights.push_back(Light(Vec3f({20, 10, -20}), 1.8));
  lights.push_back(Light(Vec3f({5, -10, -30}), 0.8));

  Render r(250, 250, {0, 0, 0}, lights);
  r.RenderScene(spheres);
  r.SaveImage("RenderSwapTest2.png");

  return 0;
}
