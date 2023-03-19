cmake_minimum_required(VERSION 3.6)

project(
    kedixa-test
    LANGUAGES CXX
)

find_package(kedixa REQUIRED)
find_package(Threads REQUIRED)

include_directories(${KEDIXA_INCLUDE_DIR})
link_directories(${KEDIXA_LIBRARY_DIR})

set(TEST_TARGETS
    test_bigint
    test_multiarray
    test_rational
    test_timer
    test_unsigned_bigint
)

set(CMAKE_BUILD_TYPE Debug)

foreach(TARGET ${TEST_TARGETS})
    add_executable(
        ${TARGET}
        ${TARGET}.cpp
    )

    target_link_libraries(
        ${TARGET}
        ${KEDIXA_LIBRARIES}
        Threads::Threads
    )
endforeach()
