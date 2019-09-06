# Try to find stb_image.h
# Once done this will define
#  STB_IMAGE_FOUND - if system found stb_image.h
#  STB_IMAGE_INCLUDE_DIRS - The stb_image.h directory

# Force cmake to refind the library when user changes any cmake library location options
unset(STB_IMAGE_INCLUDE_DIR CACHE)
    
if (WIN32)
    find_path(STB_IMAGE_INCLUDE_DIR
		NAMES stb_image.h
		HINTS
			${STB_IMAGE_INCLUDE_PATH}
		PATHS
			${STB_IMAGE_PATH}
            ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            $ENV{PROGRAMFILES}
            "$ENV{PROGRAMFILES\(x86\)}"
        PATH_SUFFIXES
            stb
            stb/include
        DOC "The stb_image.h directory"
	)
else()
    find_path(STB_IMAGE_INCLUDE_DIR
		NAMES stb_image.h
		HINTS
			${STB_IMAGE_INCLUDE_PATH}
        PATHS
			${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
            /usr/include
            /usr/local/include
            /sw/include
            /opt/local/include
        PATH_SUFFIXES
            stb
            stb/include
        DOC "The stb_image.h directory"
	)
endif()

set(STB_IMAGE_INCLUDE_PATH, ${STB_IMAGE_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(STB_IMAGE DEFAULT_MSG STB_IMAGE_INCLUDE_DIR)

if (STB_IMAGE_FOUND)
	set(STB_IMAGE_INCLUDE_DIRS ${STB_IMAGE_INCLUDE_DIR})
	set(STB_IMAGE_DEFINITIONS "-DSTB_IMAGE_IMPLEMENTATION")
endif()

# Export STB_IMAGE::STB_IMAGE as target if it doesn't already exist
if (STB_IMAGE_FOUND AND NOT TARGET STB_IMAGE::STB_IMAGE)
    add_library(STB_IMAGE::STB_IMAGE INTERFACE IMPORTED)
    set_target_properties(STB_IMAGE::STB_IMAGE PROPERTIES
		INTERFACE_INCLUDE_DIRECTORIES "${STB_IMAGE_INCLUDE_DIRS}"
		INTERFACE_COMPILE_DEFINITIONS "STB_IMAGE_IMPLEMENTATION"
    )
endif()