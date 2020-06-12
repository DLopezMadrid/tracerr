cmake_minimum_required(VERSION 3.16)
project(tracerr)

set(CMAKE_CXX_STANDARD 20)
# uncomment to enable coverage analysis
#set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")

# set a build type if non is given
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-fpic")

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

if( PYBIND )
  # python module
  add_subdirectory(pybind11)
  pybind11_add_module(tracerr
          src/main_py.cpp
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

else()
  # main executable
  ADD_EXECUTABLE(tracerr src/main.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h src/Light.h src/Render.h src/Render.cpp src/Triangle.cpp src/Triangle.h src/Rectangle.cpp src/Rectangle.h src/ObjLoader.cpp src/ObjLoader.h src/ThreadPool.h src/SceneManager.cpp src/SceneManager.h)

  # Tracerr libraries
  TARGET_LINK_LIBRARIES(tracerr Threads::Threads)
  TARGET_LINK_LIBRARIES(tracerr proto ${PROTOBUF_LIBRARIES})

  # test executable
  ADD_EXECUTABLE(Tests tests/tests.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h src/Light.h src/Render.h src/Render.cpp src/Triangle.cpp src/Triangle.h src/Rectangle.cpp src/Rectangle.h src/ObjLoader.cpp src/ObjLoader.h src/ThreadPool.h src/SceneManager.cpp src/SceneManager.h)

  # Tests executable libraries
  TARGET_LINK_LIBRARIES(Tests gtest_main)
  TARGET_LINK_LIBRARIES(Tests Threads::Threads)
  TARGET_LINK_LIBRARIES(Tests proto ${PROTOBUF_LIBRARY})


  # Everything below this line is related to download and install gtest
  # Download and unpack googletest at configure time
  configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
  endif()

endif()
