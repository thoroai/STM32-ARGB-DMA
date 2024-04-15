if (${ROBOT} STREQUAL "bpm44")
    list(APPEND ARGB_DONT_MOCK ${CMAKE_CURRENT_SOURCE_DIR}/../fast_math.h)
    mock_header(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR} cmock_config.yml ${ARGB_DONT_MOCK})
    
    add_library(
        ${BOARD}_argb_mock
        STATIC mocks/ARGB_mock.c
    )
    target_include_directories(
        ${BOARD}_argb_mock
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/mocks
            ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
    target_link_libraries(
        ${BOARD}_argb_mock
        PRIVATE ${BOARD}_unit_test_core
                ${BOARD}_kernel_mock
    )
else ()

    add_library(${BOARD}_argb_mock INTERFACE)

endif ()
