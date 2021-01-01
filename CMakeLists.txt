cmake_minimum_required (VERSION 3.15)

project ("accel")

add_library(accel INTERFACE)
target_compile_features(accel INTERFACE cxx_std_20)
target_sources(accel INTERFACE "include/accel.core.h")
target_include_directories(accel INTERFACE "include/")

enable_testing()
add_subdirectory("test")