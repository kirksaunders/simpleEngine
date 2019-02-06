# Try to find ASSIMP library
# Once done this will define
#  ASSIMP_FOUND - if system found ASSIMP library
#  ASSIMP_INCLUDE_DIRS - The ASSIMP include directories
#  ASSIMP_LIBRARIES - The libraries needed to use ASSIMP
#  ASSIMP_SHARED - The ASSIMP shared library (only set if the shared library is the one being used)

# Define additional compiler definitions and handle different libraries
set(ASSIMP_ORIG_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if (ASSIMP_USE_STATIC_LIBS)
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a)
        set(ASSIMP_LIBRARY_NAME assimp)
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
        set(ASSIMP_LIBRARY_NAME assimp)
    endif()

    set(ASSIMP_IS_STATIC TRUE)
else()
    if (WIN32)
        set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a .dll)
        set(ASSIMP_LIBRARY_NAME assimp.dll)
    else()
        set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
        set(ASSIMP_LIBRARY_NAME assimp)
    endif()
endif()

# ASSIMP static depends on zlib
if (ASSIMP_IS_STATIC)
    find_package(ZLIB QUIET)

    if (ZLIB_LIBRARY)
        message(STATUS "Found ZLIB: ${ZLIB_LIBRARY}")
    else()
        message(SEND_ERROR "Could NOT find ZLIB (missing: ZLIB_LIRBARY)")
    endif()
endif()
    
if (WIN32)
    find_path(ASSIMP_INCLUDE_DIR
		NAMES assimp/Importer.hpp
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            assimp
            assimp/include
            assimp/build
            assimp/build/include
        DOC "The ASSIMP include directory"
    )

    find_library(ASSIMP_LIBRARY
		NAMES ${ASSIMP_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            assimp
            assimp/lib
            assimp/bin
            assimp/build
            assimp/build/lib
            assimp/build/bin
            assimp/build/code
        DOC "The ASSIMP library path"
    )

    set(CMAKE_FIND_LIBRARY_SUFFIXES "")

    if (NOT ASSIMP_IS_STATIC)
        find_library(ASSIMP_SHARED
			NAMES ${ASSIMP_LIBRARY_NAME}
            PATHS
                ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
                $ENV{PROGRAMFILES}
                "$ENV{PROGRAMFILES\(x86\)}"
            PATH_SUFFIXES
                assimp
                assimp/lib
                assimp/bin
                assimp/build
                assimp/build/lib
                assimp/build/bin
                assimp/build/code
            DOC "The ASSIMP shared library path"
        )
    endif()
else()
    find_path(ASSIMP_INCLUDE_DIR
		NAMES assimp/Importer.hpp
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        PATH_SUFFIXES
            assimp
            assimp/include
            assimp/build
            assimp/build/include
        DOC "The ASSIMP include directory"
    )

    find_library(ASSIMP_LIBRARY
		NAMES ${ASSIMP_LIBRARY_NAME}
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/lib64
            /usr/lib
            /usr/local/lib64
            /usr/local/lib
            /sw/lib
            /opt/local/lib
        PATH_SUFFIXES
            assimp
            assimp/lib
            assimp/bin
            assimp/build
            assimp/build/lib
            assimp/build/bin
            assimp/build/code
        DOC "The ASSIMP library path"
    )

    if (NOT ASSIMP_IS_STATIC)
        set(ASSIMP_SHARED ${ASSIMP_LIBRARY})
    endif()
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
if (WIN32)
	if (ASSIMP_IS_STATIC)
		find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR ZLIB_LIBRARY)
	else()
		find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR ASSIMP_SHARED)
	endif()
else()
    if (ASSIMP_IS_STATIC)
        find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR ZLIB_LIBRARY)
    else()
        find_package_handle_standard_args(ASSIMP DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)
    endif()
endif()

if (ASSIMP_FOUND)
    set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
    set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
endif()

# Export ASSIMP::ASSIMP as target if it doesn't already exist
if (ASSIMP_FOUND AND NOT TARGET ASSIMP::ASSIMP)
    add_library(ASSIMP::ASSIMP INTERFACE IMPORTED)
    set_target_properties(ASSIMP::ASSIMP PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIRS}"
    )
    target_link_libraries(ASSIMP::ASSIMP
        INTERFACE
            ${ASSIMP_LIBRARY}
    )

    if (NOT ASSIMP_IS_STATIC)
        add_library(ASSIMP::ASSIMP-shared SHARED IMPORTED)
        set_property(TARGET ASSIMP::ASSIMP-shared APPEND PROPERTY IMPORTED_LOCATION "${ASSIMP_SHARED}")
    else()
        # ASSIMP static on windows requires these extra system libraries to be linked manually
        target_link_libraries(ASSIMP::ASSIMP
            INTERFACE
                ${ZLIB_LIBRARY}
                IrrXML
        )
    endif()
endif()

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${ASSIMP_ORIG_SUFFIXES})

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY ASSIMP_LIBRARY_NAME ASSIMP_SHARED ASSIMP_ORIG_SUFFIXES)