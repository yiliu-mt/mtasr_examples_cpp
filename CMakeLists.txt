cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(mtasr)

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(NOT MSVC)
  # Keep the same with openfst, -fPIC or -fpic
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fPIC")
else()
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# Include all dependency
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/gflags-2.2.2-cmake-2.8/build/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/glog-0.4.0-cmake-2.8/build
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/glog-0.4.0-cmake-2.8/src
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/grpc/include
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/grpc/third_party/protobuf/src
)

link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/gflags-2.2.2-cmake-2.8/build/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/glog-0.4.0-cmake-2.8/build
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/grpc/libs/opt
)

# Build all libraries
add_subdirectory(utils)
add_subdirectory(frontend)
add_subdirectory(grpc)

# Build all bins
add_subdirectory(bin)
