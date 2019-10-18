# Try to find SDL2 library
# Once done this will define
#  SDL2_FOUND - if system found SDL2 library
#  SDL2_INCLUDE_DIRS - The SDL2 include directories
#  SDL2_LIBRARIES - The libraries needed to use SDL2
#  SDL2_SHARED - The SDL2 shared library (only set if the shared library is the one being used)

# Force cmake to refind the library when user changes any cmake library location options
unset(INCLUDE_DIR CACHE)
unset(LIBRARY CACHE)
unset(SHARED_LIBRARY CACHE)

# set library name base
set(LIBRARY_NAME_BASE SDL2)

# set library name prefix
if (MSVC)
    set(LIBRARY_NAME_PREFIX "")
else()
    set(LIBRARY_NAME_PREFIX "lib")
endif()

# set library name suffix1
if (SDL2_USE_STATIC_LIBS)
    set(LIBRARY_NAME_SUFFIX1 "-static")
else()
    set(LIBRARY_NAME_SUFFIX1 "")
endif()

# set library name suffix2
if (WIN32 AND MSVC AND SDL2_USE_DEBUG)
    # debug version has a d suffix
    set(LIBRARY_NAME_SUFFIX2 "d")
else()
    set(LIBRARY_NAME_SUFFIX2 "")
endif()

# set library extra extension
if (WIN32 AND (NOT MSVC) AND (NOT SDL2_USE_STATIC_LIBS))
    set(LIBRARY_EXTENSION ".dll")
else()
    set(LIBRARY_EXTENSION "")
endif()

# set find library suffixes
set(ORIG_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES})
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
set(LIBRARY_NAME ${LIBRARY_NAME_PREFIX}${LIBRARY_NAME_BASE}${LIBRARY_NAME_SUFFIX1}${LIBRARY_NAME_SUFFIX2}${LIBRARY_EXTENSION})
set(MAIN_LIBRARY_NAME ${LIBRARY_NAME_PREFIX}${LIBRARY_NAME_BASE}main${LIBRARY_NAME_SUFFIX2})
if (NOT SDL2_USE_STATIC_LIBS)
    if (WIN32)
        set(SHARED_LIBRARY_NAME ${LIBRARY_NAME_BASE}${LIBRARY_NAME_SUFFIX2}.dll)
    else()
        set(SHARED_LIBRARY_NAME ${LIBRARY_NAME_PREFIX}${LIBRARY_NAME_BASE}${LIBRARY_NAME_SUFFIX2}.so)
    endif()
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
set(SEARCH_INCLUDE_SUBLOCATIONS SDL2
                                SDL2/include
                                SDL2/build
                                SDL2/build/include)
set(SEARCH_LIBRARY_SUBLOCATIONS SDL2
                                SDL2/lib
                                SDL2/bin
                                SDL2/build
                                SDL2/build/lib
                                SDL2/build/bin)

# find include dir
if (SDL2_INCLUDE_DIR)
    find_path(INCLUDE_DIR
        NAMES SDL2/SDL.h
        PATHS ${SDL2_INCLUDE_DIR}
        PATH_SUFFIXES
            "include"
            ${SEARCH_INCLUDE_SUBLOCATIONS}
        DOC "The SDL2 include directory"
    )
else()
    find_path(INCLUDE_DIR
        NAMES SDL2/SDL.h
        PATHS ${SEARCH_INCLUDE_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_INCLUDE_SUBLOCATIONS}
        DOC "The SDL2 include directory"
    )
endif()

# empty library prefixes
set(CMAKE_FIND_LIBRARY_PREFIXES "")

# find library
if (SDL2_LIBRARY_DIR)
    find_library(LIBRARY
        NAMES ${LIBRARY_NAME}
        PATHS ${SDL2_LIBRARY_DIR}
        PATH_SUFFIXES
            lib
            bin
            ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The SDL2 library path"
    )
