//
// Created by dani on 30/04/2020.
//

#include "SceneManager.h"

// Protobuf docs and examples
// https://developers.google.com/protocol-buffers/docs/reference/cpp-generated#fields
// https://github.com/protocolbuffers/protobuf/tree/master/examples
//
//


// Raytracing examples
// https://github.com/VityasZV/tinyraytracer/blob/master/raytracing/raytracing.cpp

SceneManager::SceneManager() {
  proto = std::make_unique<tracerr::Scene>();
}

void SceneManager::LoadScene(char *proto_path_char) {
  proto_path_ = std::string(proto_path_char);
  auto start_t{std::chrono::high_resolution_clock::now()};

  std::cout << "Loading proto file" << '\n';
  LoadTextProto();
  std::cout << "Loading scene config" << '\n';
  LoadSceneConfig();
  std::cout << "Loading lights" << '\n';
  LoadLights();
  std::cout << "Loading spheres" << '\n';
  LoadSpheres();
  std::cout << "Loading triangles" << '\n';
  LoadTriangles();
  std::cout << "Loading rectangles" << '\n';
  LoadRectangles();
  std::cout << "Loading .obj files" << '\n';
  LoadObjs();

  Render render(width_, height_, {0, 0, 0}, lights_);
  render.enable_chessboard_ = show_chessboard_;
  if (scene_in_.has_background_color()) {
    render.background_color_ = background_color_;
  }
  std::cout << "Rendering" << '\n';
  //  GenerateRandomSpheres();
  //  SaveSceneToProto();
  render.RenderSceneMultiThread(std::move(shapes_));
  if (scene_in_.savefile()) {
    std::cout << "Saving file as " << scene_in_.fname() << '\n';
    render.SaveImage(scene_in_.fname());
  }
  std::cout << "Done" << '\n';

  auto end_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_t);

  if (show_elapsed_time_) {
    std::cout << "Completed " << scene_in_.width() << "x" << scene_in_.height() << " image in " << end_t.count() << " ms" << std::endl;
  }
}

bool SceneManager::LoadTextProto() {
  std::ifstream ifs(proto_path_);
  google::protobuf::io::IstreamInputStream iis(&ifs);
  auto success = google::protobuf::TextFormat::Parse(&iis, proto.get());
  if (!success) {
    std::cerr << "Failed to parse scene" << std::endl;
    return -1;
  }
  return true;
}

void SceneManager::LoadSceneConfig() {
  scene_in_ = *((tracerr::Scene *) proto.get());
  width_ = scene_in_.width();
  height_ = scene_in_.height();
  show_chessboard_ = scene_in_.show_checkerboard();
  show_elapsed_time_ = scene_in_.show_elapsed_time();
  if (scene_in_.has_ambient_light()) {
    Material::ambient_light = scene_in_.ambient_light();
  }
  if (scene_in_.has_background_color()) {
    background_color_(0) = scene_in_.background_color().r() / 255.0f;
    background_color_(1) = scene_in_.background_color().g() / 255.0f;
    background_color_(2) = scene_in_.background_color().b() / 255.0f;
  }
}

void SceneManager::LoadLights() {
  for (int i{0}; i < scene_in_.light_size(); i++) {
    xyz pos{scene_in_.light(i).position().x(), scene_in_.light(i).position().y(), scene_in_.light(i).position().z()};
    lights_.emplace_back(pos, scene_in_.light(i).intensity());
  }
}

void SceneManager::LoadSpheres() {
  Material mat = Materials::red_rubber;
  for (int i{0}; i < scene_in_.sphere_size(); i++) {
    xyz pos{scene_in_.sphere(i).position().x(), scene_in_.sphere(i).position().y(), scene_in_.sphere(i).position().z()};
    if (scene_in_.sphere(i).custom_material_enabled()) {
      mat = GetCustomMaterialFromProto(scene_in_.sphere(i).custom_material());
    } else {
      mat = LoadMaterial(scene_in_.sphere(i).material());
    }
    shapes_.push_back(std::make_unique<Sphere>(pos, scene_in_.sphere(i).radius(), mat));
  }
}

