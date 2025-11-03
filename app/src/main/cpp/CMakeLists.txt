cmake_minimum_required(VERSION 3.18)
project(theengs-bridge)

# 1. 解码库（header-only）
add_subdirectory(${CMAKE_SOURCE_DIR}/../../../../decoder ${CMAKE_BINARY_DIR}/decoder)

# 2. JNI 共享库
add_library(theengs-jni SHARED native.cpp)
target_include_directories(theengs-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src
)
target_link_libraries(theengs-jni decoder)

# 3. CLI 可执行文件
add_executable(theengs-cli-jni cli.cpp)

# 改成共享库（同名 .so）
add_library(theengs-cli-jni SHARED cli.cpp)

# 其余不变
target_include_directories(theengs-cli-jni PRIVATE
    ${CMAKE_SOURCE_DIR}/../../../../decoder/src
)
target_link_libraries(theengs-cli-jni decoder)

install(TARGETS theengs-cli-jni
        LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        OPTIONAL)
