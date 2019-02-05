add_library(ASTAR_SRC INTERFACE IMPORTED)
target_sources(ASTAR_SRC
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/astar.cpp
        ${CMAKE_CURRENT_LIST_DIR}/grid.cpp
        ${CMAKE_CURRENT_LIST_DIR}/node.cpp
)