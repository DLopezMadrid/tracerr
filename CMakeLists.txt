cmake_minimum_required(VERSION 3.16)
project(tracerr)

set(CMAKE_CXX_STANDARD 20)

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

if (protobuf_VERBOSE)
    message(STATUS "Using Protocol Buffers ${Protobuf_VERSION}")
endif ()

INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER scene.proto)
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})

add_executable(tracerr src/main.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h src/Light.h src/Render.h src/Render.cpp src/Triangle.cpp src/Triangle.h src/Rectangle.cpp src/Rectangle.h src/ObjLoader.cpp src/ObjLoader.h src/ThreadPool.h proto/scene.pb.cc proto/scene.pb.h src/SceneManager.cpp src/SceneManager.h)
target_link_libraries(tracerr Threads::Threads)
target_link_libraries(tracerr protobuf::libprotobuf)
# Link runTests with what we want to test and the GTest and pthread library
#add_executable(runTests src/tests.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h)
#target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)