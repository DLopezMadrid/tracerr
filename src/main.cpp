#include "Render.h"
#include <chrono>
#include <eigen3/Eigen/Geometry>
#include <memory>


// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

int main() {

  auto start_t{std::chrono::high_resolution_clock::now()};
  int width{1200};
  int height{1200};

  std::vector<Triangle> triangles;


  Triangle t1(Vec3f({0, 0, -17}), Vec3f({4, 1, -17}), Vec3f({1, 3, -17}), Materials::mirror);
  Triangle t2(Vec3f({-2, -2, -15}), Vec3f({-4, -4, -15}), Vec3f({-2, -4, -15}), Materials::glass);//Renders well
  Triangle t3(Vec3f({5, 4, -20}), Vec3f({5, 7, -17}), Vec3f({6, 7, -16}), Materials::mirror);
  Triangle t4(Vec3f({5, -4, -14}), Vec3f({6, -4, -13}), Vec3f({5.5, -2, -13.5}), Materials::mirror);


  Sphere s1(Vec3f({-3, -3, -16}), 1, Materials::ivory);
  Sphere s2(Vec3f({0, -2.5, -14}), 1.5, Materials::mirror);
  Sphere s3(Vec3f({1.5, -2, -18}), 2, Materials::red_rubber);
  std::vector<std::unique_ptr<Shape>> shapes;

  shapes.push_back(std::make_unique<Sphere>(s1));
  shapes.push_back(std::make_unique<Sphere>(s2));
  shapes.push_back(std::make_unique<Sphere>(s3));
  shapes.push_back(std::make_unique<Triangle>(t1));
  shapes.push_back(std::make_unique<Triangle>(t3));
  shapes.push_back(std::make_unique<Triangle>(t2));
  shapes.push_back(std::make_unique<Triangle>(t4));

  int num_spheres{40};

  srand(time(NULL));

  for (int i{0}; i < num_spheres; i++) {
    int xpos;
    int zpos;
    Material mat = Materials::blue_rubber;
    //    auto r = rand();
    xpos = -10 + (rand() % 20);
    zpos = -30 + (rand() % 20);

    Sphere ns({xpos, -3.75, zpos}, 0.25, mat);

    shapes.push_back(std::make_unique<Sphere>(ns));
  }


  //  shapes.push_back(Sphere(Vec3f({-3, 0, -16}), 2, Materials::ivory));
  //  shapes.push_back(Sphere(Vec3f({-2.0, 2, -12}), 1.5, Materials::glass));
  //  shapes.push_back(Sphere(Vec3f({1.5, -0.5, -18}), 3, Materials::red_rubber));
  //  shapes.push_back(Sphere(Vec3f({7, 5, -18}), 4, Materials::mirror));
  //  shapes.push_back(Triangle(Vec3f({0,0,-17}), Vec3f({4,1,-17}), Vec3f({1,3,-17}), Materials::blue_rubber));
  //  shapes.push_back(Triangle(Vec3f({-2,-2,-15}), Vec3f({-4,-4,-15}), Vec3f({-2,-4,-17}), Materials::red_rubber));
  //  shapes.push_back(Triangle(Vec3f({5,4,-20}), Vec3f({5,7,-17}), Vec3f({6,7,-16}), Materials::green_rubber));
  std::vector<Light> lights;
  lights.emplace_back(Vec3f({-20, 20, 10}), 1.5);
  lights.emplace_back(Vec3f({20, 10, -20}), 1.8);
  lights.emplace_back(Vec3f({5, -10, -30}), 0.8);

  Render r(width, height, {0, 0, 0}, std::move(lights));
  //    r.RenderScene(std::move(shapes));
  //  r.RenderSceneMultiThread(std::move(shapes));
  r.RenderSceneOMP(std::move(shapes));
  //  r.RenderScene(shapes)
  //  r.RenderTriangles(std::move(triangles));
  r.SaveImage("RenderDP.png");

  auto end_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_t);
  std::cout << "Completed " << width << "x" << height << " image in " << end_t.count() << " ms" << std::endl;

  return 0;
}