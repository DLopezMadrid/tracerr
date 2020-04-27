#include "ObjLoader.h"
#include "Render.h"
#include <chrono>
#include <eigen3/Eigen/Geometry>
#include <memory>

// Coordinate frame of reference
// Camera is at (0,0,0)
// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

float Material::ambient_light{0.05};


int main() {

  auto start_t{std::chrono::high_resolution_clock::now()};

  srand(time(NULL));

  //Image parameters & number of randomly generated small spheres for the demo scene
  int width{2000};
  int height{1000};
  int num_spheres{400};
  std::string fname{"RenderTest.png"};

  //Scene setup
  std::vector<std::unique_ptr<Shape>> shapes;
  std::vector<Light> lights;

  lights.emplace_back(Light({-20, 20, 10}, 1.5));
  lights.emplace_back(Light({20, 10, -20}, 1.8));
  lights.emplace_back(Light({0, 20, 0}, 0.5));
  lights.emplace_back(Light({20, 20, -5}, 0.5));

  Render r(width, height, {0, 0, 0}, std::move(lights));

  //Bigger spheres
  Sphere s1({-5, 0, -6}, 1, Materials::red_plastic);
  Sphere s2({0, 0.5, -4}, 1.5, Materials::mirror);
  Sphere s3({-3, 0, -5}, 1, Materials::glass);
  Sphere s4({-5, 0, 3}, 1, Materials::green_rubber);
  Sphere s5({0, 0, 3}, 1, Materials::pink_rubber);
  Sphere s6({5, 0, 3}, 1, Materials::orange_rubber);

  shapes.push_back(std::make_unique<Sphere>(std::move(s1)));
  shapes.push_back(std::make_unique<Sphere>(std::move(s2)));
  shapes.push_back(std::make_unique<Sphere>(std::move(s3)));
  shapes.push_back(std::make_unique<Sphere>(std::move(s4)));
  shapes.push_back(std::make_unique<Sphere>(std::move(s5)));
  shapes.push_back(std::make_unique<Sphere>(std::move(s6)));

  //Randomly generated small spheres
  for (int i{0}; i < num_spheres; i++) {
    float xpos;
    float zpos;
    float radius{0.225};
    float f1{(((rand() % 255) + 50) % 255) / 1.0f};
    float f2{(((rand() % 255) + 50) % 255) / 1.0f};
    float f3{(((rand() % 255) + 50) % 255) / 1.0f};

    Material mat({f1, f2, f3}, {0.6, 0.1, 0.0, 0.0}, 10, 1.0);
    xpos = -10.0f + (rand() % 20) + (rand() % 2) / 2.0f;
    zpos = -10.0f + (rand() % 25) + (rand() % 2) / 2.0f;

    Sphere ns({xpos, (-1.0f + radius), zpos}, radius, mat);

    shapes.push_back(std::make_unique<Sphere>(std::move(ns)));
  }

  //Render the scene using single thread, OpenMP multithreading or our own implementation
  //  r.RenderScene(std::move(shapes));
  //  r.RenderSceneOMP(std::move(shapes));
  r.LoadObj("../obj/duck.obj", {1, 3, 1}, Materials::blue_rubber);
  r.RenderSceneMultiThread(std::move(shapes));
  r.SaveImage(fname);

  auto end_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_t);
  std::cout << "Completed " << width << "x" << height << " image in " << end_t.count() << " ms" << std::endl;

  return 0;
}