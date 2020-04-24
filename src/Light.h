//
// Created by dani on 23/04/2020.
//

#ifndef TRACERR_LIGHT_H
#define TRACERR_LIGHT_H

#include "Shape.h"

struct Light {
  Light(xyz const &p, float const &i) : position_{p}, intensity_{i} {}
  xyz position_;
  float intensity_;
};


#endif//TRACERR_LIGHT_H
