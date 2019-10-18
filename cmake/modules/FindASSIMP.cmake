# Try to find ASSIMP library
# Once done this will define
#  ASSIMP_FOUND - if system found ASSIMP library
#  ASSIMP_INCLUDE_DIRS - The ASSIMP include directories
#  ASSIMP_LIBRARIES - The libraries needed to use ASSIMP
#  ASSIMP_SHARED - The ASSIMP shared library (only set if the shared library is the one being used)
# 
# Note: If ASSIMP is being linked statically, then IrrXML and zlibstatic must be found and linked.

# Force cmake to refind the library when user changes any cmake library location options
unset(INCLUDE_DIR CACHE)
unset(LIBRARY CACHE)
unset(SHARED_LIBRARY CACHE)

# set library name base
if (MSVC)
    set(LIBRARY_NAME_BASE assimp-vc140-mt)
else()
    set(LIBRARY_NAME_BASE assimp)
endif()

# set library name prefix
if (MSVC)
    set(LIBRARY_NAME_PREFIX "")
else()
    set(LIBRARY_NAME_PREFIX "lib")
endif()

# set library name suffix1
if (ASSIMP_USE_STATIC_LIBS AND MSVC)
    set(LIBRARY_NAME_SUFFIX1 "-static")
else()
    set(LIBRARY_NAME_SUFFIX1 "")
endif()

# set library name suffix2
if (WIN32 AND MSVC AND ASSIMP_USE_DEBUG)
    # debug version has a _d suffix
    set(LIBRARY_NAME_SUFFIX2 "_d")
else()
    set(LIBRARY_NAME_SUFFIX2 "")
endif()

# set library extra extension
if (WIN32 AND (NOT MSVC) AND (NOT ASSIMP_USE_STATIC_LIBS))
    set(LIBRARY_EXTENSION ".dll")
else()
    set(LIBRARY_EXTENSION "")
endif()

# set find library suffixes
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
if (ASSIMP_USE_STATIC_LIBS)
    set(IRRXML_LIBRARY_NAME ${LIBRARY_NAME_PREFIX}IrrXML)
    if (ASSIMP_USE_DEBUG)
        set(ZLIB_LIBRARY_NAME ${LIBRARY_NAME_PREFIX}zlibstaticd)
    else()
        set(ZLIB_LIBRARY_NAME ${LIBRARY_NAME_PREFIX}zlibstatic)
    endif()
else()
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
set(SEARCH_INCLUDE_SUBLOCATIONS assimp
                                assimp/include
                                assimp/build
                                assimp/build/include)
set(SEARCH_LIBRARY_SUBLOCATIONS assimp
                                assimp/lib
                                assimp/bin
                                assimp/build
                                assimp/build/lib
                                assimp/build/bin)

# find include dir
if (ASSIMP_INCLUDE_DIR)
    find_path(INCLUDE_DIR
        NAMES assimp/Importer.hpp
        PATHS ${ASSIMP_INCLUDE_DIR}
        PATH_SUFFIXES
            "include"
            ${SEARCH_INCLUDE_SUBLOCATIONS}
        DOC "The ASSIMP include directory"
    )
else()
    find_path(INCLUDE_DIR
        NAMES assimp/Importer.hpp
        PATHS ${SEARCH_INCLUDE_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_INCLUDE_SUBLOCATIONS}
        DOC "The ASSIMP include directory"
    )
endif()

# find library
if (ASSIMP_LIBRARY_DIR)
    find_library(LIBRARY
        NAMES ${LIBRARY_NAME}
        PATHS ${ASSIMP_LIBRARY_DIR}
        PATH_SUFFIXES
            lib
            bin
            ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The ASSIMP library path"
    )
else()
    find_library(LIBRARY
        NAMES ${LIBRARY_NAME}
        PATHS ${SEARCH_LIBRARY_LOCATIONS}
        PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
        DOC "The ASSIMP library path"
    )
endif()

# empty library suffixes
set(CMAKE_FIND_LIBRARY_SUFFIXES "")

get_filename_component(LIBRARY_DIR_TEMP ${LIBRARY} DIRECTORY)

