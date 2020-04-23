#include "Image.h"
#include <eigen3/Eigen/Core>
#include <iostream>
#include <memory>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "../include/stb_image_write.h"


int main() {
  int img_width{800};
  int img_height{600};
  int channels{3};


  Image new_image(img_width, img_height);

  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{0};
  for (float row = 0; row < new_image.pixels_->rows(); row++) {
    for (float col = 0; col < new_image.pixels_->cols(); col += channels) {
      red = floor(255 * (col / new_image.pixels_->cols()));
      (*new_image.pixels_)(row, col) = red;
      (*new_image.pixels_)(row, col + 1) = 0;
      (*new_image.pixels_)(row, col + 2) = 0;
    }
  }

  new_image.SaveImage("Test13.png");


  return 0;
}
