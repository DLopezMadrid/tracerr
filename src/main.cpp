#include "Render.h"
#include <chrono>


// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

int main() {

  auto start_t{std::chrono::high_resolution_clock::now()};
  int width{250};
  int height{250};

  std::vector<Sphere> spheres;
  spheres.push_back(Sphere(Vec3f({-3, 0, -16}), 2, Materials::ivory));
  spheres.push_back(Sphere(Vec3f({-2.0, 2, -12}), 1.5, Materials::glass));
  spheres.push_back(Sphere(Vec3f({1.5, -0.5, -18}), 3, Materials::red_rubber));
  spheres.push_back(Sphere(Vec3f({7, 5, -18}), 4, Materials::mirror));

  std::vector<Light> lights;
  lights.push_back(Light(Vec3f({-20, 20, 10}), 1.5));
  lights.push_back(Light(Vec3f({20, 10, -20}), 1.8));
  lights.push_back(Light(Vec3f({5, -10, -30}), 0.8));

  Render r(2500, 2500, {0, 0, 0}, lights);
  r.RenderScene(std::move(spheres));
  r.SaveImage("RenderSwapTest3.png");

  auto end_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_t);
  std::cout << "Completed " << width << "x" << height << " image in " << end_t.count() << " ms" << std::endl;

  return 0;
}
