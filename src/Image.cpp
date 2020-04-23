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


Image::Image(int x, int y) : pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>()} {
  //Image::Image(int x, int y) : pix_width_{x}, pix_height_{y}, pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>>()} {
  Resize(x, y);
}

Image::Image() : pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>()} {
  //Image::Image() : pix_width_{0}, pix_height_{0}, pixels_{std::make_unique<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>>()} {
  //  pixels_->resize(pix_height_, (pix_width_ * pix_channels_));
  //  pixels_->resize(pix_width_ * pix_channels_, (pix_height_));
  //  pixels_->setZero();
  Resize(0, 0);
}

void Image::Resize(int x, int y) {
  try {
    if (x < 0 || y < 0) {
      throw std::logic_error("not valid values for Resize(x,y)");
    };
  } catch (...) {
  }
  pix_width_ = x;
  pix_height_ = y;
  pixels_->resize(pix_height_, (pix_width_ * pix_channels_));
  //  pixels_->resize(pix_width_ * pix_channels_, (pix_height_));
  pixels_->setZero();
}


void Image::SaveImage(std::string fname) const {
  void (*fun)(void *, void *, int);
  fun = &fwrite_func;
  std::string *fp = &fname;
  stbi_write_png_to_func(fun, fp, pix_width_, pix_height_, pix_channels_, pixels_->data(), pix_width_ * pix_channels_);
}
