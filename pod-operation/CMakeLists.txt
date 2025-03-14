cmake_minimum_required(VERSION 3.15)
project(PodOperation LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE C_SOURCES src/components/c/*.c src/utils/*.c)
file(GLOB_RECURSE CPP_SOURCES src/components/cpp/*.cpp src/*.cpp)

add_library(pod_c ${C_SOURCES})
set_target_properties(pod_c PROPERTIES LINKER_LANGUAGE C)

add_executable(pod_operation ${CPP_SOURCES})
target_link_libraries(pod_operation pod_c pthread m)
