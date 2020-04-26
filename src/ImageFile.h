//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_IMAGEFILE_H
#define TRACERR_IMAGEFILE_H

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>

// RAII for writing the image to a png file
// Data comes from the STB library
class ImageFile {
  public:
  ImageFile(std::string const &fname) {
    try {
      file_ = fopen(fname.c_str(), "wb");
    } catch (...) {
    }
  }
  void Write(void *data, int const &size) {
    fwrite(data, 1, size, file_);
  }

  ~ImageFile() {
    if (file_ != nullptr) {
      fclose(file_);
    }
  }

  protected:
  FILE *file_;
};


#endif//TRACERR_IMAGEFILE_H
