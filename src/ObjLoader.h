//
// Created by dani on 26/04/2020.
//

#ifndef TRACERR_OBJLOADER_H
#define TRACERR_OBJLOADER_H

#include "Triangle.h"
#include <eigen3/Eigen/Core>
#include <fstream>

//RAII for reading the .obj file
class ObjFile {
  public:
  ObjFile(const std::string &fname) {
    try {
      in_.open(fname, std::ifstream::in);
    } catch (...) {
      throw std::logic_error("Failed to open the .obj file");
    }
  }

  std::string GetLine() {
    std::string line;
    if (!in_.eof()) { std::getline(in_, line); }
    return line;
  }

  bool EndOfFile() {
    return in_.eof();
  }

  ~ObjFile() {
    if (in_.is_open()) { in_.close(); }
  }

  private:
  std::ifstream in_;
};

// Parses the .obj file and creates the respective triangles for the render (including material and an optional translation)
// From https://stackoverflow.com/questions/23319490/loading-obj-file-using-c-and-opengl
class ObjLoader {
  void calcTriangles(Eigen::Vector3f const &translation, Material const &mat);
  std::vector<Eigen::Vector3f> vertexes_;
  std::vector<Eigen::Vector3i> faces_;

  public:
  std::vector<Triangle> triangles_;
  void readFile(const char *file_name, Eigen::Vector3f const &translation, Material const &mat);
};


#endif//TRACERR_OBJLOADER_H
