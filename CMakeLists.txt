cmake_minimum_required(VERSION 3.8)

# Set project name
project(simpleEngine LANGUAGES CXX)

# Set output directories for binary, library, and archive files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add executable target
add_executable(${CMAKE_PROJECT_NAME})

# Set C++ standard to 11
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_include_directories(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

find_package(Threads)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(STB_IMAGE REQUIRED)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        Threads::Threads
        SDL2::SDL2main
        SDL2::SDL2
        GLEW::GLEW
        ASSIMP::ASSIMP
        STB_IMAGE::STB_IMAGE
)

include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)

# Sometimes it is useful to copy 3rd party dll's into the bin directory so that the program can run
# successfully, especially on Windows
if (COPY_3RDPARTY_DLLS)
    if (TARGET GLEW::GLEW-shared)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GLEW::GLEW-shared>
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
        
        get_filename_component(GLEW_SHARED_NAME "${GLEW_SHARED}" NAME)

        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${GLEW_SHARED_NAME}
        )
    endif()

    if (TARGET SDL2::SDL2-shared)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2-shared>
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
        
        get_filename_component(SDL2_SHARED_NAME "${SDL2_SHARED}" NAME)

        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${SDL2_SHARED_NAME}
        )
    endif()

    if (TARGET ASSIMP::ASSIMP-shared)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ASSIMP::ASSIMP-shared>
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)
        
        get_filename_component(ASSIMP_SHARED_NAME "${ASSIMP_SHARED}" NAME)

        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${ASSIMP_SHARED_NAME}
        )
    endif()
endif()