//
// Created by dani on 24/04/2020.
//

#include "Render.h"

#include <utility>

Render::Render(int img_width, int img_height, xyz origin, std::vector<Light> lights, bool grad_background) : image_{img_width, img_height}, background_{img_width, img_height}, image_origin_{std::move(origin)}, lights_{std::move(lights)} {

  width_ = image_.GetImageWidth();
  height_ = image_.GetImageHeight();
  if (lights_.empty()) {
    lights_.emplace_back(Light({-20, 20, 20}, 1.5));
  }
  //  if (grad_background) {
  //    //unused for now
  //    background_.DrawGradientBackground();
  //  }
}


void Render::SaveImage(std::string fname) const {
  image_.SaveImage(std::move(fname));
}

// checks if a ray intersects with anything in the scene (dynamic polymorphism)
bool Render::scene_intersect(Vec3f const &origin, Vec3f const &direction, Vec3f &hit, Vec3f &normal, Material &mat) {
  float shapes_dist = std::numeric_limits<float>::max();
  for (auto &s : shapes_) {
    float dist_i;
    if (s->RayIntersect(origin, direction, dist_i) && dist_i < shapes_dist) {
      shapes_dist = dist_i;
      hit = origin + direction * dist_i;
      normal = s->GetNormal(hit);
      mat = s->GetMaterial();
    }
  }

  // draw checkerboard floor plane
  float checkerboard_dist = std::numeric_limits<float>::max();
  if (fabs(direction(1)) > 1e-3) {
    float d = -(origin(1) + 1) / direction(1);// the checkerboard plane has equation y = -1
    Vec3f pt = origin + direction * d;
    // draws the checkerboard from -20 < x < 20 and -30 < z < 30
    if (d > 0 && fabs(pt(0)) < 20 && pt(2) < 30 && pt(2) > -30 && d < shapes_dist) {
      checkerboard_dist = d;
      hit = pt;
      normal = Vec3f({0, 1, 0});
      mat = Materials::red_rubber;
      mat.color_f_ = (int(.5 * hit(0) + 1000) + int(.5 * hit(2))) & 1 ? Vec3f({.3, .3, .3}) : Vec3f({.3, .2, .1});
    }
  }
  return std::min(shapes_dist, checkerboard_dist) < 1000;

}

xyz Render::reflect(const xyz &I, const xyz &N) {
  auto n = I - N * 2.f * (I.dot(N));
  return n;
}

//Snell law
xyz Render::refract(const xyz &I, const xyz &N, const float eta_t, const float eta_i) {
  float cosi = -std::max(-1.f, std::min(1.f, I.dot(N)));
  if (cosi < 0) return refract(I, -N, eta_i, eta_t);// swap the values if th ray comes from within the object
  float eta = eta_i / eta_t;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  xyz unit{1.0, 0.0, 0.0};
  return k < 0 ? unit : I * eta + N * (eta * cosi - sqrtf(k));
}