else()
    find_library(LIBRARY
        NAMES ${LIBRARY_NAME}
        PATHS ${SEARCH_LIBRARY_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The SDL2 library path"
    )
endif()

# empty library suffixes
set(CMAKE_FIND_LIBRARY_SUFFIXES "")

get_filename_component(LIBRARY_DIR_TEMP ${LIBRARY} DIRECTORY)

# find shared library
if (NOT SDL2_USE_STATIC_LIBS)
    if (SDL2_SHARED_DIR)
        find_library(SHARED_LIBRARY
            NAMES ${SHARED_LIBRARY_NAME}
            PATHS ${SDL2_SHARED_DIR}
            PATH_SUFFIXES
            DOC "The SDL2 shared library path"
        )
    else()        
        find_library(SHARED_LIBRARY
            NAMES ${SHARED_LIBRARY_NAME}
            PATHS
                ${LIBRARY_DIR_TEMP}
                ${LIBRARY_DIR_TEMP}/..
                ${SEARCH_LIBRARY_LOCATIONS}
            PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The SDL2 shared library path"
        )
    endif()
endif()

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${ORIG_SUFFIXES})

# find main library
if (SDL2_MAIN_LIBRARY_DIR)
    find_library(MAIN_LIBRARY
        NAMES ${MAIN_LIBRARY_NAME}
        PATHS ${SDL2_MAIN_LIBRARY_DIR}
        PATH_SUFFIXES
            lib
            bin
            ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The SDL2main library path"
    )
else()
    find_library(MAIN_LIBRARY
        NAMES ${MAIN_LIBRARY_NAME}
        PATHS
            ${LIBRARY_DIR_TEMP}
            ${LIBRARY_DIR_TEMP}/..
            ${SEARCH_LIBRARY_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The SDL2main library path"
    )
endif()

# Restore the original CMAKE_FIND_LIBRARY_PREFIXES
set(CMAKE_FIND_LIBRARY_PREFIXES ${ORIG_PREFIXES})

include(FindPackageHandleStandardArgs)
# let cmake handle erroring if the library components weren't found
if (SDL2_USE_STATIC_LIBS)
    find_package_handle_standard_args(SDL2 DEFAULT_MSG LIBRARY INCLUDE_DIR MAIN_LIBRARY)
else()
    find_package_handle_standard_args(SDL2 DEFAULT_MSG LIBRARY INCLUDE_DIR SHARED_LIBRARY MAIN_LIBRARY)
endif()

if (SDL2_FOUND)
    set(SDL2_INCLUDE_DIRS ${INCLUDE_DIR})
    set(SDL2_LIBRARIES ${MAIN_LIBRARY} ${LIBRARY})
    set(SDL2_SHARED ${SHARED_LIBRARY})
endif()

# unset all of the previous variables we set
unset(LIBRARY CACHE)
unset(MAIN_LIBRARY CACHE)
unset(INCLUDE_DIR CACHE)
unset(SHARED_LIBRARY CACHE)

# Export SDL2::SDL2 as target if it doesn't already exist
if (SDL2_FOUND AND NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
    )
    # When compiling for MinGW, we must also link mingw32.dll for SDL2main to work
    if (MINGW)
        target_link_libraries(SDL2::SDL2
            INTERFACE
                mingw32
        )
    endif()
    target_link_libraries(SDL2::SDL2
        INTERFACE
            ${SDL2_LIBRARIES}
    )

    if (NOT SDL2_USE_STATIC_LIBS)
        add_library(SDL2::SDL2-shared SHARED IMPORTED)
        set_property(TARGET SDL2::SDL2-shared APPEND PROPERTY IMPORTED_LOCATION "${SDL2_SHARED}")
    else()
        # SDL2 static on windows requires these extra system libraries to be linked manually
        if (WIN32)
            target_link_libraries(SDL2::SDL2
                INTERFACE
                    winmm
                    imm32
                    version
            )
        endif()
    endif()
endif()