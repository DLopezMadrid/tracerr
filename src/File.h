//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_FILE_H
#define TRACERR_FILE_H

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>


class File {
  public:
  File(std::string fname);
  void Write(void *data, int size);
  ~File();

  protected:
  FILE *file_;
};


#endif//TRACERR_FILE_H
