# Try to find SDL2 library
# Once done this will define
#  SDL2_FOUND - if system found SDL2 library
#  SDL2_INCLUDE_DIRS - The SDL2 include directories
#  SDL2_LIBRARIES - The libraries needed to use SDL2
#  SDL2_SHARED - The SDL2 shared library (only set if the shared library is the one being used)

# Force cmake to refind the library when user changes any cmake library location options
unset(SDL2_INCLUDE_DIR CACHE)
unset(SDL2_LIBRARY CACHE)
unset(SDL2_SHARED CACHE)
unset(SDL2_MAIN_LIBRARY CACHE)

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
        set(SDL2_LIBRARY_NAME SDL2)
    endif()
endif()
    
if (WIN32)
    find_path(SDL2_INCLUDE_DIR
		NAMES SDL2/SDL.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            SDL2
            SDL2/include
            SDL2/build
            SDL2/build/include
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

    if (NOT SDL2_IS_STATIC)
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
    endif()
else()
    find_path(SDL2_INCLUDE_DIR
		NAMES SDL2/SDL.h
        PATHS
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        PATH_SUFFIXES
            SDL2
            SDL2/include
            SDL2/build
            SDL2/build/include
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
if (WIN32)
	if (SDL2_IS_STATIC)
		find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_MAIN_LIBRARY SDL2_INCLUDE_DIR)
	else()
		find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_MAIN_LIBRARY SDL2_INCLUDE_DIR SDL2_SHARED)
	endif()
else()
	find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_MAIN_LIBRARY SDL2_INCLUDE_DIR)
endif()

if (SDL2_FOUND)
    set(SDL2_LIBRARIES "${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY}")
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
endif()

# Export SDL2::SDL2 as target if it doesn't already exist
if (SDL2_FOUND AND NOT TARGET SDL2::SDL2)
    add_library(SDL2::SDL2 INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2 PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
    )
    target_link_libraries(SDL2::SDL2
        INTERFACE
            ${SDL2_LIBRARY}
    )

    add_library(SDL2::SDL2main INTERFACE IMPORTED)
    set_target_properties(SDL2::SDL2main PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIRS}"
    )
    # When compiling for MINGW, we must also link mingw32.dll for SDL2main to work
    if (MINGW)
        target_link_libraries(SDL2::SDL2main
            INTERFACE
                mingw32
                ${SDL2_MAIN_LIBRARY}
        )
    endif()
    target_link_libraries(SDL2::SDL2main
        INTERFACE
            ${SDL2_MAIN_LIBRARY}
    )

    if (NOT SDL2_IS_STATIC)
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

# Restore the original CMAKE_FIND_LIBRARY_SUFFIXES
set(CMAKE_FIND_LIBRARY_SUFFIXES ${SDL2_ORIG_SUFFIXES})

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(SDL2_LIBRARY_NAME SDL2_ORIG_SUFFIXES)