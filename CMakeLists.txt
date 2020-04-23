cmake_minimum_required(VERSION 3.16)
project(tracerr)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tracerr src/main.cpp src/Shape.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/File.cpp src/File.h src/Render.cpp src/Render.h src/Material.h src/Light.h)

# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests src/tests.cpp src/Shape.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/File.cpp src/File.h src/Cube.cpp src/Cube.h src/Render.cpp src/Render.h src/Material.h src/Light.h)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)