//
// Created by dani on 30/04/2020.
//

#ifndef TRACERR_SCENEMANAGER_H
#define TRACERR_SCENEMANAGER_H

#include "proto/scene.proto"
#include "ObjLoader.h"
#include "Rectangle.h"
#include "Render.h"
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <eigen3/Eigen/Geometry>
#include <fstream>

// The scene manager is in charge of reading the scene from the protobuf text file, parse it, generate the required shapes
// set up the render environment and call the render class
// It can also write to text protobufs but this is not a user facing feature yet. It was implemented for learning and debugging purposes only
// It is a friend class of Render, so it can access all the private members on it
class SceneManager {
  public:
  SceneManager();
  void LoadScene(char *proto_path_char);
  void SaveExampleScene(char *proto_path_char);
  void WriteTemplateFile(char *template_fname);
  void WriteCleanTemplateFile(char *template_fname);

  private:
  bool LoadTextProto();
  void LoadSpheres();
  void LoadLights();
  void LoadTriangles();
  void LoadRectangles();
  void LoadObjs();
  void LoadSceneConfig();
  Material LoadMaterial(int m);
  bool WriteProtoText(const std::string &save_file_path);
  void SaveSceneToProto(std::string save_textproto_file_path);
  void Sphere2ProtoMessage(tracerr::Scene_Sphere *new_sphere, Shape *shp);
  void Rectangle2ProtoMessage(tracerr::Scene_Rectangle *new_rectangle, Shape *shp);
  void Triangle2ProtoMessage(tracerr::Scene_Triangle *new_triangle, Shape *shp);
  void Light2ProtoMessage(tracerr::Scene_Light *new_light, Light l);
  void AddCommentedObjToProto(std::string example_proto_path_);
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

};


#endif//TRACERR_SCENEMANAGER_H
