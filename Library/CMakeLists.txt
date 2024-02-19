if (NOT TARGET ${BOARD}_argb)
    if (${ROBOT} STREQUAL "bpm44")
        add_library(
            ${BOARD}_argb 
            STATIC ${SUBREPO_DIR}/stm32_argb_dma/Library/ARGB.c
        )
        target_include_directories(
            ${BOARD}_argb 
            PUBLIC ${SUBREPO_DIR}/stm32_argb_dma/Library
        )
        target_link_libraries(
            ${BOARD}_argb 
            PUBLIC ${BOARD}_kernel
        )
    else ()
        add_library(${BOARD}_argb INTERFACE)
    endif ()
endif ()