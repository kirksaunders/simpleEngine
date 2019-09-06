include(${CMAKE_CURRENT_LIST_DIR}/astar/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/mains/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/math/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/objects/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/raytracer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/render_base/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/utilities/CMakeLists.txt)

add_library(SOURCES INTERFACE IMPORTED)
target_link_libraries(SOURCES
    INTERFACE
        ASTAR_SRC
        MAINS_SRC
        MATH_SRC
        OBJECTS_SRC
        RAYTRACER_SRC
        RENDER_BASE_SRC
        UTILITIES_SRC
)