#include "Render.h"


// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

int main() {

  //  std::vector<Light> lights;
  //  lights.emplace_back(Light({-20, 20, 10}, 1.5));
  //  lights.emplace_back(Light({20, 10, -20}, 1.8));
  //  lights.emplace_back(Light({5, -10, -30}, 0.8));
  //  Render render(800, 600, {0, 0, 0});
  //  Render render2(600, 400, {0, 0, 0}, lights);
  //  //  Sphere sphere({-1, -1, -10}, 1, Materials::red_rubber);
  //  Sphere sphere({1.5, -0.50, -18}, 3, Materials::red_rubber);
  //  Sphere sphere2({0, 0, -15}, 2, Materials::green_rubber);
  //  Sphere sphere3({1, 1, -20}, 2, Materials::blue_rubber);
  //  Sphere sphere4({5, 5, -25}, 2, Materials::black_plastic);
  //  //  Sphere sphere5({5, -5, -15}, 2, Materials::ivory);
  //  Sphere sphere5({-3, 0, -16}, 2, Materials::ivory);
  //  //  Sphere sphere6({-5, -5, -15}, 2, Materials::mirror);
  //  Sphere sphere6({7, 5, -18}, 4, Materials::mirror);
  //  Sphere sphere7({-2, 2, -12}, 1.5, Materials::glass);
  //
  //  std::vector<Sphere> spheres;
  //  spheres.push_back(sphere);
  //  //  spheres.push_back(sphere2);
  //  //  spheres.push_back(sphere3);
  //  //  spheres.push_back(sphere4);
  //  spheres.push_back(sphere5);
  //  spheres.push_back(sphere6);
  //  spheres.push_back(sphere7);
  //
  //  //  render.image_.DrawGradientBackground();
  //  //  render.RenderSphere(sphere);
  //  //  render.RenderSphere(sphere2);
  //  //  render.RenderSphere(sphere3);
  //  //  render.RenderSphere(sphere4);
  //  //  render.SaveImage("RenderSinglesTest.png");
  //
  //  //  render2.image_.DrawGradientBackground();
  //  render2.RenderScene(spheres);
  //  render2.SaveImage("RenderRefractionsTestNew.png");
  //

  //-----------------------------

  //
  //  RenderRep r;
  //  Material      ivory(1.0, Vec4f({0.6, 0.3, 0.1, 0.0}), Vec3f({0.4, 0.4, 0.3}),   50.);
  //  Material      glass(1.5, Vec4f({0.0, 0.5, 0.1, 0.8}), Vec3f({0.6, 0.7, 0.8}),  125.);
  //  Material red_rubber(1.0, Vec4f({0.9, 0.1, 0.0, 0.0}), Vec3f({0.3, 0.1, 0.1}),   10.);
  //  Material     mirror(1.0, Vec4f({0.0, 10.0, 0.8, 0.0}), Vec3f({1.0, 1.0, 1.0}), 1425.);
  //
  //  std::vector<Sphere> spheres;
  ////  spheres.push_back(Sphere(Vec3f({-3,    0,   -16}), 2,      ivory));
  ////  spheres.push_back(Sphere(Vec3f({-1.0, -1.5, -12}), 2,      glass));
  ////  spheres.push_back(Sphere(Vec3f({ 1.5, -0.5, -18}), 3, red_rubber));
  ////  spheres.push_back(Sphere(Vec3f({ 7,    5,   -18}), 4,     mirror));
  //
  //
  //  spheres.push_back(Sphere(Vec3f({-3, 0, -16}), 2,      ivory));
  //  spheres.push_back(Sphere(Vec3f({-2.0, 2, -12}), 1.5,      glass));
  //  spheres.push_back(Sphere(Vec3f({1.5, -0.5, -18}), 3, red_rubber));
  //  spheres.push_back(Sphere(Vec3f( {7, 5, -18}), 4,     mirror));
  //
  //
  //  std::vector<Light>  lights;
  ////  lights.push_back(Light(Vec3f({-20, 20, 20}), 1.5));
  ////  lights.push_back(Light(Vec3f({30, 50, -25}), 1.8));
  ////  lights.push_back(Light(Vec3f({30, 20, 30}), 1.7));
  //  lights.push_back(Light(Vec3f({-20, 20, 10}), 1.5));
  //  lights.push_back(Light(Vec3f({20, 10, -20}), 1.8));
  //  lights.push_back(Light(Vec3f({5, -10, -30}), 0.8));
  //
  //  r.render(spheres, lights);
  //


  // ----------------------------

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
  //  r.render(spheres, lights);
  r.RenderScene(spheres);
  r.SaveImage("RenderSwapTest2.png");
  //

  return 0;
}
