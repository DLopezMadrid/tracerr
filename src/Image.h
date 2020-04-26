//
// Created by dani on 21/04/2020.
//

#ifndef TRACERR_IMAGE_H
#define TRACERR_IMAGE_H
#include "ImageFile.h"
//#include "Shape.h"
#include <cassert>
#include <eigen3/Eigen/Core>
#include <memory>

typedef std::array<int, 2> PixPos;
typedef std::array<uint8_t, 3> rgb;

// Holds & owns the image bitmap (pixels_) in the heap
// It is in charge of writing the color values to every pixel and to save the image
class Image {
  public:
  Image(int const &x = 0, int const &y = 0);
  void Resize(int const &x, int const &y);
  int GetImageWidth() const { return pix_width_; };
  int GetImageHeight() const { return pix_height_; };
  int GetImageChannels() const { return pix_channels_; };
  void SetPixelColor(PixPos const &pixel, rgb const &color);
  rgb GetPixelColor(PixPos const &pixel);
  void DrawGradientBackground();
  std::unique_ptr<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> pixels_;

  void SaveImage(std::string fname) const;

  //TODO does it really need to be a friend function?
  //  friend void fwrite_func(void *context, void *data, int size);

  private:
  int pix_height_;
  int pix_width_;
  int pix_channels_{3};
};


#endif//TRACERR_IMAGE_H
