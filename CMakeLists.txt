cmake_minimum_required(VERSION 3.19)
project(Autocoder)

set(CMAKE_CXX_STANDARD 20)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
add_executable(Autocoder main.cpp AutocoderSrc/Perceptron.h AutocoderSrc/Autocoder.h AutocoderSrc/Autocoder.cpp)