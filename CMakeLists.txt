cmake_minimum_required(VERSION 3.0)

# Set project name
project(simpleEngine)

# Set output directories for binary, library, and archive files
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find all source files
file(GLOB_RECURSE SRC_FILES ${CMAKE_BINARY_DIR}/../src/*.cpp)

# Add final executable target
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

# Find all library include paths
find_path(SDL2_INCLUDE SDL.h PATHS ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/include ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/build/include)
find_path(GLEW_INCLUDE glew.h PATHS ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/include ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/build/include ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/include/GL ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/build/include/GL)
find_path(ASSIMP_INCLUDE assimp PATHS ${CMAKE_BINARY_DIR}/../3rdparty/ASSIMP/include ${CMAKE_BINARY_DIR}/../3rdparty/ASSIMP/build/include)
find_path(STB_IMAGE_INCLUDE stb_image.h PATHS ${CMAKE_BINARY_DIR}/../3rdparty ${CMAKE_BINARY_DIR}/../3rdparty/stb)

# Add library include directories
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_INCLUDE})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${GLEW_INCLUDE})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${ASSIMP_INCLUDE})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${STB_IMAGE_INCLUDE})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/../src)

# Find all of the libraries
find_library(SDL2_LIB SDL2.dll PATHS ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/lib ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/build/lib ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/build)
find_library(SDL2_MAIN_LIB SDL2main PATHS ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/lib ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/build/lib ${CMAKE_BINARY_DIR}/../3rdparty/SDL2/build)
find_library(GLEW_LIB glew32.dll PATHS ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/lib ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/build/lib ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/bin ${CMAKE_BINARY_DIR}/../3rdparty/GLEW/build/bin)
find_library(ASSIMP_LIB libassimp.dll PATHS ${CMAKE_BINARY_DIR}/../3rdparty/ASSIMP/lib ${CMAKE_BINARY_DIR}/../3rdparty/ASSIMP/build/lib ${CMAKE_BINARY_DIR}/../3rdparty/ASSIMP/build)
find_library(OPENGL_LIB opengl32)
find_library(MINGW32_LIB mingw32 PATHS C:/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/x86_64-w64-mingw32/lib)

# Link all of the libraries to the executable
target_link_libraries(${CMAKE_PROJECT_NAME} ${MINGW32_LIB} ${GLEW_LIB} ${SDL2_MAIN_LIB} ${SDL2_LIB} ${ASSIMP_LIB} ${OPENGL_LIB})

# Custom commands to copy the sdl and glew shared libraries over to the binary directory with the final executable
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    			   COMMAND ${CMAKE_COMMAND} -E copy_if_different
        		   ${SDL2_LIB}
				   $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${GLEW_LIB}
				   $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${ASSIMP_LIB}
				   $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>)

# Since we are copying some shared libraries into the binary dir, we must list those as needed to be deleted when make clean is used.
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/SDL2.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/glew32.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libassimp.dll)
