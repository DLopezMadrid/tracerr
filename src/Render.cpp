//
// Created by dani on 22/04/2020.
//

#include "Render.h"
Render::Render(int img_width, int img_height, xyz origin, std::vector<Light> lights) : image_{img_width, img_height}, origin_{origin}, lights_{lights} {

  if (lights_.empty()) {
    lights_.emplace_back(Light({-20, 20, 20}, 1.5));
  }
}

bool Render::SceneIntersect(xyz origin, xyz direction, std::vector<Sphere> spheres, xyz &hit, xyz &normal, Material &mat) {
  float spheres_dist = std::numeric_limits<float>::max();
  for (Sphere s : spheres) {
    float dist_i;
    if (s.RayIntersect(origin, direction, dist_i) && dist_i < spheres_dist) {
      spheres_dist = dist_i;
      hit = origin_ + direction * dist_i;
      normal = (hit - s.GetPos());
      normal.normalize();
      mat = s.GetMaterial();
    }
  }
  return spheres_dist < 1000;
}

xyz Render::Reflect(const xyz &I, const xyz &N) {
  auto n = I - N * 2.f * (I.dot(N));
  return n;
}

xyz Render::Refract(const xyz &I, const xyz &N, const float eta_t, const float eta_i = 1.f) {
  //Snell law
  float cosi = -std::max(-1.f, std::min(1.f, I.dot(N)));
  if (cosi < 0) return Refract(I, -N, eta_i, eta_t);// if the ray comes from the inside the object, swap the air and the media
  float eta = eta_i / eta_t;
  float k = 1 - eta * eta * (1 - cosi * cosi);
  xyz unit{1.0, 0.0, 0.0};
  return k < 0 ? unit : I * eta + N * (eta * cosi - sqrtf(k));// k<0 = total reflection, no ray to refract. I refract it anyways, this has no physical meaning
}

rgb Render::CastRay(xyz origin, xyz direction, std::vector<Sphere> spheres, PixPos pixel, int recursive_depth) {
  xyz hit{0, 0, 0};
  xyz normal{0, 0, 0};
  Material mat;
  if (!SceneIntersect(origin, direction, spheres, hit, normal, mat) || recursive_depth > 4) {
    //    return image_.GetPixelColor(pixel);
    return rgb({50, 180, 205});
  } else {

    xyz reflect_dir = Reflect(direction, normal);
    reflect_dir.normalize();
    xyz refract_dir = Refract(direction, normal, mat.refractive_index);
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


    rgb reflect_color = CastRay(reflect_orig, reflect_dir, spheres, pixel, recursive_depth + 1);
    rgb refract_color = CastRay(refract_orig, refract_dir, spheres, pixel, recursive_depth + 1);

    float diffuse_light_intensity = 0;
    float specular_light_intensity = 0;
    for (auto light : lights_) {
      xyz light_dir = (light.position - hit);
      light_dir.normalize();
      float light_distance = (light.position - hit).norm();

      xyz shadow_orig;
      if (light_dir.dot(normal) < 0) {
        shadow_orig = hit - normal * 1e-3;
      } else {
        shadow_orig = hit + normal * 1e-3;
      }

      //    xyz shadow_orig = light_dir.dot(normal) < 0 ? hit - normal * 1e-3 : hit + normal * 1e-3;// checking if the point lies in the shadow of the lights[i]
      xyz shadow_pt, shadow_N;
      Material tmpmaterial;
      if (SceneIntersect(shadow_orig, light_dir, spheres, shadow_pt, shadow_N, tmpmaterial) && (shadow_pt - shadow_orig).norm() < light_distance) {
        continue;
      }

      diffuse_light_intensity += light.intensity * std::max(0.f, light_dir.dot(normal));
      specular_light_intensity += powf(std::max(0.f, (-1.0f * Reflect(-light_dir, normal)).dot(direction)), mat.specular_comp_) * light.intensity;
    }
    //    return mat.DiffuseSpecularColor(diffuse_light_intensity, specular_light_intensity);
    //        return mat.DiffuseSpecularReflectionColor(diffuse_light_intensity, specular_light_intensity, reflect_color);
    //    return mat.DiffuseSpecularReflectionRefractionColor(diffuse_light_intensity, specular_light_intensity, reflect_color, refract_color);
    return mat.DSRRColor(diffuse_light_intensity, specular_light_intensity, reflect_color, refract_color);
  }
}

rgb Render::CastRay(xyz direction, Sphere sphere, PixPos pixel) {
  float sphere_dist = std::numeric_limits<float>::max();
  if (!sphere.RayIntersect(origin_, direction, sphere_dist)) {
    return image_.GetPixelColor(pixel);
  } else {
    return sphere.GetColor();
  }
}


void Render::RenderSphere(Sphere s) {
  int width{image_.GetImageWidth()};
  int height{image_.GetImageHeight()};
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      float x = (2 * (col + 0.5) / (float) width - 1) * tan(fov_ / 2.) * width / (float) height;
      float y = -(2 * (row + 0.5) / (float) height - 1) * tan(fov_ / 2.);
      xyz dir{x, y, -1};
      dir.normalize();
      PixPos pixel{col, row};
      image_.SetPixelColor(pixel, CastRay(dir, s, pixel));
    }
  }
}
xyz Render::GetRay(xyz direction) {
  return xyz();
}
void Render::SaveImage(std::string fname) const {
  image_.SaveImage(fname);
}
void Render::RenderScene(std::vector<Sphere> spheres) {
  int width{image_.GetImageWidth()};
  int height{image_.GetImageHeight()};
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      //      float x = (2 * (col + 0.5) / (float) width - 1) * tan(fov_ / 2.) * width / (float) height;
      //      float y = -(2 * (row + 0.5) / (float) height - 1) * tan(fov_ / 2.);
      //      xyz dir{x, y, -1};
      float dir_x = (col + 0.5) - width / 2.;
      float dir_y = -(row + 0.5) + height / 2.;// this flips the image at the same time
      float dir_z = -height / (2. * tan(fov_ / 2.));
      xyz dir{dir_x, dir_y, dir_z};
      dir.normalize();
      PixPos pixel{col, row};
      image_.SetPixelColor(pixel, CastRay(origin_, dir, spheres, pixel, 0));
    }
  }
}

