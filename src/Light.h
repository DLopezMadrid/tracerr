//
// Created by dani on 23/04/2020.
//

#ifndef TRACERR_LIGHT_H
#define TRACERR_LIGHT_H

#include "Shape.h"

struct Light {
  Light(const xyz &p, const float &i) : position(p), intensity(i) {}
  xyz position;
  float intensity;
};


#endif//TRACERR_LIGHT_H
