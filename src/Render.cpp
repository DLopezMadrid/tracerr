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
  //TODO implement background
  //  if (grad_background) {
  //    //unused for now
  //    background_.DrawGradientBackground();
  //  }
}


void Render::SaveImage(std::string fname) const {
  image_.SaveImage(std::move(fname));
}

// checks if a ray intersects with anything in the scene (dynamic polymorphism)
bool Render::scene_intersect(xyz const &origin, xyz const &direction, xyz &hit, xyz &normal, Material &mat) {
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
  if (enable_chessboard_ && fabs(direction(1)) > 1e-3) {
    float d = -(origin(1) + 1) / direction(1);// the checkerboard plane has equation y = -1
    xyz pt = origin + direction * d;
    // draws the checkerboard from -20 < x < 20 and -30 < z < 30
    if (d > 0 && fabs(pt(0)) < 20 && pt(2) < 30 && pt(2) > -30 && d < shapes_dist) {
      checkerboard_dist = d;
      hit = pt;
      normal = xyz({0, 1, 0});
      mat = Materials::chessboard;
      mat.color_f_ = (int(.5 * hit(0) + 1000) + int(.5 * hit(2))) & 1 ? rgb_f({.3, .3, .3}) : rgb_f({.3, .2, .1});
    }
  }
  return std::min(shapes_dist, checkerboard_dist) < 1000;
}

// Calculates reflected ray
xyz Render::reflect(const xyz &I, const xyz &N) {
  return I - N * 2.f * (I.dot(N));
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

// Main function for the ray tracing process
// it casts a ray using an origin point + a direction unit vector
// checks if the ray intersects with anything in the scene
// if it doesn't it returns the background color
// If it does, it calculates the resulting pixel color value by using the phong illumination model (ambient, diffusion and specular) + reflections and refractions
// To calculate reflections and refractions, it calls itself recursively with the hit point as the new origin and the reflected / refracted ray as the direction unit vector
// To calculate shadows it checks if the hit point has line of sight with the lights, if not it is in a shadow
// The final calculation is done considering the previously calculated values + the object material properties
rgb_f Render::cast_ray(const xyz &orig, const xyz &dir, size_t depth) {

  xyz hit{0, 0, 0};
  xyz normal{0, 0, 0};
  Material mat;
  if (!scene_intersect(orig, dir, hit, normal, mat) || depth > 4) {
    //TODO add background
    //    int a = std::max(0, std::min(background_.GetImageWidth() -1, static_cast<int>((atan2(dir(2), dir(0))/(2*M_PI) + .5)*background_.GetImageWidth())));
    //    int b = std::max(0, std::min(background_.GetImageHeight()-1, static_cast<int>(acos(dir(1))/M_PI*background_.GetImageHeight())));
    //    return Material::rgb2vec(background_.GetPixelColor({a,b}));

    // background color
    return background_color_;
  } else {

    xyz reflect_dir = reflect(dir, normal);
    reflect_dir.normalize();
    xyz refract_dir = refract(dir, normal, mat.refractive_index);
    refract_dir.normalize();
    xyz reflect_orig;
    if (reflect_dir.dot(normal) < 0) {
      reflect_orig = hit - normal * 1e-3;// offset the original point to avoid occlusion by the object itself
    } else {
      reflect_orig = hit + normal * 1e-3;
    }

    xyz refract_orig;
    if (refract_dir.dot(normal) < 0) {
      refract_orig = hit - normal * 1e-3;// offset the original point to avoid occlusion by the object itself
    } else {
      refract_orig = hit + normal * 1e-3;
    }

    rgb_f reflect_color;
    rgb_f refract_color;
    if (mat.albedo_(2) != 0) {
      reflect_color = cast_ray(reflect_orig, reflect_dir, depth + 1);
    }
    if (mat.albedo_(3) != 0) {
      refract_color = cast_ray(refract_orig, refract_dir, depth + 1);
    }
    float diffuse_light_intensity = 0;
    float specular_light_intensity = 0;
    for (auto const &light : lights_) {
      xyz light_dir = (light.position_ - hit);
      light_dir.normalize();
      float light_distance = (light.position_ - hit).norm();

      xyz shadow_orig;
      if (light_dir.dot(normal) < 0) {
        shadow_orig = hit - normal * 1e-3;
      } else {
        shadow_orig = hit + normal * 1e-3;
      }

      xyz shadow_pt;
      xyz shadow_N;
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

// Plain render function using a single thread
void Render::RenderScene(std::vector<std::unique_ptr<Shape>> shapes) {
  if (!shapes.empty()) {
    for (auto &shape : shapes) {
      shapes_.push_back(std::move(shape));
    }
  }
  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2.f * tan(fov_ / 2.f));
  for (int row = 0; row < height_; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      rgb_f pix = cast_ray(image_origin_, dir, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
    //    std::cout << row << '\n';
  }
}

// Uses OpenMP for parallelization. In order for this to work you need to set up a cmake environment with the -fopenmp flag
// the pragma statement below automatically parallelizes the content of the for loop
void Render::RenderSceneOMP(std::vector<std::unique_ptr<Shape>> shapes) {
  if (!shapes.empty()) {
    for (auto &shape : shapes) {
      shapes_.push_back(std::move(shape));
    }
  }
  float dir_x, dir_y, dir_z;
  dir_z = -height_ / (2.f * tan(fov_ / 2.f));
#pragma omp parallel for
  for (int row = 0; row < height_; row++) {
    dir_y = -(row + 0.5f) + height_ / 2.f;// this flips the image at the same time
    for (int col = 0; col < width_; col++) {
      dir_x = (col + 0.5f) - width_ / 2.f;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      rgb_f pix = cast_ray(image_origin_, dir, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
}


// Function to be used for an indivudual thread in a multi threading context
// The thread will render row_n number of lines starting from row_init and write the results to the image file
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
      rgb_f pix = cast_ray(image_origin_, dir, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
  if ((row_init % 20) == 0){
    std::lock_guard<std::mutex>(this->mtx_);
    int barWidth = 30;
    float progress = static_cast<float>(row_init) /  static_cast<float>(height_);
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
      if (i < pos) std::cout << "=";
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
  }
}

// creates a thread pool and splits the image into line tasks for the pool workers
void Render::RenderSceneMultiThread(std::vector<std::unique_ptr<Shape>> shapes) {
  if (!shapes.empty()) {
    for (auto &shape : shapes) {
      shapes_.push_back(std::move(shape));
    }
  }
  ThreadPool pool;
  int num_threads = std::thread::hardware_concurrency();
  // to avoid allocation and initialization at every call
  const int row_n{1};
  for (int i{0}; i < height_; i++) {
    pool.queue([&, i]() { RenderThread(i, row_n); });
  }

  pool.start(num_threads);
}


// Reads and parses the .obj file, then puts all the triangles into the shapes_ vector
void Render::LoadObj(std::string fname, xyz const &translation, Material const &mat) {
  ObjLoader obj;
  obj.readFile2(fname.c_str(), translation, mat);

  for (auto& triangle : obj.triangles_ ){
    shapes_.push_back(std::make_unique<Triangle>(triangle));
  }
  for (auto& rectangle: obj.rectangles_){
    shapes_.push_back(std::make_unique<Rectangle>(rectangle));
  }
}
