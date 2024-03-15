if (${ROBOT} STREQUAL "bpm44")

    mock_header(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR})
    add_library(
        ${BOARD}_argb_mock
        STATIC mocks/ARGB_mock.c
            mocks/fast_math_mock.c
    )
    target_include_directories(
        ${BOARD}_argb_mock
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/mocks
            ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
    target_link_libraries(
        ${BOARD}_argb_mock
        PRIVATE ${BOARD}_unit_test_core
                ${BOARD}_kernel
    )
else ()

    add_library(${BOARD}_argb_mock INTERFACE)

endif ()
