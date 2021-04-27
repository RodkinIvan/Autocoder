cmake_minimum_required(VERSION 3.19)
project(AutocoderSrc)

set(CMAKE_CXX_STANDARD 20)

add_executable(AutocoderSrc main.cpp AutocoderSrc/Perceptron.h AutocoderSrc/Autocoder.h AutocoderSrc/Autocoder.cpp)