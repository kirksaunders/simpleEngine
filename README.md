# simpleEngine
***WARNING***: Very incomplete and in progress. The API (both the internal API and the external API) is highly volatile and I will change it massively anytime I see fit (this is a personal, educational project afterall).

The purpose of this project is to provide an easy-to-use, cross-platform library that is capable of rendering 3D objects with simple and powerful code. simpleEngine is built on top of OpenGL and aims to provide easy-to-use APIs while still providing powerful and fast 3D rendering capabilities. This is being developed for fun and is the first program I have written that uses OpenGL. Therefore, it is more of a learning experience, but I hope to make this engine useful for projects to come.

## Dependencies (All cross-platform)
- **ASSIMP**(Open Asset Importer Library) - Used to parse many different 3D object file formats
- **GLEW**(GL Extension Wrangler) - Loads in all of the required OpenGL libraries for use
- **SDL**(Simple DirectMedia Layer) - Windowing and input library with OpenGL support
- **stb_image**(Sean Barrett's Image Loading Library) - Single file, header-only C library for loading images

## Build Instructions
See README in build/ for instructions on building with CMake. I don't recommend using the Makefile.old in the project root to build. It is no longer maintained, so there is no guarantee that it works. Furthermore it was only designed for GNU Make/MSYS Make in the first place, so it doesn't have the same reach as the CMake configs.