void SceneManager::LoadTriangles() {
  Material mat = Materials::red_rubber;
  for (int i{0}; i < scene_in_.triangle_size(); i++) {
    xyz p0{scene_in_.triangle(i).p0().x(), scene_in_.triangle(i).p0().y(), scene_in_.triangle(i).p0().z()};
    xyz p1{scene_in_.triangle(i).p1().x(), scene_in_.triangle(i).p1().y(), scene_in_.triangle(i).p1().z()};
    xyz p2{scene_in_.triangle(i).p2().x(), scene_in_.triangle(i).p2().y(), scene_in_.triangle(i).p2().z()};
    if (scene_in_.triangle(i).custom_material_enabled()) {
      mat = GetCustomMaterialFromProto(scene_in_.triangle(i).custom_material());
    } else {
      mat = LoadMaterial(scene_in_.triangle(i).material());
    }
    shapes_.push_back(std::make_unique<Triangle>(p0, p1, p2, mat));
  }
}

void SceneManager::LoadRectangles() {
  Material mat = Materials::red_rubber;
  for (int i{0}; i < scene_in_.rectangle_size(); i++) {
    xyz p0{scene_in_.rectangle(i).p0().x(), scene_in_.rectangle(i).p0().y(), scene_in_.rectangle(i).p0().z()};
    xyz p1{scene_in_.rectangle(i).p1().x(), scene_in_.rectangle(i).p1().y(), scene_in_.rectangle(i).p1().z()};
    xyz p2{scene_in_.rectangle(i).p2().x(), scene_in_.rectangle(i).p2().y(), scene_in_.rectangle(i).p2().z()};
    xyz p3{scene_in_.rectangle(i).p3().x(), scene_in_.rectangle(i).p3().y(), scene_in_.rectangle(i).p3().z()};
    if (scene_in_.rectangle(i).custom_material_enabled()) {
      mat = GetCustomMaterialFromProto(scene_in_.rectangle(i).custom_material());
    } else {
      mat = LoadMaterial(scene_in_.rectangle(i).material());
    }
    shapes_.push_back(std::make_unique<Rectangle>(p0, p1, p2, p3, mat));
  }
}

void SceneManager::LoadObjs() {
  ObjLoader objLoader;
  Material mat = Materials::red_rubber;
  for (int i{0}; i < scene_in_.obj_size(); i++) {
    std::string obj_fname = scene_in_.obj(i).fname();
    xyz translation{scene_in_.obj(i).translation().x(), scene_in_.obj(i).translation().y(), scene_in_.obj(i).translation().z()};
    if (scene_in_.obj(i).custom_material_enabled()) {
      mat = GetCustomMaterialFromProto(scene_in_.obj(i).custom_material());
    } else {
      mat = LoadMaterial(scene_in_.obj(i).material());
    }
    objLoader.readFile2(obj_fname.c_str(), translation, mat);

    for (int j{0}; j < objLoader.triangles_.size(); j++) {
      shapes_.push_back(std::make_unique<Triangle>(objLoader.triangles_[j]));
    }
    for (int k{0}; k < objLoader.rectangles_.size(); k++) {
      shapes_.push_back(std::make_unique<Rectangle>(objLoader.rectangles_[k]));
    }
  }
}

Material SceneManager::LoadMaterial(int m) {
  Material mat{Materials::red_rubber};

  if (m == 0) {
    mat = Materials::ivory;
  } else if (m == 1) {
    mat = Materials::red_rubber;
  } else if (m == 2) {
    mat = Materials::green_rubber;
  } else if (m == 3) {
    mat = Materials::blue_rubber;
  } else if (m == 4) {
    mat = Materials::orange_rubber;
  } else if (m == 5) {
    mat = Materials::pink_rubber;
  } else if (m == 6) {
    mat = Materials::red_plastic;
  } else if (m == 7) {
    mat = Materials::black_plastic;
  } else if (m == 8) {
    mat = Materials::mirror;
  } else if (m == 9) {
    mat = Materials::glass;
  } else if (m == 10) {
    mat = Materials::chessboard;
  }
  return mat;
}

