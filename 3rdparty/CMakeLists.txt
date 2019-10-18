# Create options for user
option(ASSIMP_USE_STATIC_LIBS
    "Use ASSIMP's static library instead of the shared one"
    OFF
)
option(GLEW_USE_STATIC_LIBS
    "Use GLEW's static library instead of the shared one"
    OFF
)
option(SDL2_USE_STATIC_LIBS
    "Use SDL's static library instead of the shared one"
    OFF
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Threads REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(STB_IMAGE REQUIRED)

add_library(3RDPARTY INTERFACE IMPORTED)
target_link_libraries(3RDPARTY
    INTERFACE
        Threads::Threads
        SDL2::SDL2main
        SDL2::SDL2
        GLEW::GLEW
        ASSIMP::ASSIMP
        STB_IMAGE::STB_IMAGE
)

# Sometimes it is useful to copy 3rd party dll's into the bin directory so that the program can run
# successfully, especially on Windows
if (TARGET GLEW::GLEW-shared)
    list(APPEND 3RDPARTY_DLLS ${GLEW_SHARED})

    get_filename_component(GLEW_SHARED_NAME "${GLEW_SHARED}" NAME)
    list(APPEND 3RDPARTY_DLLS_FINAL "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${GLEW_SHARED_NAME}")
endif()

if (TARGET SDL2::SDL2-shared)
    list(APPEND 3RDPARTY_DLLS ${SDL2_SHARED})
    
    get_filename_component(SDL2_SHARED_NAME "${SDL2_SHARED}" NAME)
    list(APPEND 3RDPARTY_DLLS_FINAL "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${SDL2_SHARED_NAME}")
endif()

if (TARGET ASSIMP::ASSIMP-shared)
    list(APPEND 3RDPARTY_DLLS ${ASSIMP_SHARED})
        
    get_filename_component(ASSIMP_SHARED_NAME "${ASSIMP_SHARED}" NAME)
    list(APPEND 3RDPARTY_DLLS_FINAL "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ASSIMP_SHARED_NAME}")
endif()