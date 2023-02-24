cmake_minimum_required(VERSION 3.6)

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
        ${LIBRARY_NAME}
    )

endforeach()
