//
// Created by dani on 21/04/2020.
//

#include "Image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"


// Need to use a friend function due to the stb library interface
// it requires to pass a void fun (void*, void*, int) function pointer
// Member classes will not satisfy that as they are void Image::fun (void*, void*, int)
void fwrite_func(void *context, void *data, int size) {
  File f(((std::string *) (context))->c_str());
  f.Write(data, size);
}


//Image::Image(int x, int y) : size_x_{x}, size_y_{y}, pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>()} {
Image::Image(int x, int y) : size_x_{x}, size_y_{y}, pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>>()} {
  Resize(x, y);
}

void Image::Resize(int x, int y) {
  try {
    if (x < 0 || y < 0) {
      throw std::logic_error("failed to parse the xml file");
    };
  } catch (...) {
  }
  size_x_ = x;
  size_y_ = y;
  //  pixels_->resize(size_x_, (size_y_*channels_));
  pixels_->resize(size_x_ * channels_, (size_y_));
  pixels_->setZero();
}
//Image::Image() : size_x_{0}, size_y_{0}, pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>()} {
Image::Image() : size_x_{0}, size_y_{0}, pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>>()} {
  //  pixels_->resize(size_x_, (size_y_*channels_));
  pixels_->resize(size_x_ * channels_, (size_y_));
  pixels_->setZero();
}

void Image::SaveImage(std::string fname) const {
  void (*fun)(void *, void *, int);
  fun = &fwrite_func;
  std::string *fp = &fname;
  stbi_write_png_to_func(fun, fp, size_x_, size_y_, channels_, pixels_->data(), size_x_ * channels_);
}
