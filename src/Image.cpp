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
//TODO add test for SetPixelColor
void Image::SetPixelColor(PixPos pixel, rgb color) {
  (*pixels_)(pixel[1], pixel[0] * 3) = color[0];
  (*pixels_)(pixel[1], pixel[0] * 3 + 1) = color[1];
  (*pixels_)(pixel[1], pixel[0] * 3 + 2) = color[2];
}
//TODO add test for GetPixelColor
rgb Image::GetPixelColor(PixPos pixel) {
  return rgb({(*pixels_)(pixel[1], pixel[0] * 3), (*pixels_)(pixel[1], pixel[0] * 3 + 1), (*pixels_)(pixel[1], pixel[0] * 3 + 2)});
}
void Image::DrawGradientBackground() {
  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{0};

  for (float row = 0; row < pixels_->rows(); row++) {
    green = floor(255 * (row / pixels_->rows()));
    for (float col = 0; col < pixels_->cols(); col += pix_channels_) {
      if (col / 3 == row) {
        blue = floor(255 * (row / pixels_->rows()));
      }
      red = floor(255 * (col / pixels_->cols()));
      (*pixels_)(row, col) = red;
      (*pixels_)(row, col + 1) = green;
      (*pixels_)(row, col + 2) = blue;
    }
  }
}
