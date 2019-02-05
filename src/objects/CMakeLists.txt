add_library(OBJECTS_SRC INTERFACE IMPORTED)
target_sources(OBJECTS_SRC
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/cuboid.cpp
        ${CMAKE_CURRENT_LIST_DIR}/mesh.cpp
        ${CMAKE_CURRENT_LIST_DIR}/model.cpp
        ${CMAKE_CURRENT_LIST_DIR}/primitive3d.cpp
)