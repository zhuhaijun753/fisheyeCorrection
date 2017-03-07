cmake_minimum_required(VERSION 3.6)
project(fisheyeCorrection)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp
        correctImage.cpp
                 correctTools.h
                 correctVideo.cpp
        fisheye_corrector/fisheye_corrector.h
                 fisheye_corrector/fisheye_corrector.cpp)
                 
add_executable(fisheyeCorrection ${SOURCE_FILES})

target_link_libraries(fisheyeCorrection ${OpenCV_LIBS})