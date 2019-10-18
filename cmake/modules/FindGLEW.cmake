# Try to find GLEW library
# Once done this will define
#  GLEW_FOUND - if system found GLEW library
#  GLEW_INCLUDE_DIRS - The GLEW include directories
#  GLEW_LIBRARIES - The libraries needed to use GLEW
#  GLEW_SHARED - The GLEW shared library (only set if the shared library is the one being used)
#  GLEW_DEFINITIONS - Compiler switches required for using GLEW

# GLEW depends on opengl
find_package(OpenGL REQUIRED)

# Force cmake to refind the library when user changes any cmake library location options
unset(INCLUDE_DIR CACHE)
unset(LIBRARY CACHE)
unset(SHARED_LIBRARY CACHE)

# set library name base
if (WIN32)
    set(LIBRARY_NAME_BASE glew32)
else()
    if (USE_STATIC_LIBS)
        set(LIBRARY_NAME_BASE glew)
    else()
        set(LIBRARY_NAME_BASE GLEW)
    endif()
endif()

# set library name prefix
if (MSVC)
    set(LIBRARY_NAME_PREFIX "")
else()
    set(LIBRARY_NAME_PREFIX "lib")
endif()

# set library name suffix
if (WIN32)
    if (MSVC)
        if (GLEW_USE_DEBUG)
            # debug version of glew has a d suffix
            set(LIBRARY_NAME_SUFFIX "d")
        endif()
    endif()
endif()

# set library extra extension
if (WIN32)
    if (NOT MSVC)
        if (NOT GLEW_USE_STATIC_LIBS)
            set(LIBRARY_EXTENSION ".dll")
        endif()
    endif()
endif()

# set library suffixes
set(ORIG_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if (MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib)
else()
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
    else()
        if (USE_STATIC_LIBS)
            set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
        else()
            set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
        endif()
    endif()
endif()

# append all of the above into one library name, and set shared library name
set(LIBRARY_NAME ${LIBRARY_NAME_PREFIX}${LIBRARY_NAME_BASE}${LIBRARY_NAME_SUFFIX}${LIBRARY_EXTENSION})
if (WIN32)
    set(SHARED_LIBRARY_NAME ${LIBRARY_NAME_BASE}${LIBRARY_NAME_SUFFIX}.dll)
else()
    set(SHARED_LIBRARY_NAME ${LIBRARY_NAME_PREFIX}${LIBRARY_NAME_BASE}${LIBRARY_NAME_SUFFIX}.so)
endif()

# set search locations
if (WIN32)
    set(SEARCH_INCLUDE_LOCATIONS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
                                 $ENV{PROGRAMFILES}
                                 "$ENV{PROGRAMFILES\(x86\)}")
    set(SEARCH_LIBRARY_LOCATIONS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
                                 $ENV{PROGRAMFILES}
                                 "$ENV{PROGRAMFILES\(x86\)}")
else()
    set(SEARCH_INCLUDE_LOCATIONS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
                                 /usr/include
                                 /usr/local/include
                                 /sw/include
                                 /opt/local/include)
    set(SEARCH_LIBRARY_LOCATIONS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
                                 /usr/lib64
                                 /usr/lib
                                 /usr/local/lib64
                                 /usr/local/lib
                                 /sw/lib
                                 /opt/local/lib)
endif()

# set sub-search locations
set(SEARCH_INCLUDE_SUBLOCATIONS glew
                                glew/include
                                glew/build
                                glew/build/include)
set(SEARCH_LIBRARY_SUBLOCATIONS glew
                                glew/lib
                                glew/bin
                                glew/build
                                glew/build/lib
                                glew/build/bin)

# find include dir
if (GLEW_INCLUDE_DIR)
    find_path(INCLUDE_DIR
        NAMES GL/glew.h
        PATHS ${GLEW_INCLUDE_DIR}
        PATH_SUFFIXES
        DOC "The GLEW include directory"
    )
else()
    find_path(INCLUDE_DIR
        NAMES GL/glew.h
        PATHS ${SEARCH_INCLUDE_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_INCLUDE_SUBLOCATIONS}
        DOC "The GLEW include directory"
    )
endif()

if (GLEW_LIBRARY_DIR)
    find_library(LIBRARY
        NAMES ${LIBRARY_NAME}
        PATHS ${GLEW_LIBRARY_DIR}
        PATH_SUFFIXES
            lib
            bin
            ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The GLEW library path"
    )
else()
    find_library(LIBRARY
        NAMES ${LIBRARY_NAME}
        PATHS ${SEARCH_LIBRARY_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The GLEW library path"
    )
endif()

# empty library suffixes
set(CMAKE_FIND_LIBRARY_SUFFIXES "")

# find shared library
if (NOT GLEW_USE_STATIC_LIBS)
    if (GLEW_SHARED_DIR)
        find_library(SHARED_LIBRARY
            NAMES ${SHARED_LIBRARY_NAME}
            PATHS ${GLEW_SHARED_DIR}
            PATH_SUFFIXES
            DOC "The GLEW shared library path"
        )
    else()
        get_filename_component(LIBRARY_DIR_TEMP ${LIBRARY} DIRECTORY)
        
        find_library(SHARED_LIBRARY
            NAMES ${SHARED_LIBRARY_NAME}
            PATHS
                ${GLEW_LIBRARY_DIR_TEMP}
                ${GLEW_LIBRARY_DIR_TEMP}/..
                ${SEARCH_LIBRARY_LOCATIONS}
            PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The GLEW shared library path"
        )
    endif()
endif()

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${ORIG_SUFFIXES})

include(FindPackageHandleStandardArgs)
# let cmake handle erroring if the library components weren't found
if (GLEW_USE_STATIC_LIBS)
    find_package_handle_standard_args(GLEW DEFAULT_MSG LIBRARY INCLUDE_DIR)
else()
    find_package_handle_standard_args(GLEW DEFAULT_MSG LIBRARY INCLUDE_DIR SHARED_LIBRARY)
endif()

if (GLEW_FOUND)
    set(GLEW_INCLUDE_DIRS ${INCLUDE_DIR})
    set(GLEW_LIBRARIES ${LIBRARY})
    set(GLEW_SHARED ${SHARED_LIBRARY})

    # define GLEW_STATIC if glew is static
    if (GLEW_USE_STATIC_LIBS)
        set(GLEW_DEFINITIONS "-DGLEW_STATIC")
    endif()
endif()

# unset all of the previous variables we set
unset(LIBRARY CACHE)
unset(INCLUDE_DIR CACHE)
unset(SHARED_LIBRARY CACHE)

# Export GLEW::GLEW as target if it doesn't already exist
if (GLEW_FOUND AND NOT TARGET GLEW::GLEW)
    add_library(GLEW::GLEW INTERFACE IMPORTED)
    set_target_properties(GLEW::GLEW PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${GLEW_INCLUDE_DIRS}"
    )
    target_link_libraries(GLEW::GLEW
        INTERFACE
            ${GLEW_LIBRARIES}
            OpenGL::GL
    )

    if (GLEW_USE_STATIC_LIBS)
        set_target_properties(GLEW::GLEW PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "GLEW_STATIC"
        )
    else()
        add_library(GLEW::GLEW-shared SHARED IMPORTED)
        set_property(TARGET GLEW::GLEW-shared APPEND PROPERTY IMPORTED_LOCATION "${GLEW_SHARED}")
    endif()
endif()