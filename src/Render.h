//
// Created by dani on 24/04/2020.
//

#ifndef TRACERR_RENDER_H
#define TRACERR_RENDER_H

#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "ObjLoader.h"
#include "Rectangle.h"
#include "Sphere.h"
#include "ThreadPool.h"
#include "Triangle.h"
#include <eigen3/Eigen/Core>
#include <mutex>
#include <thread>

// Main class for the raytracing
// Owns the shapes to be rendered (stored in the heap)
// The render casts rays through every pixel and projects them to the scene
// If no object is hit, the render saves the background color as the value for that pixel
// If an object is hit, the render calculates the reflections and refractions from that object recursively by casting rays from the hit point at the reflected and refracted angles
// Then it also checks for line of sight to the different lights to calculate shadows
// finally it calculates the definitive value for the pixel by taking into account the phong illumination model (ambient, diffused and specular) and reflections and refractions
// The raytracing can be run in series, in parallel using OpenMP or in parallel using our own thread pool implementation
// https://en.wikipedia.org/wiki/Phong_shading for more info on phong illumination model
class Render {
  public:
  explicit Render(int img_width = 800, int img_height = 600, xyz origin = {0, 0, 0}, std::vector<Light> lights = {}, bool grad_background = false);
  void SaveImage(std::string fname) const;
  void RenderScene(std::vector<std::unique_ptr<Shape>> shapes = std::vector<std::unique_ptr<Shape>>());
  void RenderSceneOMP(std::vector<std::unique_ptr<Shape>> shapes = std::vector<std::unique_ptr<Shape>>());
  void RenderThread(const int &row_init, const int &row_n);
  void RenderSceneMultiThread(std::vector<std::unique_ptr<Shape>> shapes = std::vector<std::unique_ptr<Shape>>());
  void LoadObj(std::string fname, xyz const &translation = {0, 0, 0}, Material const &mat = Materials::red_rubber);

  private:
  xyz reflect(const xyz &I, const xyz &N);
  xyz refract(const xyz &I, const xyz &N, const float eta_t, const float eta_i = 1.f);
  bool scene_intersect(xyz const &origin, xyz const &direction, xyz &hit, xyz &normal, Material &mat);
  rgb_f cast_ray(const xyz &orig, const xyz &dir, size_t depth = 0);
  bool enable_chessboard_;
  Image image_;
  Image background_;
  rgb_f background_color_{50.0 / 255.0, 180.0 / 255.0, 205.0 / 255.0};
  float fov_{M_PI / 3.0};
  xyz image_origin_;
  std::vector<std::unique_ptr<Shape>> shapes_;
  std::vector<Light> lights_;
  int width_;
  int height_;
  friend class SceneManager;
  std::mutex mtx_;
};


#endif//TRACERR_RENDER_H
