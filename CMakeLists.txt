cmake_minimum_required(VERSION 3.16)
project(tracerr)

set(CMAKE_CXX_STANDARD 20)
# enable coverage in the debug builds
#set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")

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
#if (protobuf_VERBOSE)
#    message(STATUS "Using Protocol Buffers ${Protobuf_VERSION}")
#endif ()

ADD_SUBDIRECTORY(proto)
add_library(proto STATIC scene_proto)
set_target_properties(supporting_library PROPERTIES POSITION_INDEPENDENT_CODE ON)
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
#target_link_libraries(tracerr Threads::Threads)
#target_link_libraries(tracerr protobuf::libprotobuf)
#TARGET_LINK_LIBRARIES(tracerr proto ${PROTOBUF_LIBRARY})
#TARGET_LINK_LIBRARIES(tracerr proto ${PROTOBUF_LIBRARIES})
#add_dependencies(tracerr scene_proto)
target_link_libraries(tracerr scene_proto)
