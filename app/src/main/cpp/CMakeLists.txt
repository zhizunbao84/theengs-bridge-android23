cmake_minimum_required(VERSION 3.18)
project(theengs-bridge)

# 1. 解码库头文件目录（header-only）
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../../decoder ${CMAKE_BINARY_DIR}/decoder)

# 2. JNI 共享库
add_library(theengs-jni SHARED native.cpp)
target_include_directories(theengs-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src   # ① 头文件根目录
    ${CMAKE_SOURCE_DIR}/../../../../decoder/include
)
target_link_libraries(theengs-jni decoder)

# 3. CLI 可执行文件
add_executable(theengs-cli cli.cpp)
target_include_directories(theengs-cli PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src   # ② 同样加上
    ${CMAKE_SOURCE_DIR}/../../../../decoder/include
)
target_link_libraries(theengs-cli decoder)

# CLI 可执行文件（安装到 lib/<abi>/libtheengs-cli.so）
add_executable(theengs-cli cli.cpp)
target_link_libraries(theengs-cli decoder)
set_target_properties(theengs-cli PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
    SUFFIX "")
