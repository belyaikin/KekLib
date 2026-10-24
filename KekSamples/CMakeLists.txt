cmake_minimum_required(VERSION 3.10)
project(KekSamples)

add_executable(Sample00_Smoke 00_Smoke/main.cpp)
target_link_libraries(Sample00_Smoke PRIVATE KekWorld)

add_executable(Sample01_Window 01_Window/main.cpp)
target_link_libraries(Sample01_Window PRIVATE KekApplication)