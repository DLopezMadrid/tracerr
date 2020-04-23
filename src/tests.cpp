#include "Image.h"
#include "Shape.h"
#include <gtest/gtest.h>

TEST(Image, ImageBasicCtor) {
  Image r;
  ASSERT_EQ(0, r.GetSizeX());
  ASSERT_EQ(0, r.GetSizeY());
  ASSERT_EQ(0, (r.pixels_)->rows());
  ASSERT_EQ(0, (r.pixels_)->cols());
  ASSERT_EQ(0, (r.pixels_)->size());
}

TEST(Image, ImageCtor) {
  int x = 100;
  int y = 200;
  int ch = 3;
  Image img(x, y);
  ASSERT_EQ(ch, img.GetChannels());
  ASSERT_EQ(x, img.GetSizeX());
  ASSERT_EQ(y, img.GetSizeY());
  ASSERT_EQ(x * 3, (img.pixels_)->rows());
  ASSERT_EQ(y, (img.pixels_)->cols());
  ASSERT_EQ(x * y * 3, (img.pixels_)->size());
}

TEST(Image, ImageResize) {
  int x = 100;
  int y = 200;
  Image img(x, y);
  x = x * 3;
  y = y / 2;
  img.Resize(x, y);

  ASSERT_EQ(x, img.GetSizeX());
  ASSERT_EQ(y, img.GetSizeY());
  ASSERT_EQ(x * img.GetChannels(), (img.pixels_)->rows());
  ASSERT_EQ(y, (img.pixels_)->cols());
  ASSERT_EQ(x * y * img.GetChannels(), (img.pixels_)->size());
}

TEST(Image, DefaultColor) {

  int size_x{300};
  int size_y{200};
  Image new_image(size_x, size_y);
  int channels{new_image.GetChannels()};

  unsigned int p0R = (*new_image.pixels_)(0, 0);
  unsigned int p0G = (*new_image.pixels_)(1, 0);
  unsigned int p0B = (*new_image.pixels_)(2, 0);

  ASSERT_EQ(0, p0R);
  ASSERT_EQ(0, p0G);
  ASSERT_EQ(0, p0B);

  srand(time(NULL));

  int row = rand() % (size_x * channels);
  int col = rand() % (size_y);

  unsigned int pXR = (*new_image.pixels_)(row, col);
  unsigned int pXG = (*new_image.pixels_)(row + 1, col);
  unsigned int pXB = (*new_image.pixels_)(row + 2, col);

  ASSERT_EQ(0, pXR);
  ASSERT_EQ(0, pXG);
  ASSERT_EQ(0, pXB);
}


TEST(Image, ColorGradient) {
  int size_x{800};
  int size_y{600};

  Image new_image(size_x, size_y);

  int channels{new_image.GetChannels()};
  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{0};
  for (float col = 0; col < new_image.pixels_->cols(); col++) {
    green = floor(255 * (col / new_image.pixels_->cols()));
    for (float row = 0; row < new_image.pixels_->rows(); row += channels) {
      if (row / 3 == col) {
        blue = floor(255 * (col / new_image.pixels_->cols()));
      }

      red = floor(255 * (row / new_image.pixels_->rows()));
      (*new_image.pixels_)(row, col) = red;
      (*new_image.pixels_)(row + 1, col) = green;
      (*new_image.pixels_)(row + 2, col) = blue;
    }
  }

  unsigned int p0R = (*new_image.pixels_)(0, 10);
  unsigned int p0G = (*new_image.pixels_)(1, 10);
  unsigned int p0B = (*new_image.pixels_)(2, 10);

  srand(time(NULL));

  int row = (size_x * channels / 2) + rand() % (size_x * channels / 2);
  int col = (size_y / 2) + rand() % (size_y / 2);

  unsigned int pXR = (*new_image.pixels_)(row, col);
  unsigned int pXG = (*new_image.pixels_)(row + 1, col);
  unsigned int pXB = (*new_image.pixels_)(row + 2, col);

  ASSERT_TRUE(row != 10);
  ASSERT_TRUE(col != 10);
  ASSERT_TRUE(p0R != pXR);
  ASSERT_TRUE(p0G != pXG);
  ASSERT_TRUE(p0B != pXB);
}

TEST(Image, SaveImage) {
  int size_x{800};
  int size_y{600};

  Image new_image(size_x, size_y);

  int channels{new_image.GetChannels()};
  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{0};
  //  for (float col = 0; col < new_image.pixels_->cols(); col++) {
  //    green = floor(255 * (col / new_image.pixels_->cols()));
  //    for (float row = 0; row < new_image.pixels_->rows(); row += channels) {
  //      if(row/3==col){
  //        blue = floor(255 * (col / new_image.pixels_->cols()));
  //      }
  //
  //      red = floor(255 * (row / new_image.pixels_->rows()));
  //      (*new_image.pixels_)(row, col) = red;
  //      (*new_image.pixels_)(row + 1, col) = green;
  //      (*new_image.pixels_)(row + 2, col) = blue;
  //    }
  //  }

  for (float col = 0; col < new_image.pixels_->cols(); col++) {
    //    green = floor(255 * (col / new_image.pixels_->cols()));
    for (float row = 0; row < new_image.pixels_->rows(); row += channels) {
      //      if(row/3==col){
      //        blue = floor(255 * (col / new_image.pixels_->cols()));
      //      }

      red = floor(255 * (row / new_image.pixels_->rows()));
      (*new_image.pixels_)(row, col) = red;
      (*new_image.pixels_)(row + 1, col) = 0;
      (*new_image.pixels_)(row + 2, col) = 0;
    }
  }

  std::string fname{"TestSuiteTestImageGradient.png"};

  std::ifstream infile(fname);
  if (infile.good()) {
    remove(fname.c_str());
  }
  infile.close();

  std::ifstream infile2(fname);
  ASSERT_FALSE(infile2.good());
  infile2.close();

  new_image.SaveImage(fname);
  std::ifstream infile3(fname);
  ASSERT_TRUE(infile3.good());
  infile3.close();
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
