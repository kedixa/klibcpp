cmake_minimum_required(VERSION 3.6)

add_library(${LIBRARY_NAME} STATIC
    bigint.cpp
    rational.cpp
    unsigned_bigint.cpp
)

include_directories(${LIBRARY_NAME}
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
