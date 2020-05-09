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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
add_executable(tracerr src/main.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h src/Light.h src/Render.h src/Render.cpp src/Triangle.cpp src/Triangle.h src/Rectangle.cpp src/Rectangle.h src/ObjLoader.cpp src/ObjLoader.h src/ThreadPool.h src/SceneManager.cpp src/SceneManager.h)
target_link_libraries(tracerr Threads::Threads)
#target_link_libraries(tracerr protobuf::libprotobuf)
#TARGET_LINK_LIBRARIES(tracerr proto ${PROTOBUF_LIBRARY})
TARGET_LINK_LIBRARIES(tracerr proto ${PROTOBUF_LIBRARIES})
#add_dependencies(tracerr scene_proto)
#target_link_libraries(tracerr scene_proto)

# Link runTests with what we want to test and the GTest and pthread library
#add_executable(runTests src/tests.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h)
#target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)


add_executable(Tests tests/tests.cpp src/Shape.h src/Image.cpp src/Image.h src/Sphere.cpp src/Sphere.h src/ImageFile.h src/Material.h src/Light.h src/Render.h src/Render.cpp src/Triangle.cpp src/Triangle.h src/Rectangle.cpp src/Rectangle.h src/ObjLoader.cpp src/ObjLoader.h src/ThreadPool.h src/SceneManager.cpp src/SceneManager.h)
target_link_libraries(Tests gtest_main)
target_link_libraries(Tests Threads::Threads)
#target_link_libraries(Tests protobuf::libprotobuf)
TARGET_LINK_LIBRARIES(Tests proto ${PROTOBUF_LIBRARY})
#add_dependencies(Tests scene_proto)
#target_link_libraries(Tests scene_proto)

#Download and unpack googletest at configure time
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