Vec3f Render::cast_ray(const Vec3f &orig, const Vec3f &dir, size_t depth, const PixPos &pixel) {

  //MINE
  Vec3f hit{0, 0, 0};
  Vec3f normal{0, 0, 0};
  Material mat;
  if (!scene_intersect(orig, dir, hit, normal, mat) || depth > 4) {
    //TODO add background
    //    int a = std::max(0, std::min(background_.GetImageWidth() -1, static_cast<int>((atan2(dir(2), dir(0))/(2*M_PI) + .5)*background_.GetImageWidth())));
    //    int b = std::max(0, std::min(background_.GetImageHeight()-1, static_cast<int>(acos(dir(1))/M_PI*background_.GetImageHeight())));
    //    return Material::rgb2vec(background_.GetPixelColor({a,b}));
    return Vec3f({50.0 / 255.0, 180.0 / 255.0, 205.0 / 255.0});
  } else {

    Vec3f reflect_dir = reflect(dir, normal);
    reflect_dir.normalize();
    Vec3f refract_dir = refract(dir, normal, mat.refractive_index);
    refract_dir.normalize();
    Vec3f reflect_orig;
    if (reflect_dir.dot(normal) < 0) {
      reflect_orig = hit - normal * 1e-3;// offset the original point to avoid occlusion by the object itself
    } else {
      reflect_orig = hit + normal * 1e-3;
    }

    Vec3f refract_orig;
    if (refract_dir.dot(normal) < 0) {
      refract_orig = hit - normal * 1e-3;// offset the original point to avoid occlusion by the object itself
    } else {
      refract_orig = hit + normal * 1e-3;
    }

    Vec3f reflect_color;
    Vec3f refract_color;
    if (mat.albedo_(2) != 0) {
      reflect_color = cast_ray(reflect_orig, reflect_dir, depth + 1, pixel);
    }
    if (mat.albedo_(3) != 0) {
      refract_color = cast_ray(refract_orig, refract_dir, depth + 1, pixel);
    }
    float diffuse_light_intensity = 0;
    float specular_light_intensity = 0;
    for (auto const &light : lights_) {
      Vec3f light_dir = (light.position_ - hit);
      light_dir.normalize();
      float light_distance = (light.position_ - hit).norm();

      Vec3f shadow_orig;
      if (light_dir.dot(normal) < 0) {
        shadow_orig = hit - normal * 1e-3;
      } else {
        shadow_orig = hit + normal * 1e-3;
      }

      Vec3f shadow_pt, shadow_N;
      Material tmpmaterial;
      if (scene_intersect(shadow_orig, light_dir, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt - shadow_orig).norm() < light_distance) {
        continue;
      }

      diffuse_light_intensity += light.intensity_ * std::max(0.f, light_dir.dot(normal));
      specular_light_intensity += powf(std::max(0.f, (-1.0f * reflect(-light_dir, normal)).dot(dir)), mat.specular_comp_) * light.intensity_;
    }

    rgb_f new_color = mat.CalcColor(diffuse_light_intensity, specular_light_intensity, reflect_color, refract_color);

    return new_color;
  }
}


void Render::RenderScene(std::vector<std::unique_ptr<Shape>> shapes) {
  shapes_ = std::move(shapes);
  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2.f * tan(fov_ / 2.f));
  PixPos pixel;
  for (int row = 0; row < height_; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      pixel = {col, row};
      Vec3f pix = cast_ray(image_origin_, dir, 0, pixel);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
}

void Render::RenderSceneOMP(std::vector<std::unique_ptr<Shape>> shapes) {
  shapes_ = std::move(shapes);
  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2.f * tan(fov_ / 2.f));
  PixPos pixel;
#pragma omp parallel for
  for (int row = 0; row < height_; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;// this flips the image at the same time
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      pixel = {col, row};
      Vec3f pix = cast_ray(image_origin_, dir, 0, pixel);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
}



void Render::RenderThread(int const &row_init, int const &row_n) {

  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2. * tan(fov_ / 2.));
  xyz dir;
  PixPos pixel;
  for (int row = row_init; row < row_init + row_n; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;// this flips the image at the same time
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      dir = {dir_x, dir_y, dir_z};
      dir.normalize();
      pixel = {col, row};
      Vec3f pix = cast_ray(image_origin_, dir, 0, pixel);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
}

// creates a thread pool
void Render::RenderSceneMultiThread(std::vector<std::unique_ptr<Shape>> shapes) {
  if (!shapes.empty()) {
    for (auto &shape : shapes) {
      shapes_.push_back(std::move(shape));
    }
  }

  ThreadPool pool;
  int num_threads = std::thread::hardware_concurrency();

  const int row_n{1};
  for (int i{0}; i < height_; i++) {
    pool.queue([&, i]() { RenderThread(i, row_n); });
  }

  pool.start(num_threads);
}

void Render::LoadObj(std::string fname, xyz const &translation, Material const &mat) {
  ObjLoader obj;
  obj.readFile(fname.c_str(), translation, mat);

  for (int i{0}; i < obj.triangles_.size(); i++) {
    shapes_.push_back(std::make_unique<Triangle>(obj.triangles_[i]));
  }
  //  RenderSceneMultiThread();
}
