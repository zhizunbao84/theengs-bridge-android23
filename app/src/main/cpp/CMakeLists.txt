cmake_minimum_required(VERSION 3.18)
project(theengs-bridge)

# 解码库
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../../decoder ${CMAKE_BINARY_DIR}/decoder)

# JNI 共享库
add_library(theengs-jni SHARED native.cpp)
target_link_libraries(theengs-jni decoder)

# CLI 可执行文件（安装到 lib/<abi>/libtheengs-cli.so）
add_executable(theengs-cli cli.cpp)
target_link_libraries(theengs-cli decoder)
set_target_properties(theengs-cli PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    SUFFIX "")
