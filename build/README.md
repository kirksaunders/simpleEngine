#CMake Build Details
It is recommended that you run cmake as an "out-of-source build", which means you don't build in the project root. The purpose of this directory is to serve as the cmake build directory.
##CMake from Command Line
To run cmake from command line, use the general format:
```
cd ./build/  # cd into this build directory
cmake [OPTIONS] ..  # run cmake to generate your build system files (Makefile, Ninja, Visual Studio Project, etc)
```
One of the options you should set (if you're new to CMake) is the desired build system, using the `-G` option. For example, generating Makefiles for MinGW is done with `cmake -G"MinGW Makefiles"`.
CMake options (shown below) can be set using: `-D[OPTION_NAME]:[OPTION_TYPE]=[VALUE]`
##CMake GUI
CMake also provides a GUI on many systems. To use it, pick the project root directory for the "Where is the source code" option and this build directory for the "Where to build the binaries" option. Any CMake options can be set here before configuring by clicking "Add Entry" and naming the variable accordingly. See below for available options.
##After Build File Generation
Once you have generated the build files, you then use them with your build system to build the project. For example, using make (assuming you used cmake to generate makefiles), you run the command `make` in this build directory. Additionally, cmake generates a "clean" target, so you could run `make clean` to clean up any previous build that were generated from running `make`.
##Available CMake Options
This list contains CMake options that can be set to control certain properties of the project build.
- **COPY_3RDPARTY_DLLS (*BOOL*)** - Set this to *TRUE* if you want the 3rd party dlls (assimp.dll, SDL2.dll, etc) to be copied to the project executable bin directory. This is useful so that the program has access to them when run. This option only copies the dlls of libraries that are being linked dynamically (see below options for more info on that). These dlls will be removed by `make clean` along with the executable.
- **XXX_USE_STATIC_LIBS (*BOOL*)** - Set this to *TRUE* if you want to link against XXX's static library instead of the shared one. If this is not set, the shared library will be linked. The libraries that this option applies to are ASSIMP, GLEW, SDL2, GLEW (just replace XXX with the library name as listed here).