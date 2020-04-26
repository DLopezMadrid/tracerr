//
// Created by dani on 26/04/2020.
//

#ifndef TRACERR_OBJLOADER_H
#define TRACERR_OBJLOADER_H

#include "Triangle.h"
#include <eigen3/Eigen/Core>
#include <fstream>

class ObjLoader {
  void calcTriangles(Eigen::Vector3f const &translation, Material const &mat);
  std::vector<Eigen::Vector3f> vertexes_;
  std::vector<Eigen::Vector3i> faces_;

  public:
  std::vector<Triangle> triangles_;
  void readFile(const char *file_name, Eigen::Vector3f const &translation, Material const &mat);
};


#endif//TRACERR_OBJLOADER_H
