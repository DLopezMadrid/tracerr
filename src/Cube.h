//
// Created by dani on 22/04/2020.
//

#ifndef TRACERR_CUBE_H
#define TRACERR_CUBE_H
#include "Shape.h"

class Cube : public Shape {
  public:
  bool rayIntersect(int pix_x, int pix_y);


  private:
  double side_len_;
  float rot_roll_;
  float rot_pitch_;
  float rot_yaw_;
};


#endif//TRACERR_CUBE_H
