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

include(${CMAKE_SOURCE_DIR}/src/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/3rdparty/CMakeLists.txt)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE
        SOURCES
        3RDPARTY
)

# Sometimes it is useful to copy 3rd party dll's into the bin directory so that the program can run
# successfully, especially on Windows
if (COPY_3RDPARTY_DLLS)
    if (3RDPARTY_DLLS)
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${3RDPARTY_DLLS}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
        
        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
            ${3RDPARTY_DLLS_FINAL}
        )
    endif()
endif()