Material SceneManager::GetCustomMaterialFromProto(const tracerr::Scene::Material &proto_mat) {
  Material mat;
  mat.color_f_(0) = proto_mat.color().r() / 255.f;
  mat.color_f_(1) = proto_mat.color().g() / 255.f;
  mat.color_f_(2) = proto_mat.color().b() / 255.f;
  mat.albedo_(0) = proto_mat.albedo().a0();
  mat.albedo_(1) = proto_mat.albedo().a1();
  mat.albedo_(2) = proto_mat.albedo().a2();
  mat.albedo_(3) = proto_mat.albedo().a3();
  mat.refractive_index = proto_mat.refractive_index();
  mat.specular_comp_ = proto_mat.specular_comp();

  return mat;
}

void SceneManager::SaveSceneToProto() {
  scene_out_.set_height(height_);
  scene_out_.set_width(width_);
  scene_out_.set_savefile(true);
  scene_out_.set_fname("GeneratedProtoImg.png");
  scene_out_.set_ambient_light(Material::ambient_light);

  for (auto &l : lights_) {
    auto new_light = scene_out_.add_light();
    Light2ProtoMessage(new_light, l);
  }

  for (auto &s : shapes_) {
    switch (s->GetType()) {
      case shapeTypes::sph: {
        auto new_sphere = scene_out_.add_sphere();
        Sphere2ProtoMessage(new_sphere, s.get());
        break;
      }
      case shapeTypes::tri: {
        auto new_triangle = scene_out_.add_triangle();
        Triangle2ProtoMessage(new_triangle, s.get());
        break;
      }
      case shapeTypes::rec: {
        auto new_rectangle = scene_out_.add_rectangle();
        Rectangle2ProtoMessage(new_rectangle, s.get());
        break;
      }
    }
  }

  WriteProtoText("./../scenes/new_scene.textproto");
}

bool SceneManager::WriteProtoText(const std::string &save_file_path) {
  std::ofstream ofs(save_file_path);
  ofs << "# proto-file: ./../proto/scene.proto \n";
  ofs << "# proto-message: Scene\n";

  auto *oos = new google::protobuf::io::OstreamOutputStream(&ofs);
  auto success = google::protobuf::TextFormat::Print(scene_out_, oos);
  delete oos;
  if (!success) {
    std::cerr << "Failed to parse scene" << std::endl;
    return -1;
  }

  return true;
}

void SceneManager::Light2ProtoMessage(tracerr::Scene::Light *new_light, Light l) {
  auto *p = new tracerr::Scene::Pos();
  p->set_x(l.position_(0));
  p->set_y(l.position_(1));
  p->set_z(l.position_(2));
  new_light->set_allocated_position(p);
  new_light->set_intensity(l.intensity_);
}

void SceneManager::Sphere2ProtoMessage(tracerr::Scene::Sphere *new_sphere, Shape *shp) {
  auto s = dynamic_cast<Sphere *>(shp);
  new_sphere->set_radius(s->GetRadius());
  if (s->custom_material) {
    auto custom_mat = GetCustomMaterialToProto(s->GetMaterial());
    new_sphere->set_custom_material_enabled(true);
    new_sphere->set_allocated_custom_material(custom_mat);
  } else {
    new_sphere->set_material(GetMaterialEnum(s->GetMaterial()));
  }
  auto *p = new tracerr::Scene::Pos();
  p->set_x(s->GetPos()(0));
  p->set_y(s->GetPos()(1));
  p->set_z(s->GetPos()(2));
  new_sphere->set_allocated_position(p);
}

void SceneManager::Triangle2ProtoMessage(tracerr::Scene::Triangle *new_triangle, Shape *shp) {
  auto s = dynamic_cast<Triangle *>(shp);
  if (s->custom_material) {
    auto custom_mat = GetCustomMaterialToProto(s->GetMaterial());
    new_triangle->set_custom_material_enabled(true);
    new_triangle->set_allocated_custom_material(custom_mat);
  } else {
    new_triangle->set_material(GetMaterialEnum(s->GetMaterial()));
  }
  auto *p0 = new tracerr::Scene::Pos();
  auto *p1 = new tracerr::Scene::Pos();
  auto *p2 = new tracerr::Scene::Pos();
  p0->set_x(s->GetP0_()(0));
  p0->set_y(s->GetP0_()(1));
  p0->set_z(s->GetP0_()(2));
  new_triangle->set_allocated_p0(p0);
  p1->set_x(s->GetP1_()(0));
  p1->set_y(s->GetP1_()(1));
  p1->set_z(s->GetP1_()(2));
  new_triangle->set_allocated_p1(p1);
  p2->set_x(s->GetP2_()(0));
  p2->set_y(s->GetP2_()(1));
  p2->set_z(s->GetP2_()(2));
  new_triangle->set_allocated_p2(p2);
}

