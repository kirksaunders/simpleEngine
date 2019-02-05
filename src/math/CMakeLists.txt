add_library(MATH_SRC INTERFACE IMPORTED)
target_sources(MATH_SRC
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/matrix4x4.cpp
        ${CMAKE_CURRENT_LIST_DIR}/vector4.cpp
)