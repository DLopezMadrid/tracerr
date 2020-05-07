#include "../src/Image.h"
#include "../src/Sphere.h"
#include <gtest/gtest.h>
#include <iostream>

float Material::ambient_light{0.05};


TEST(Image, ImageBasicCtor) {
  Image img;
  EXPECT_EQ(0, img.GetImageWidth());
  EXPECT_EQ(0, img.GetImageHeight());
  EXPECT_EQ(0, img.pixels_->rows());
  EXPECT_EQ(0, img.pixels_->cols());
  EXPECT_EQ(0, img.pixels_->size());
}

TEST(Image, ImageCtor) {
  int img_width = 100;
  int img_height = 200;
  int ch = 3;
  Image img(img_width, img_height);
  EXPECT_EQ(ch, img.GetImageChannels());
  EXPECT_EQ(img_width, img.GetImageWidth());
  EXPECT_EQ(img_height, img.GetImageHeight());
  EXPECT_EQ(img_width * 3, img.pixels_->cols());
  EXPECT_EQ(img_height, img.pixels_->rows());
  EXPECT_EQ(img_width * img_height * 3, img.pixels_->size());
}

TEST(Image, ImageResize) {
  int img_width = 100;
  int img_height = 200;
  Image img(img_width, img_height);
  img_width = img_width * 3;
  img_height = img_height / 2;
  img.Resize(img_width, img_height);

  EXPECT_EQ(img_width, img.GetImageWidth());
  EXPECT_EQ(img_height, img.GetImageHeight());
  EXPECT_EQ(img_width * 3, img.pixels_->cols());
  EXPECT_EQ(img_height, img.pixels_->rows());
  EXPECT_EQ(img_width * img_height * 3, img.pixels_->size());
}

TEST(Image, ImageResizeException){
  int img_width = 100;
  int img_height = 200;
  Image img(img_width, img_height);
  img_width =  -1 * img_width * 3;
  img_height = img_height / 2;
  EXPECT_ANY_THROW(img.Resize(img_width, img_height));
}

TEST(Image, DefaultColor) {

  int img_width{300};
  int img_height{200};
  Image new_image(img_width, img_height);
  int channels{new_image.GetImageChannels()};

  unsigned int p0R = (*new_image.pixels_)(0, 0);
  unsigned int p0G = (*new_image.pixels_)(0, 1);
  unsigned int p0B = (*new_image.pixels_)(0, 2);

  EXPECT_EQ(0, p0R);
  EXPECT_EQ(0, p0G);
  EXPECT_EQ(0, p0B);

  srand(time(NULL));

  int row = rand() % (img_height);
  int col = rand() % (img_width * channels);

  unsigned int pXR = (*new_image.pixels_)(row, col);
  unsigned int pXG = (*new_image.pixels_)(row, col + 1);
  unsigned int pXB = (*new_image.pixels_)(row, col + 2);

  EXPECT_EQ(0, pXR);
  EXPECT_EQ(0, pXG);
  EXPECT_EQ(0, pXB);
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

  EXPECT_TRUE(row != 10);
  EXPECT_TRUE(col != 10);
  EXPECT_TRUE(p0R != pXR);
  EXPECT_TRUE(p0G != pXG);
  EXPECT_TRUE(p0B != pXB);
}

TEST(Image, SaveImage) {

  // saves a png image called "TestSuiteTestImageGradient.png" in the build directory
  // The image should be a gradient that goes from top left (black),
  // top right (red), bottom left (cyan), bottom right (white)

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
  EXPECT_FALSE(infile2.good());
  infile2.close();

  new_image.SaveImage(fname);
  std::ifstream infile3(fname);
  EXPECT_TRUE(infile3.good());
  infile3.close();
}

TEST(Sphere, SphereCtor) {
  xyz pos{10, 15, 20};
  double radius{1.5};
  Material mat = Materials::blue_rubber;
  Sphere sphere(pos, radius, mat);
  EXPECT_EQ(pos, sphere.GetPos());
  EXPECT_EQ(radius, sphere.GetRadius());
  //  EXPECT_EQ(mat, sphere.GetMaterial());
}

TEST(Sphere, SphereCtorNoArgs) {
  xyz pos{0, 0, 0};
  double radius{1};
  Material mat = Materials::red_rubber;
  Sphere sphere(pos, radius, mat);
  EXPECT_EQ(pos, sphere.GetPos());
  EXPECT_EQ(radius, sphere.GetRadius());
  //  EXPECT_EQ(mat, sphere.GetMaterial());
}

