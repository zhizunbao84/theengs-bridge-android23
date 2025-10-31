cmake_minimum_required(VERSION 3.18)
project(theengs-bridge)

# 1. 解码库（header-only）
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../../decoder ${CMAKE_BINARY_DIR}/decoder)

# 2. JNI 共享库
add_library(theengs-jni SHARED native.cpp)
target_include_directories(theengs-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src
    ${CMAKE_SOURCE_DIR}/../../../../decoder/include
)
target_link_libraries(theengs-jni decoder)

# 3. CLI 可执行文件（仅一次定义）
add_executable(theengs-cli-jni cli.cpp)
target_include_directories(theengs-cli-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src
    ${CMAKE_SOURCE_DIR}/../../../../decoder/include
)
target_link_libraries(theengs-cli-jni decoder)

# 可选：把可执行文件安装到 lib/<abi> 目录
set_target_properties(theengs-cli-jni PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    SUFFIX "")
