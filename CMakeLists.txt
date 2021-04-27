cmake_minimum_required(VERSION 3.19)
project(Autocoder)

set(CMAKE_CXX_STANDARD 20)

add_executable(Autocoder main.cpp Perceptron.h Autocoder.h Autocoder.cpp)