TEST(Sphere, RayIntersect) {
  xyz ray{0, 0, -1};
  xyz ray2{1, 1, 0};
  xyz ray3{1, 0, -1};
  xyz ray4{99.0f/1000.0f, 0.0f/1000.0f, -1};
  xyz ray5{101.0f/1000.0f, 0.0f/1000.0f, -1};
  xyz ray6{99.0f/1000.0f, 0.0f/1000.0f, 1};
  xyz ray7{99.0f/1000.0f, 99.0f/1000.0f, -1};
  xyz ray8{-99.0f/1000.0f, 0.0f/1000.0f, -1};
  xyz ray9{0.0f/1000.0f, 99.0f/1000.0f, -1};
  xyz ray10{0.0f/1000.0f, -99.0f/1000.0f, -1};
  xyz ray11{70.0f/1000.0f, 70.0f/1000.0f, -1};
  xyz ray12{-70.0f/1000.0f, 70.0f/1000.0f, -1};
  xyz ray13{70.0f/1000.0f, -70.0f/1000.0f, -1};
  xyz ray14{-70.0f/1000.0f, -70.0f/1000.0f, -1};
  xyz ray15{72.0f/1000.0f, 72.0f/1000.0f, -1};
  ray.normalize();
  ray2.normalize();
  ray3.normalize();
  ray4.normalize();
  ray5.normalize();
  ray6.normalize();
  ray7.normalize();
  ray8.normalize();
  ray9.normalize();
  ray10.normalize();
  ray11.normalize();
  ray12.normalize();
  ray13.normalize();
  ray14.normalize();
  ray15.normalize();

  xyz origin{0, 0, 0};
  xyz pos{0, 0, -10};
  float radius{1};
  float dist{std::numeric_limits<float>::max()};
  Sphere sphere(pos, radius);
  EXPECT_TRUE(sphere.RayIntersect(origin, ray, dist));
  EXPECT_FALSE(sphere.RayIntersect(origin, ray2, dist));
  EXPECT_FALSE(sphere.RayIntersect(origin, ray3, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray4, dist));
  EXPECT_FALSE(sphere.RayIntersect(origin, ray5, dist));
  EXPECT_FALSE(sphere.RayIntersect(origin, ray6, dist));
  EXPECT_FALSE(sphere.RayIntersect(origin, ray7, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray8, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray9, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray10, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray11, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray12, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray13, dist));
  EXPECT_TRUE(sphere.RayIntersect(origin, ray14, dist));
  EXPECT_FALSE(sphere.RayIntersect(origin, ray15, dist));
}




//
//
//TEST(Render, RenderCtor) {
//  int img_height{1000};
//  int img_width{800};
//  Render render(img_width, img_height);
//  xyz origin{1, 3, -5};
//  render.SetOrigin(origin);
//  EXPECT_EQ(img_height, render.image_.GetImageHeight());
//  EXPECT_EQ(img_width, render.image_.GetImageWidth());
//  EXPECT_EQ(origin, render.GetOrigin());
//}
//
//TEST(Render, RenderCtorNoArgs) {
//  int img_width{800};
//  int img_height{600};
//  xyz origin{-1, 0, 0};
//  Render render;
//  EXPECT_EQ(img_height, render.image_.GetImageHeight());
//  EXPECT_EQ(img_width, render.image_.GetImageWidth());
//  EXPECT_EQ(origin, render.GetOrigin());
//}

TEST(Ray, rays) {
  xyz x{1, 2, 3};
  xyz y{1, 2, 3};

  EXPECT_EQ(x, y);
}

TEST(Color, colors) {
  rgb c1{1, 2, 3};
  rgb c2{1, 2, 3};
  rgb c3{1, 3, 3};

  EXPECT_EQ(c1, c2);
  EXPECT_NE(c1, c3);
}

//
//TEST(Render, RenderCastRay) {
//  int img_height{800};
//  int img_width{600};
//  xyz origin{-1, 0, 0};
//  Render render(img_width, img_height, origin);
//
//  EXPECT_EQ(origin, render.GetOrigin());
//  xyz ray_x{1, 0, 0};
//  xyz dir_x{0, 0, 0};
//  xyz ray_y{1, 1, 0};
//  xyz dir_y{0, 1, 0};
//  xyz ray_z{1, 0, 1};
//  xyz dir_z{0, 0, 1};
//  xyz ray_yz{1, 1, 1};
//  xyz dir_yz{0, 1, 1};
//  xyz ray_negyz{1, -1, -1};
//  xyz dir_negyz{0, -1, -1};
//  EXPECT_EQ(ray_x, render.GetRay(dir_x));
//  EXPECT_EQ(ray_y, render.GetRay(dir_y));
//  EXPECT_EQ(ray_z, render.GetRay(dir_z));
//  EXPECT_EQ(ray_yz, render.GetRay(dir_yz));
//  EXPECT_EQ(ray_negyz, render.GetRay(dir_negyz));
//}
//
//TEST(Render, spheres) {
//  Render render(800, 600, {0, 0, 0});
//  Render render2(800, 600, {0, 0, 0});
//  Sphere sphere({-1, -1, -10}, 2, Materials::red_rubber);
//  Sphere sphere2({0, 0, -15}, 2, Materials::green_rubber);
//  Sphere sphere3({1, 1, -20}, 2, Materials::blue_rubber);
//
//  std::vector<Sphere> spheres;
//  spheres.push_back(sphere);
//  spheres.push_back(sphere2);
//  spheres.push_back(sphere3);
//
//  render.image_.DrawGradientBackground();
//  render.RenderSphere(sphere);
//  render.RenderSphere(sphere2);
//  render.RenderSphere(sphere3);
//  //  render.SaveImage("RenderSingleTest.png");
//
//  render2.image_.DrawGradientBackground();
//  render2.RenderScene(spheres);
//  //  render2.SaveImage("RenderMultiTest.png");
//}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
