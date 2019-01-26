# Try to find GLEW library
# Once done this will define
#  GLEW_FOUND - if system found GLEW library
#  GLEW_INCLUDE_DIRS - The GLEW include directories
#  GLEW_LIBRARIES - The libraries needed to use GLEW
#  GLEW_SHARED - The GLEW shared library (only set if the shared library is the one being used)
#  GLEW_DEFINITIONS - Compiler switches required for using GLEW

# Glew depends on opengl
find_package(OpenGL REQUIRED)

# Define additional compiler definitions and handle different libraries
set(GLEW_ORIG_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if (GLEW_USE_STATIC_LIBS)
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)
        set(GLEW_LIBRARY_NAME glew32)
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
        set(GLEW_LIBRARY_NAME glew)
    endif()

    set(GLEW_IS_STATIC TRUE)
else()
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a .dll)
        set(GLEW_LIBRARY_NAME glew32.dll)
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
        set(GLEW_LIBRARY_NAME glew.so)
    endif()
endif()
    
if (WIN32)
    find_path(GLEW_INCLUDE_DIR
        NAMES GL/glew.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            glew
            glew/include
            glew/build
            glew/build/include
        DOC "The glew include directory"
    )

    find_library(GLEW_LIBRARY
        NAMES ${GLEW_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            glew
            glew/lib
            glew/bin
            glew/build
            glew/build/lib
            glew/build/bin
        DOC "The glew library path"
    )

    set(CMAKE_FIND_LIBRARY_SUFFIXES "")

    find_library(GLEW_SHARED
        NAMES ${GLEW_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            glew
            glew/lib
            glew/bin
            glew/build
            glew/build/lib
            glew/build/bin
        DOC "The glew shared library path"
    )
else()
    find_path(GLEW_INCLUDE_DIR
        NAMES GL/glew.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        PATH_SUFFIXES
            glew
            glew/include
            glew/build
            glew/build/include
        DOC "The glew include directory"
    )

    find_library(GLEW_LIBRARY
        NAMES ${GLEW_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
        PATH_SUFFIXES
            glew
            glew/lib
            glew/bin
            glew/build
            glew/build/lib
            glew/build/bin
        DOC "The glew library path"
    )

    if (NOT GLEW_IS_STATIC)
        set(GLEW_SHARED ${GLEW_LIBRARY})
    endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)

if (GLEW_FOUND)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
    set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})

    if (GLEW_IS_STATIC)
        set(GLEW_DEFINITIONS "-DGLEW_STATIC")
    endif()
endif()

# Export GLEW::GLEW as target if it doesn't already exist
if (GLEW_FOUND AND NOT TARGET GLEW::GLEW)
    add_library(GLEW::GLEW UNKNOWN IMPORTED)
    set_target_properties(GLEW::GLEW PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES OpenGL::GL
    )

    if (GLEW_IS_STATIC)
        set_target_properties(GLEW::GLEW PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "GLEW_STATIC"
        )
    else()
        add_library(GLEW::GLEW-shared SHARED IMPORTED)
        set_property(TARGET GLEW::GLEW-shared APPEND PROPERTY IMPORTED_LOCATION "${GLEW_SHARED}")
    endif()

    set_property(TARGET GLEW::GLEW APPEND PROPERTY IMPORTED_LOCATION "${GLEW_LIBRARY}")
endif()

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${GLEW_ORIG_SUFFIXES})

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY GLEW_LIBRARY_NAME GLEW_SHARED GLEW_ORIG_SUFFIXES)