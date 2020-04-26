//
// Created by dani on 24/04/2020.
//

#include "Render.h"

#include <utility>

Render::Render(int img_width, int img_height, xyz origin, std::vector<Light> lights) : image_{img_width, img_height}, original_origin_{std::move(origin)}, lights_{std::move(lights)} {

  width_ = image_.GetImageWidth();
  height_ = image_.GetImageHeight();
  if (lights_.empty()) {
    lights_.emplace_back(Light({-20, 20, 20}, 1.5));
  }
}


void Render::SaveImage(std::string fname) const {
  image_.SaveImage(std::move(fname));
}


bool Render::scene_intersect(Vec3f const &origin, Vec3f const &direction, Vec3f &hit, Vec3f &normal, Material &mat) {
  float shapes_dist = std::numeric_limits<float>::max();
  //  for (Sphere const &s : spheres_) {
  for (auto &s : shapes_) {
    float dist_i;
    if (s->RayIntersect(origin, direction, dist_i) && dist_i < shapes_dist) {
      shapes_dist = dist_i;
      hit = origin + direction * dist_i;
      normal = s->GetNormal(hit);
      mat = s->GetMaterial();
    }
  }


  float checkerboard_dist = std::numeric_limits<float>::max();
  if (fabs(direction(1)) > 1e-3) {
    float d = -(origin(1) + 4) / direction(1);// the checkerboard plane has equation y = -4
    Vec3f pt = origin + direction * d;
    if (d > 0 && fabs(pt(0)) < 10 && pt(2) < -10 && pt(2) > -30 && d < shapes_dist) {
      checkerboard_dist = d;
      hit = pt;
      normal = Vec3f({0, 1, 0});
      mat = Materials::red_rubber;
      mat.color_f_ = (int(.5 * hit(0) + 1000) + int(.5 * hit(2))) & 1 ? Vec3f({.3, .3, .3}) : Vec3f({.3, .2, .1});
    }
  }
  return std::min(shapes_dist, checkerboard_dist) < 1000;


  //  return shapes_dist < 1000;
}

xyz Render::reflect(const xyz &I, const xyz &N) {
  auto n = I - N * 2.f * (I.dot(N));
  return n;
}

xyz Render::refract(const xyz &I, const xyz &N, const float eta_t, const float eta_i) {
  //Snell law
  float cosi = -std::max(-1.f, std::min(1.f, I.dot(N)));
  if (cosi < 0) return refract(I, -N, eta_i, eta_t);// if the ray comes from the inside the object, swap the air and the media
  float eta = eta_i / eta_t;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  xyz unit{1.0, 0.0, 0.0};
  return k < 0 ? unit : I * eta + N * (eta * cosi - sqrtf(k));// k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}


void Render::RenderScene(std::vector<std::unique_ptr<Shape>> shapes) {
  shapes_ = std::move(shapes);
  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2.f * tan(fov_ / 2.f));
  //#pragma omp parallel for
  for (int row = 0; row < height_; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;// this flips the image at the same time
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      //      PixPos pixel{col, row};
      Vec3f pix = cast_ray(original_origin_, dir, 0, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
}

void Render::RenderSceneOMP(std::vector<std::unique_ptr<Shape>> shapes) {
  shapes_ = std::move(shapes);
  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2.f * tan(fov_ / 2.f));
#pragma omp parallel for
  for (int row = 0; row < height_; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;// this flips the image at the same time
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      //      PixPos pixel{col, row};
      Vec3f pix = cast_ray(original_origin_, dir, 0, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
}

void Render::RenderSceneMultiThread(std::vector<std::unique_ptr<Shape>> shapes) {

  shapes_ = std::move(shapes);


  std::vector<std::thread> threads;

  int num_threads = std::thread::hardware_concurrency();

  int num_rows_thread{height_ / num_threads};
  int num_rows_last_thread = num_rows_thread + (height_ % num_threads);

  for (int i{0}; i < num_threads - 1; i++) {
    threads.emplace_back(std::thread(&Render::RenderThread, this, i * num_rows_thread, num_rows_thread));
  }
  threads.emplace_back(std::thread(&Render::RenderThread, this, (num_threads - 1) * num_rows_thread, num_rows_last_thread));

  for (auto &thread : threads) {
    thread.join();
  }
}


void Render::RenderThread(int const &row_init, int const &row_n) {

  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2. * tan(fov_ / 2.));
  xyz dir;
  for (int row = row_init; row < row_init + row_n; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;// this flips the image at the same time
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      dir = {dir_x, dir_y, dir_z};
      dir.normalize();
      //      PixPos pixel{col, row};
      Vec3f pix = cast_ray(original_origin_, dir, 0, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
  //  std::cout << row_init << '\n';
}


Vec3f Render::cast_ray(const Vec3f &orig, const Vec3f &dir, size_t depth, int ray_type) {

  //MINE
  Vec3f hit{0, 0, 0};
  Vec3f normal{0, 0, 0};
  Material mat;
  if (!scene_intersect(orig, dir, hit, normal, mat) || depth > 4) {
    //    return image_.GetPixelColor(pixel);
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
      reflect_color = cast_ray(reflect_orig, reflect_dir, depth + 1, 0);
    }
    if (mat.albedo_(3) != 0) {
      refract_color = cast_ray(refract_orig, refract_dir, depth + 1, 0);
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

    rgb_f new_color = mat.DSRRColor3(diffuse_light_intensity, specular_light_intensity, reflect_color, refract_color);

    return new_color;
  }
}
void Render::RenderTriangles(std::vector<Triangle> &&triangles) {
}


void Render::ParallelQueue(std::vector<std::unique_ptr<Shape>> shapes) {
  shapes_ = std::move(shapes);

  ThreadPool pool;
  int num_threads = std::thread::hardware_concurrency();

  const int row_n{1};
  for (int i{0}; i < height_; i++) {
    pool.queue([&, i]() { RenderThread(i, row_n); });
  }

  pool.start(num_threads);
}
