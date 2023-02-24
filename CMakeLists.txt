cmake_minimum_required(VERSION 3.6)

project(
    kedixa
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_SKIP_RPATH TRUE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

set(LIBRARY_NAME ${PROJECT_NAME})

add_subdirectory(src)
add_subdirectory(test)
