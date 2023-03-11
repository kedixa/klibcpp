cmake_minimum_required(VERSION 3.6)

add_library(${KEDIXA_LIBRARY_NAME} STATIC
    bigint.cpp
    rational.cpp
    unsigned_bigint.cpp
)

include_directories(${KEDIXA_LIBRARY_NAME}
    ${KEDIXA_INCLUDE_DIR}
)

target_include_directories(${KEDIXA_LIBRARY_NAME} PUBLIC
    ${KEDIXA_INCLUDE_DIR}
)
