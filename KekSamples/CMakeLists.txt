cmake_minimum_required(VERSION 3.10)
project(KekSamples)

add_executable(Sample00_Smoke 00_Smoke/main.cpp)
target_link_libraries(Sample00_Smoke PRIVATE KekWorld)

add_executable(Sample01_Window 01_Window/main.cpp)
target_link_libraries(Sample01_Window PRIVATE KekApplication)

add_executable(Sample02_GLObjects 02_GLObjects/main.cpp)
target_link_libraries(Sample02_GLObjects PRIVATE KekApplication)

add_executable(Sample03_Shaders 03_Shaders/main.cpp)
target_link_libraries(Sample03_Shaders PRIVATE KekApplication)