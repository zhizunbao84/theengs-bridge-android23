cmake_minimum_required(VERSION 3.18)
project(theengs-bridge)

# 1. 解码库（header-only）
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../../decoder ${CMAKE_BINARY_DIR}/decoder)

# 2. JNI 共享库
add_library(theengs-jni SHARED native.cpp)
target_include_directories(theengs-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src   # ← 包含 theengs/ 子目录
)
target_link_libraries(theengs-jni decoder)

# 3. CLI 可执行文件
add_executable(theengs-cli-jni cli.cpp)
target_include_directories(theengs-cli-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src   # ← 同样路径
)
target_link_libraries(theengs-cli-jni decoder)
