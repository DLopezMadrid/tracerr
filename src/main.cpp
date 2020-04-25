#include "Render.h"
#include <chrono>


// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

int main() {

  auto start_t{std::chrono::high_resolution_clock::now()};
  int width{3000};
  int height{3000};

  std::vector<Sphere> spheres;
  spheres.emplace_back(Vec3f({-3, 0, -16}), 2, Materials::ivory);
  spheres.emplace_back(Vec3f({-2.0, 2, -12}), 1.5, Materials::glass);
  spheres.emplace_back(Vec3f({1.5, -0.5, -18}), 3, Materials::red_rubber);
  spheres.emplace_back(Vec3f({7, 5, -18}), 4, Materials::mirror);

  std::vector<Light> lights;
  lights.emplace_back(Vec3f({-20, 20, 10}), 1.5);
  lights.emplace_back(Vec3f({20, 10, -20}), 1.8);
  lights.emplace_back(Vec3f({5, -10, -30}), 0.8);

  Render r(width, height, {0, 0, 0}, std::move(lights));
  //  r.RenderScene(std::move(spheres));
  //  r.RenderSceneMultiThread(std::move(spheres));
  r.RenderSceneOMP(std::move(spheres));
  r.SaveImage("Render8bit.png");

  auto end_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_t);
  std::cout << "Completed " << width << "x" << height << " image in " << end_t.count() << " ms" << std::endl;

  return 0;
}