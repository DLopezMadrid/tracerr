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
  bool LoadTextProto(std::string proto_path);
  std::unique_ptr<google::protobuf::Message> proto;
  tracerr::Scene scene_;
  std::vector<Light> lights_;
  std::vector<std::unique_ptr<Shape>> shapes_;

  void LoadSpheres();
  void LoadLights();
  void LoadTriangles();
  void LoadRectangles();
  Material LoadMaterial(std::string m);
  void LoadObjs();
  void LoadScene(char *proto_path_char);
};


#endif//TRACERR_SCENEMANAGER_H