void SceneManager::Rectangle2ProtoMessage(tracerr::Scene::Rectangle *new_rectangle, Shape *shp) {
  auto s = dynamic_cast<Rectangle *>(shp);
  if (s->custom_material) {
    auto custom_mat = GetCustomMaterialToProto(s->GetMaterial());
    new_rectangle->set_custom_material_enabled(true);
    new_rectangle->set_allocated_custom_material(custom_mat);
  } else {
    new_rectangle->set_material(GetMaterialEnum(s->GetMaterial()));
  }
  auto *p0 = new tracerr::Scene::Pos();
  auto *p1 = new tracerr::Scene::Pos();
  auto *p2 = new tracerr::Scene::Pos();
  auto *p3 = new tracerr::Scene::Pos();
  p0->set_x(s->GetP0_()(0));
  p0->set_y(s->GetP0_()(1));
  p0->set_z(s->GetP0_()(2));
  new_rectangle->set_allocated_p0(p0);
  p1->set_x(s->GetP1_()(0));
  p1->set_y(s->GetP1_()(1));
  p1->set_z(s->GetP1_()(2));
  new_rectangle->set_allocated_p1(p1);
  p2->set_x(s->GetP2_()(0));
  p2->set_y(s->GetP2_()(1));
  p2->set_z(s->GetP2_()(2));
  new_rectangle->set_allocated_p2(p2);
  p3->set_x(s->GetP3_()(0));
  p3->set_y(s->GetP3_()(1));
  p3->set_z(s->GetP3_()(2));
  new_rectangle->set_allocated_p3(p3);
}

tracerr::Scene::MaterialType SceneManager::GetMaterialEnum(const Material &mat) {
  auto ret = tracerr::Scene::MaterialType::Scene_MaterialType_ivory;
  if (mat == Materials::ivory) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_ivory;
  } else if (mat == Materials::red_rubber) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_red_rubber;
  } else if (mat == Materials::green_rubber) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_green_rubber;
  } else if (mat == Materials::blue_rubber) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_blue_rubber;
  } else if (mat == Materials::orange_rubber) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_orange_rubber;
  } else if (mat == Materials::pink_rubber) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_pink_rubber;
  } else if (mat == Materials::red_plastic) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_red_plastic;
  } else if (mat == Materials::black_plastic) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_black_plastic;
  } else if (mat == Materials::mirror) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_mirror;
  } else if (mat == Materials::glass) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_glass;
  } else if (mat == Materials::chessboard) {
    ret = tracerr::Scene::MaterialType::Scene_MaterialType_chessboard;
  }
  return ret;
}

tracerr::Scene::Material *SceneManager::GetCustomMaterialToProto(Material shape_mat) {
  auto *m = new tracerr::Scene::Material();
  auto *alb = new tracerr::Scene::Material::Albedo();
  auto *color = new tracerr::Scene::Material::Color_diff();
  alb->set_a0(shape_mat.albedo_(0));
  alb->set_a1(shape_mat.albedo_(1));
  alb->set_a2(shape_mat.albedo_(2));
  alb->set_a3(shape_mat.albedo_(3));
  color->set_r(shape_mat.color_f_(0) * 255.f);
  color->set_g(shape_mat.color_f_(1) * 255.f);
  color->set_b(shape_mat.color_f_(2) * 255.f);
  m->set_allocated_albedo(alb);
  m->set_allocated_color(color);
  m->set_refractive_index(shape_mat.refractive_index);
  m->set_specular_comp(shape_mat.specular_comp_);

  return m;
};

void SceneManager::GenerateRandomSpheres() {
  srand(time(NULL));
  int num_spheres{0};
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
    ns.custom_material = true;
    shapes_.push_back(std::make_unique<Sphere>(std::move(ns)));
  }
}
