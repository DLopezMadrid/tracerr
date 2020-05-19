cmake_minimum_required(VERSION 3.16)
project(tracerr)

set(CMAKE_CXX_STANDARD 20)
# uncomment to enable coverage analysis
#set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")

# set a build type if non is given
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

## enable optimization in release version and compiler warnings
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# add multithreading library
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# add protobuf library
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf 3.6.1 REQUIRED)

# add the proto directory and its respective CMakeList.txt file
ADD_SUBDIRECTORY(proto)

# include build directory since we are generating the protobuf C++ header and source on it
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(pybind11)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
#add_executable(tracerr src/main.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h src/Light.h src/Render.h src/Render.cpp src/Triangle.cpp src/Triangle.h src/Rectangle.cpp src/Rectangle.h src/ObjLoader.cpp src/ObjLoader.h src/ThreadPool.h src/SceneManager.cpp src/SceneManager.h)
pybind11_add_module(tracerr
        src/main.cpp
        src/Shape.h
        src/Image.cpp
        src/Image.h
        src/Sphere.cpp
        src/Sphere.h
        src/ImageFile.h
        src/Material.h
        src/Light.h
        src/Render.h
        src/Render.cpp
        src/Triangle.cpp
        src/Triangle.h
        src/Rectangle.cpp
        src/Rectangle.h
        src/ObjLoader.cpp
        src/ObjLoader.h
        src/ThreadPool.h
        src/SceneManager.cpp
        src/SceneManager.h)

target_link_libraries(tracerr PRIVATE pybind11::module)
target_link_libraries(tracerr PRIVATE Threads::Threads)
target_link_libraries(tracerr PRIVATE proto ${PROTOBUF_LIBRARIES})
