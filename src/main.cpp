//#include "ObjLoader.h"
//#include "Render.h"
//#include "Rectangle.h"
//#include <chrono>
//#include <eigen3/Eigen/Geometry>
//#include <memory>
#include "SceneManager.h"

// Coordinate frame of reference
// Camera is at (0,0,0)
// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

float Material::ambient_light{0.05};


int main(int argc, char **argv) {

  if (argc != 2) {
    std::cerr << "Usage:  " << argv[0] << " TEXT_PROTO_FILE" << std::endl;
    return -1;
  }
  std::cout << "Rendering " << argv[1] << '\n';
  SceneManager sm;
  sm.LoadScene(argv[1]);

  return 0;
}