# CMake Build Details
It is recommended that you run cmake as an "out-of-source build", which means you don't build in the project root. The purpose of this directory is to serve as the cmake build directory.

## CMake from Command Line
To run cmake from command line, use the general format:
```
cd ./build/  # cd into this build directory
cmake [OPTIONS] ..  # run cmake to generate your build system files (Makefile, Ninja, Visual Studio Project, etc)
```
One of the options you should set (if you're new to CMake) is the desired build system, using the `-G` option. For example, generating Makefiles for MinGW is done with `cmake -G"MinGW Makefiles"`.
CMake options (shown below) can be set using: `-D[OPTION_NAME]:[OPTION_TYPE]=[VALUE]`

## CMake GUI
CMake also provides a GUI on many systems. To use it, pick the project root directory for the "Where is the source code" option and this build directory for the "Where to build the binaries" option. Any CMake options can be set here before configuring by clicking "Add Entry" and naming the option accordingly. See below for available options.

## After Build File Generation
Once you have generated the build files, you then use them with your build system to build the project. For example, using make (assuming you used cmake to generate makefiles), you run the command `make` in this build directory. Additionally, cmake generates a "clean" target, so you could run `make clean` to clean up any previous build that were generated from running `make`.

## Available CMake Options
This list contains CMake options that can be set to control certain properties of the project build.

- **COPY_3RDPARTY_DLLS (*BOOL*)** - Set this to *TRUE* if you want the 3rd party dlls (assimp.dll, SDL2.dll, etc) to be copied to the project executable bin directory. This is useful so that the program has access to them when run. This option only copies the dlls of libraries that are being linked dynamically (see below options for more info on that). These dlls will be removed by `make clean` along with the executable.

- **[DEPENDENCY_NAME]_USE_STATIC_LIBS (*BOOL*)** - Set this to *TRUE* if you want to link against [DEPENDENCY_NAME]'s static library instead of the shared one. If this is not set, the shared library will be linked. [DEPENDENCY_NAME] = ASSIMP, GLEW, or SDL2 (just replace [DEPENDENCY_NAME] with the library name as listed here)

    **Note:** The CMakeCache file (if you have run the CMake config already) must be deleted if one of these options are changed. This is because CMake likes to cache the locations of the libraries, and they won't be updated by your option change unless you delete the cache.

- **[DEPENDENCY_NAME]_INCLUDE_DIR (*PATH*)** - To manually specify the path to [DEPENDENCY_NAME]'s include files on your system, set this option to the path containing the [DEPENDENCY_NAME] include directory. [DEPENDENCY_NAME] = ASSIMP, GLEW, SDL2, or STB_IMAGE (just replace [DEPENDENCY_NAME] with the library name as listed here)

- **[DEPENDENCY_NAME]_LIBRARY (*FILEPATH*)** - To manually specify the path to [DEPENDENCY_NAME]'s library file on your system, set this option to the path of the [DEPENDENCY_NAME] library file.

	**Note:** If you are on Windows and linking this library as shared, this should be set to the `.dll.a` or .`dll.lib` library file, not the `.dll` file. See below for the `.dll` file. [DEPENDENCY_NAME] = ASSIMP, GLEW, or SDL2 (just replace [DEPENDENCY_NAME] with the library name as listed here)

- **SDL2_MAIN_LIBRARY (*FILEPATH*)** - SDL2 requires an additional *static* library in order to link properly. This is the SDL2-main library and this option should be set as the path of that static library file (should be SDL2-main.a or SDL2-main.lib).

### Windows Specific Options
- **[DEPENDENCY_NAME]_SHARED (*FILEPATH*)** - To manually specify the path to [DEPENDENCY_NAME]'s *shared* library file (`.dll`) on your system, set this option to the path of the [DEPENDENCY_NAME] *shared* library file.
	
	**Note:** This option is only required if [DEPENDENCY_NAME]_USE_STATIC_LIBS is set to *FALSE*. [DEPENDENCY_NAME] = ASSIMP, GLEW, or SDL2 (just replace [DEPENDENCY_NAME] with the library name as listed here)