if (NOT TARGET ${BOARD}_can_mock)

    mock_header(${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR})
    add_library(
        ${BOARD}_can_mock
        STATIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_can_icd.c
                ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_canserver.c
    )
    target_include_directories(
        ${BOARD}_can_mock
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/mocks
               ${CMAKE_CURRENT_SOURCE_DIR}/../include
               ${CMAKE_CURRENT_SOURCE_DIR}/../include/can
    )
    target_link_libraries(
        ${BOARD}_can_mock
        PRIVATE ${BOARD}_unit_test_core
                ${BOARD}_kernel
    )

endif ()
