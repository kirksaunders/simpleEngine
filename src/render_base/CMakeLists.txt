add_library(RENDER_BASE_SRC INTERFACE IMPORTED)
target_sources(RENDER_BASE_SRC
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/camera.cpp
        ${CMAKE_CURRENT_LIST_DIR}/color.cpp
        ${CMAKE_CURRENT_LIST_DIR}/context3d.cpp
        ${CMAKE_CURRENT_LIST_DIR}/shader.cpp
        ${CMAKE_CURRENT_LIST_DIR}/shadermanager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/shadervariable.cpp
        ${CMAKE_CURRENT_LIST_DIR}/shadervariableblock.cpp
        ${CMAKE_CURRENT_LIST_DIR}/texture.cpp
        ${CMAKE_CURRENT_LIST_DIR}/texturebuffer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/texturemanager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/uniformbuffermanager.cpp
        ${CMAKE_CURRENT_LIST_DIR}/window.cpp
)