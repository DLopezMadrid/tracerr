#include "Image.h"
#include <eigen3/Eigen/Core>
#include <iostream>
#include <memory>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "../include/stb_image_write.h"


int main() {
  int size_x{800};
  int size_y{600};
  int channels{3};


  Image new_image(size_x, size_y);

  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{125};
  for (float col = 0; col < new_image.pixels_->cols(); col++) {
    green = floor(255 * (col / new_image.pixels_->cols()));
    for (float row = 0; row < new_image.pixels_->rows(); row += channels) {
      red = floor(255 * (row / new_image.pixels_->rows()));
      (*new_image.pixels_)(row, col) = red;
      (*new_image.pixels_)(row + 1, col) = green;
      (*new_image.pixels_)(row + 2, col) = blue;
    }
  }

  new_image.SaveImage("Test13.png");


  return 0;
}
