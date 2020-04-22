#include "Image.h"
#include "Shape.h"
#include <gtest/gtest.h>


TEST(Tracer, TracerCtor) {
  int x = 100;
  int y = 200;
  Image r(x, y);
  ASSERT_EQ(x, r.GetSizeX());
  ASSERT_EQ(y, r.GetSizeY());
  ASSERT_EQ(x, (r.pixels_)->rows());
  ASSERT_EQ(y, (r.pixels_)->cols());
  ASSERT_EQ(x * y, (r.pixels_)->size());
}

TEST(Tracer, TracerResize) {
  int x = 100;
  int y = 200;
  Image r(x, y);
  r.Resize(x * 2, y * 2);
  ASSERT_EQ(x * 2, r.GetSizeX());
  ASSERT_EQ(y * 2, r.GetSizeY());
  ASSERT_EQ(x * y * 4, (r.pixels_)->size());
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
