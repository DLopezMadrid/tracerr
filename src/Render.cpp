//
// Created by dani on 24/04/2020.
//

#include "Render.h"


bool Render::scene_intersect(Vec3f origin, Vec3f direction, Vec3f &hit, Vec3f &normal, Material &mat) {
  float spheres_dist = std::numeric_limits<float>::max();
  for (Sphere const &s : spheres_) {
    float dist_i;
    if (s.RayIntersect(origin, direction, dist_i) && dist_i < spheres_dist) {
      spheres_dist = dist_i;
      hit = origin + direction * dist_i;
      normal = (hit - s.GetPos());
      normal.normalize();
      mat = s.GetMaterial();
    }
  }
  return spheres_dist < 1000;
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


void Render::RenderScene(std::vector<Sphere> &&spheres) {
  int width{image_.GetImageWidth()};
  int height{image_.GetImageHeight()};
  spheres_ = std::move(spheres);
  float dir_x, dir_y, dir_z;
  dir_z = -height / (2. * tan(fov_ / 2.));
#pragma omp parallel for
  for (int row = 0; row < height; row++) {
    dir_y = -(row + 0.5) + height / 2.;// this flips the image at the same time
    for (int col = 0; col < width; col++) {
      dir_x = (col + 0.5) - width / 2.;
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      //      PixPos pixel{col, row};
      Vec3f pix = cast_ray(original_origin_, dir, 0, 0);
      rgb rgb_val = Material::vec2rgb(pix);
      image_.SetPixelColor({col, row}, rgb_val);
    }
  }
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

    Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, depth + 1, 0);
    Vec3f refract_color = cast_ray(refract_orig, refract_dir, depth + 1, 0);
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
Render::Render(int img_width, int img_height, xyz origin, std::vector<Light> lights) : image_{img_width, img_height}, original_origin_{origin}, lights_{lights} {

  if (lights_.empty()) {
    lights_.emplace_back(Light({-20, 20, 20}, 1.5));
  }
}
void Render::SaveImage(std::string fname) const {
  image_.SaveImage(fname);
}