# find shared library
if (NOT ASSIMP_USE_STATIC_LIBS)
    if (ASSIMP_SHARED_DIR)
        find_library(SHARED_LIBRARY
            NAMES ${SHARED_LIBRARY_NAME}
            PATHS ${ASSIMP_SHARED_DIR}
            PATH_SUFFIXES
            DOC "The ASSIMP shared library path"
        )
    else()        
        find_library(SHARED_LIBRARY
            NAMES ${SHARED_LIBRARY_NAME}
            PATHS
                ${LIBRARY_DIR_TEMP}
                ${LIBRARY_DIR_TEMP}/..
                ${SEARCH_LIBRARY_LOCATIONS}
            PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The ASSIMP shared library path"
        )
    endif()
endif()

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${ORIG_SUFFIXES})

# find IrrXML library
if (ASSIMP_USE_STATIC_LIBS)
    if (ASSIMP_IRRXML_LIBRARY_DIR)
        find_library(IRRXML_LIBRARY
            NAMES ${IRRXML_LIBRARY_NAME}
            PATHS ${ASSIMP_IRRXML_LIBRARY_DIR}
            PATH_SUFFIXES
                lib
                bin
                ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The ASSIMP IrrXML library path"
        )
    else()
        find_library(IRRXML_LIBRARY
            NAMES ${IRRXML_LIBRARY_NAME}
            PATHS
                ${LIBRARY_DIR_TEMP}
                ${LIBRARY_DIR_TEMP}/..
                ${SEARCH_LIBRARY_LOCATIONS}
            PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The ASSIMP IrrXML library path"
        )
    endif()
endif()

include(FindPackageHandleStandardArgs)

# let cmake handle erroring if the library components weren't found
if (ASSIMP_USE_STATIC_LIBS)
    find_package_handle_standard_args(ASSIMP DEFAULT_MSG LIBRARY INCLUDE_DIR IRRXML_LIBRARY)
else()
    find_package_handle_standard_args(ASSIMP DEFAULT_MSG LIBRARY INCLUDE_DIR SHARED_LIBRARY)
endif()

# find zlib library
if (ASSIMP_USE_STATIC_LIBS)
    if (ZLIB_LIBRARY_DIR)
        find_library(ZLIB_LIBRARY
            NAMES ${ZLIB_LIBRARY_NAME}
            PATHS ${ZLIB_LIBRARY_DIR}
            PATH_SUFFIXES
                lib
                bin
                ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The zlib library path"
        )
    else()
        find_library(ZLIB_LIBRARY
            NAMES ${ZLIB_LIBRARY_NAME}
            PATHS
                ${LIBRARY_DIR_TEMP}
                ${LIBRARY_DIR_TEMP}/..
                ${SEARCH_LIBRARY_LOCATIONS}
            PATH_SUFFIXES ${SEARCH_LIBRARY_SUBLOCATIONS}
            DOC "The zlib library path"
        )

        if (NOT ZLIB_LIBRARY)
            find_package(ZLIB QUIET)
        endif()
    endif()

    # let cmake handle erroring if zlib wasn't found
    find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_LIBRARY)
endif()

if (ASSIMP_FOUND)
    set(ASSIMP_INCLUDE_DIRS ${INCLUDE_DIR})
    if (ASSIMP_USE_STATIC_LIBS)
        set(ASSIMP_LIBRARIES ${LIBRARY} ${IRRXML_LIBRARY} ${ZLIB_LIBRARY})
    else()
        set(ASSIMP_LIBRARIES ${LIBRARY})
    endif()
    set(ASSIMP_SHARED ${SHARED_LIBRARY})
endif()

# unset all of the previous variables we set
unset(LIBRARY CACHE)
unset(IRRXML_LIBRARY CACHE)
unset(ZLIB_LIBRARY CACHE)
unset(INCLUDE_DIR CACHE)
unset(SHARED_LIBRARY CACHE)

# Export ASSIMP::ASSIMP as target if it doesn't already exist
if (ASSIMP_FOUND AND NOT TARGET ASSIMP::ASSIMP)
    add_library(ASSIMP::ASSIMP INTERFACE IMPORTED)
    set_target_properties(ASSIMP::ASSIMP PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIRS}"
    )
    target_link_libraries(ASSIMP::ASSIMP
        INTERFACE
            ${ASSIMP_LIBRARIES}
    )

    if (NOT ASSIMP_IS_STATIC)
        add_library(ASSIMP::ASSIMP-shared SHARED IMPORTED)
        set_property(TARGET ASSIMP::ASSIMP-shared APPEND PROPERTY IMPORTED_LOCATION "${ASSIMP_SHARED}")
    endif()
endif()