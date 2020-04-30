//
// Created by dani on 30/04/2020.
//

#include "SceneManager.h"
SceneManager::SceneManager() {
  proto = std::make_unique<tracerr::Scene>();
}

void SceneManager::LoadLights() {
  for (int i{0}; i < scene_.light_size(); i++) {
    xyz pos{scene_.light(i).p().x(), scene_.light(i).p().y(), scene_.light(i).p().z()};
    lights_.emplace_back(pos, scene_.light(i).intensity());
  }
}

void SceneManager::LoadSpheres() {
  for (int i{0}; i < scene_.sphere_size(); i++) {
    xyz pos{scene_.sphere(i).p().x(), scene_.sphere(i).p().y(), scene_.sphere(i).p().z()};
    Material mat = LoadMaterial(scene_.sphere(i).material());
    shapes_.push_back(std::make_unique<Sphere>(pos, scene_.sphere(i).radius(), mat));
  }
}

void SceneManager::LoadTriangles() {
  for (int i{0}; i < scene_.triangle_size(); i++) {
    xyz p0{scene_.triangle(i).p0().x(), scene_.triangle(i).p0().y(), scene_.triangle(i).p0().z()};
    xyz p1{scene_.triangle(i).p1().x(), scene_.triangle(i).p1().y(), scene_.triangle(i).p1().z()};
    xyz p2{scene_.triangle(i).p2().x(), scene_.triangle(i).p2().y(), scene_.triangle(i).p2().z()};
    Material mat = LoadMaterial(scene_.triangle(i).material());
    shapes_.push_back(std::make_unique<Triangle>(p0, p1, p2, mat));
  }
}

void SceneManager::LoadRectangles() {
  for (int i{0}; i < scene_.rectangle_size(); i++) {
    xyz p0{scene_.rectangle(i).p0().x(), scene_.rectangle(i).p0().y(), scene_.rectangle(i).p0().z()};
    xyz p1{scene_.rectangle(i).p1().x(), scene_.rectangle(i).p1().y(), scene_.rectangle(i).p1().z()};
    xyz p2{scene_.rectangle(i).p2().x(), scene_.rectangle(i).p2().y(), scene_.rectangle(i).p2().z()};
    xyz p3{scene_.rectangle(i).p3().x(), scene_.rectangle(i).p3().y(), scene_.rectangle(i).p3().z()};
    Material mat = LoadMaterial(scene_.rectangle(i).material());
    shapes_.push_back(std::make_unique<Rectangle>(p0, p1, p2, p3, mat));
  }
}

void SceneManager::LoadObjs() {
  ObjLoader objLoader;
  for (int i{0}; i < scene_.obj_size(); i++) {
    std::string obj_fname = scene_.obj(i).fname();
    xyz translation{scene_.obj(i).translation().x(), scene_.obj(i).translation().y(), scene_.obj(i).translation().z()};
    Material mat = LoadMaterial(scene_.obj(i).material());
    objLoader.readFile2(obj_fname.c_str(), translation, mat);

    for (int i{0}; i < objLoader.triangles_.size(); i++) {
      shapes_.push_back(std::make_unique<Triangle>(objLoader.triangles_[i]));
    }
    for (int i{0}; i < objLoader.rectangles_.size(); i++) {
      shapes_.push_back(std::make_unique<Rectangle>(objLoader.rectangles_[i]));
    }
  }
}


bool SceneManager::LoadTextProto(std::string proto_path) {
  std::ifstream ifs(proto_path);
  google::protobuf::io::IstreamInputStream iis(&ifs);
  auto success = google::protobuf::TextFormat::Parse(&iis, proto.get());
  if (!success) {
    std::cerr << "Failed to parse scene" << std::endl;
    return -1;
  }
  return true;
}

void SceneManager::LoadScene(char *proto_path_char) {
  std::string proto_path = std::string(proto_path_char);
  auto start_t{std::chrono::high_resolution_clock::now()};

  std::cout << "Loading proto file" << '\n';
  LoadTextProto(proto_path);
  scene_ = *((tracerr::Scene *) proto.get());
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

  Render r2(scene_.width(), scene_.height(), {0, 0, 0}, lights_);
  std::cout << "Rendering" << '\n';
  r2.RenderSceneMultiThread(std::move(shapes_));
  if (scene_.savefile()) {
    std::cout << "Saving file as " << scene_.fname() << '\n';
    r2.SaveImage(scene_.fname());
  }
  std::cout << "Done" << '\n';

  auto end_t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_t);

  if (scene_.show_elapsed_time()) {
    std::cout << "Completed " << scene_.width() << "x" << scene_.height() << " image in " << end_t.count() << " ms" << std::endl;
  }
}


Material SceneManager::LoadMaterial(std::string m) {
  Material mat{Materials::red_rubber};
  if (m == "Materials::red_rubber") {
    mat = Materials::red_rubber;
  } else if (m == "Materials::blue_rubber") {
    mat = Materials::blue_rubber;
  } else if (m == "Materials::green_rubber") {
    mat = Materials::green_rubber;
  } else if (m == "Materials::orange_rubber") {
    mat = Materials::orange_rubber;
  } else if (m == "Materials::pink_rubber") {
    mat = Materials::pink_rubber;
  } else if (m == "Materials::ivory") {
    mat = Materials::ivory;
  } else if (m == "Materials::red_plastic") {
    mat = Materials::red_plastic;
  } else if (m == "Materials::black_plastic") {
    mat = Materials::black_plastic;
  } else if (m == "Materials::glass") {
    mat = Materials::glass;
  } else if (m == "Materials::mirror") {
    mat = Materials::mirror;
  }
  return mat;
}