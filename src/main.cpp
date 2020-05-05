//#include "ObjLoader.h"
//#include "Render.h"
//#include "Rectangle.h"
//#include <chrono>
//#include <eigen3/Eigen/Geometry>
//#include <memory>
#include "SceneManager.h"
#define TRACERR_MAJOR_VERSION 1
#define TRACERR_MINOR_VERSION 0

// Coordinate frame of reference
// Camera is at (0,0,0)
// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

float Material::ambient_light{0.05};


int main(int argc, char **argv) {


  if (std::strcmp(argv[1], "-h") == 0) {
    std::cout << "Tracerr v" << TRACERR_MAJOR_VERSION << "." << TRACERR_MINOR_VERSION << '\n';
    std::cout << "Check https://github.com/DLopezMadrid/tracerr for source code and docs \n";
    std::cout << "Usage:  " << argv[0] << " -OPTION [IO_TEXT_PROTO_FILE]" << '\n';
    std::cout << "Available options are: \n"
                 "  -r TEXT_PROTO_FILE -> Render scene described in text_proto file \n"
                 "    EXAMPLE: ./tracerr -r simple_scene.textproto \n"
                 "  -t SAVEFILE_FOR_TEMPLATE_FILE -> Generates an empty Template with comments for a scene \n"
                 "    EXAMPLE: ./tracerr -t my_new_scene.textproto \n"
                 "  -u SAVEFILE_FOR_UNCOMMENTED_TEMPLATE_FILE -> Generates an empty Template WITHOUT comments for a scene \n"
                 "    EXAMPLE: ./tracerr -u my_new_scene.textproto \n"
                 "  -e SAVEFILE_FOR_EXAMPLE_SCENE -> Generates a textproto file of a Example scene and saves it to the specified file \n"
                 "    EXAMPLE: ./tracerr -e example_scene.textproto \n"
                 "  -v -> Show program version \n"
                 "  -h -> Displays this menu \n";
  } else if (std::strcmp(argv[1], "-v") == 0) {
    std::cout << "Tracerr v" << TRACERR_MAJOR_VERSION << "." << TRACERR_MINOR_VERSION << '\n';
  } else if (std::strcmp(argv[1], "-r") == 0) {
    std::cout << "Rendering " << argv[1] << '\n';
    SceneManager sm;
    sm.LoadScene(argv[2]);
  } else if (std::strcmp(argv[1], "-e") == 0) {
    std::cout << "Generating example file " << argv[2] << '\n';
    SceneManager sm;
    sm.SaveExampleScene(argv[2]);
  } else if (std::strcmp(argv[1], "-t") == 0) {
    std::cout << "Generating template file with comments " << argv[2] << '\n';
    SceneManager sm;
    sm.WriteTemplateFile(argv[2]);
  } else if (std::strcmp(argv[1], "-u") == 0) {
    std::cout << "Generating template file without comments " << argv[2] << '\n';
    SceneManager sm;
    sm.WriteCleanTemplateFile(argv[2]);
  } else {
    std::cerr << "Unknown command, use -h to show the help menu:" << std::endl;
    std::cerr << argv[0] << " -h" << std::endl;
    return -1;
  }

  return 0;
}