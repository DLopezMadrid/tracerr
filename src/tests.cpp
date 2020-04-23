#include "Image.h"
#include "Shape.h"
#include <gtest/gtest.h>

TEST(Image, ImageBasicCtor) {
  Image img;
  ASSERT_EQ(0, img.GetImageWidth());
  ASSERT_EQ(0, img.GetImageHeight());
  ASSERT_EQ(0, img.pixels_->rows());
  ASSERT_EQ(0, img.pixels_->cols());
  ASSERT_EQ(0, img.pixels_->size());
}

TEST(Image, ImageCtor) {
  int img_width = 100;
  int img_height = 200;
  int ch = 3;
  Image img(img_width, img_height);
  ASSERT_EQ(ch, img.GetImageChannels());
  ASSERT_EQ(img_width, img.GetImageWidth());
  ASSERT_EQ(img_height, img.GetImageHeight());
  ASSERT_EQ(img_width * 3, img.pixels_->cols());
  ASSERT_EQ(img_height, img.pixels_->rows());
  ASSERT_EQ(img_width * img_height * 3, img.pixels_->size());
}

TEST(Image, ImageResize) {
  int img_width = 100;
  int img_height = 200;
  Image img(img_width, img_height);
  img_width = img_width * 3;
  img_height = img_height / 2;
  img.Resize(img_width, img_height);

  ASSERT_EQ(img_width, img.GetImageWidth());
  ASSERT_EQ(img_height, img.GetImageHeight());
  ASSERT_EQ(img_width * 3, img.pixels_->cols());
  ASSERT_EQ(img_height, img.pixels_->rows());
  ASSERT_EQ(img_width * img_height * 3, img.pixels_->size());
}

TEST(Image, DefaultColor) {

  int img_width{300};
  int img_height{200};
  Image new_image(img_width, img_height);
  int channels{new_image.GetImageChannels()};

  unsigned int p0R = (*new_image.pixels_)(0, 0);
  unsigned int p0G = (*new_image.pixels_)(0, 1);
  unsigned int p0B = (*new_image.pixels_)(0, 2);

  ASSERT_EQ(0, p0R);
  ASSERT_EQ(0, p0G);
  ASSERT_EQ(0, p0B);

  srand(time(NULL));

  int row = rand() % (img_height);
  int col = rand() % (img_width * channels);

  unsigned int pXR = (*new_image.pixels_)(row, col);
  unsigned int pXG = (*new_image.pixels_)(row, col + 1);
  unsigned int pXB = (*new_image.pixels_)(row, col + 2);

  ASSERT_EQ(0, pXR);
  ASSERT_EQ(0, pXG);
  ASSERT_EQ(0, pXB);
}


TEST(Image, ColorGradient) {
  int img_width{800};
  int img_height{600};

  Image new_image(img_width, img_height);

  int channels{new_image.GetImageChannels()};
  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{0};
  for (float row = 0; row < new_image.pixels_->rows(); row++) {
    green = floor(255 * (row / new_image.pixels_->rows()));
    for (float col = 0; col < new_image.pixels_->cols(); col += channels) {
      if (col / 3 == row) {
        blue = floor(255 * (row / new_image.pixels_->rows()));
      }
      red = floor(255 * (col / new_image.pixels_->cols()));
      (*new_image.pixels_)(row, col) = red;
      (*new_image.pixels_)(row, col + 1) = green;
      (*new_image.pixels_)(row, col + 2) = blue;
    }
  }

  unsigned int p0R = (*new_image.pixels_)(10, 3);
  unsigned int p0G = (*new_image.pixels_)(10, 4);
  unsigned int p0B = (*new_image.pixels_)(10, 5);

  srand(time(NULL));

  int col = (img_width * channels / 2) + rand() % (img_width * channels / 2);
  int row = (img_height / 2) + rand() % (img_height / 2);

  unsigned int pXR = (*new_image.pixels_)(row, col);
  unsigned int pXG = (*new_image.pixels_)(row, col + 1);
  unsigned int pXB = (*new_image.pixels_)(row, col + 2);

  ASSERT_TRUE(row != 10);
  ASSERT_TRUE(col != 10);
  ASSERT_TRUE(p0R != pXR);
  ASSERT_TRUE(p0G != pXG);
  ASSERT_TRUE(p0B != pXB);
}

TEST(Image, SaveImage) {
  int img_width{800};
  int img_height{600};

  Image new_image(img_width, img_height);

  int channels{new_image.GetImageChannels()};
  unsigned int red{0};
  unsigned int green{0};
  unsigned int blue{0};

  for (float row = 0; row < new_image.pixels_->rows(); row++) {
    green = floor(255 * (row / new_image.pixels_->rows()));
    for (float col = 0; col < new_image.pixels_->cols(); col += channels) {
      if (col / 3 == row) {
        blue = floor(255 * (row / new_image.pixels_->rows()));
      }
      red = floor(255 * (col / new_image.pixels_->cols()));
      (*new_image.pixels_)(row, col) = red;
      (*new_image.pixels_)(row, col + 1) = green;
      (*new_image.pixels_)(row, col + 2) = blue;
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
