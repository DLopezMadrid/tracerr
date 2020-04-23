//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_IMAGE_H
#define TRACERR_IMAGE_H
#include "File.h"
#include <cassert>
#include <eigen3/Eigen/Core>
#include <memory>


class Image {
  public:
  Image(int x, int y);
  Image();
  void Resize(int x, int y);
  int GetImageWidth() const { return pix_width_; };
  int GetImageHeight() const { return pix_height_; };
  int GetImageChannels() const { return pix_channels_; };
  std::unique_ptr<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> pixels_;
  //    std::unique_ptr<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>> pixels_;

  void SaveImage(std::string fname) const;

  friend void fwrite_func(void *context, void *data, int size);

  private:
  int pix_height_;
  int pix_width_;
  int pix_channels_{3};
};


#endif//TRACERR_IMAGE_H
