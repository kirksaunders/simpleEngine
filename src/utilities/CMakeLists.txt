add_library(UTILITIES_SRC INTERFACE IMPORTED)
target_sources(UTILITIES_SRC
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/exception.cpp
)