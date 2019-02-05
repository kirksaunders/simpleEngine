include(${CMAKE_CURRENT_LIST_DIR}/astar/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/mains/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/math/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/objects/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/raytracer/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/render_base/CMakeLists.txt)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        ASTAR_SRC
        MAINS_SRC
        MATH_SRC
        OBJECTS_SRC
        RAYTRACER_SRC
        RENDER_BASE_SRC
)