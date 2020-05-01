//
// Created by dani on 30/04/2020.
//

#ifndef TRACERR_SCENEMANAGER_H
#define TRACERR_SCENEMANAGER_H

#include "../proto/scene.pb.h"
#include "ObjLoader.h"
#include "Rectangle.h"
#include "Render.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
#include <eigen3/Eigen/Geometry>
#include <fstream>

class SceneManager {
  public:
  SceneManager();
  void LoadScene(char *proto_path_char);

  private:
  bool LoadTextProto();
  void LoadSpheres();
  void LoadLights();
  void LoadTriangles();
  void LoadRectangles();
  void LoadObjs();
  Material LoadMaterial(int m);
  bool WriteProtoText(const std::string &save_file_path);
  void SaveSceneToProto();
  void Sphere2ProtoMessage(tracerr::Scene_Sphere *new_sphere, Shape *shp);
  void Rectangle2ProtoMessage(tracerr::Scene_Rectangle *new_rectangle, Shape *shp);
  void Triangle2ProtoMessage(tracerr::Scene_Triangle *new_triangle, Shape *shp);
  void Light2ProtoMessage(tracerr::Scene_Light *new_light, Light l);
  void GenerateRandomSpheres();
  tracerr::Scene::Material *GetCustomMaterialToProto(Material shape_mat);
  tracerr::Scene::MaterialType GetMaterialEnum(const Material &mat);
  Material GetCustomMaterialFromProto(const tracerr::Scene_Material &proto_mat);
  std::unique_ptr<google::protobuf::Message> proto;
  tracerr::Scene scene_in_;
  tracerr::Scene scene_out_;
  std::vector<Light> lights_;
  std::vector<std::unique_ptr<Shape>> shapes_;
  std::string proto_path_;
  int width_{800};
  int height_{400};
  bool show_chessboard_{false};
  bool show_elapsed_time_{true};
  rgb_f background_color_;
  void LoadSceneConfig();
};


#endif//TRACERR_SCENEMANAGER_H
