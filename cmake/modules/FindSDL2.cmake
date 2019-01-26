# Try to find SDL2 library
# Once done this will define
#  SDL2_FOUND - if system found SDL2 library
#  SDL2_INCLUDE_DIRS - The SDL2 include directories
#  SDL2_LIBRARIES - The libraries needed to use SDL2
#  SDL2_SHARED - The SDL2 shared library (only set if the shared library is the one being used)

# Define additional compiler definitions and handle different libraries
set(SDL2_ORIG_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if (SDL2_USE_STATIC_LIBS)
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)
        set(SDL2_LIBRARY_NAME SDL2-static)
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
        set(SDL2_LIBRARY_NAME SDL2-static)
    endif()

    set(SDL2_IS_STATIC TRUE)
else()
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a .dll)
        set(SDL2_LIBRARY_NAME SDL2.dll)
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
        set(SDL2_LIBRARY_NAME SDL2.so)
    endif()
endif()
    
if (WIN32)
    find_path(SDL2_INCLUDE_DIR
        NAMES SDL.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            SDL2
            SDL2/include
            SDL2/include/SDL2
            SDL2/build
            SDL2/build/include
            SDL2/build/include/SDL2
        DOC "The SDL2 include directory"
    )

    find_library(SDL2_LIBRARY
        NAMES ${SDL2_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            SDL2
            SDL2/lib
            SDL2/bin
            SDL2/build
            SDL2/build/lib
            SDL2/build/bin
        DOC "The SDL2 library path"
    )

    set(CMAKE_FIND_LIBRARY_SUFFIXES ${SDL2_ORIG_SUFFIXES})

    find_library(SDL2_MAIN_LIBRARY
        NAMES SDL2main
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            SDL2
            SDL2/lib
            SDL2/bin
            SDL2/build
            SDL2/build/lib
            SDL2/build/bin
        DOC "The SDL2main library path"
    )

    set(CMAKE_FIND_LIBRARY_SUFFIXES "")

    find_library(SDL2_SHARED
        NAMES ${SDL2_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            SDL2
            SDL2/lib
            SDL2/bin
            SDL2/build
            SDL2/build/lib
            SDL2/build/bin
        DOC "The SDL2 shared library path"
    )
else()
    find_path(SDL2_INCLUDE_DIR
        NAMES SDL.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        PATH_SUFFIXES
            SDL2
            SDL2/include
            SDL2/include/SDL2
            SDL2/build
            SDL2/build/include
            SDL2/build/include/SDL2
        DOC "The SDL2 include directory"
    )

    find_library(SDL2_LIBRARY
        NAMES ${SDL2_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
        PATH_SUFFIXES
            SDL2
            SDL2/lib
            SDL2/bin
            SDL2/build
            SDL2/build/lib
            SDL2/build/bin
        DOC "The SDL2 library path"
    )

    set(CMAKE_FIND_LIBRARY_SUFFIXES ${SDL2_ORIG_SUFFIXES})

    find_library(SDL2_MAIN_LIBRARY
        NAMES SDL2main
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
        PATH_SUFFIXES
            SDL2
            SDL2/lib
            SDL2/bin
            SDL2/build
            SDL2/build/lib
            SDL2/build/bin
        DOC "The SDL2main library path"
    )

    if (NOT SDL2_IS_STATIC)
        set(SDL2_SHARED ${GLEW_LIBRARY})
    endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_MAIN_LIBRARY SDL2_INCLUDE_DIR)

if (SDL2_FOUND)
    set(SDL2_LIBRARIES "${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY}")
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
endif()

# Export SDL2::SDL2 as target if it doesn't already exist
if (SDL2_FOUND AND NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
    )

    add_library(SDL2::SDL2main UNKNOWN IMPORTED)
    set_target_properties(SDL2::SDL2main PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
    )

    if (NOT SDL2_IS_STATIC)
        add_library(SDL2::SDL2-shared SHARED IMPORTED)
        set_property(TARGET SDL2::SDL2-shared APPEND PROPERTY IMPORTED_LOCATION "${SDL2_SHARED}")
    endif()

    set_property(TARGET SDL2::SDL2 APPEND PROPERTY IMPORTED_LOCATION "${SDL2_LIBRARY}")
    set_property(TARGET SDL2::SDL2main APPEND PROPERTY IMPORTED_LOCATION "${SDL2_MAIN_LIBRARY}")
endif()

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${SDL2_ORIG_SUFFIXES})

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY SDL2_MAIN_LIBRARY SDL2_LIBRARY_NAME SDL2_SHARED SDL2_ORIG_SUFFIXES)