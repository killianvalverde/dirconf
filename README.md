### dirconf ###

dirconf applies directory configuration files by editing the GNOME Virtual File System directories 
metadata. JSON files are used to specify the configuration in the directories. Currently, it is 
only implemented for nemo. For more information about how to write the configuration files and use 
the program, use the `dirconf --help` command.

### Build ###

Use the folowing commands to buil and install the CMake project.

    Create a directory to hold the build output and generate the native build scripts:
            $ cmake -H. -Bbuild

    Compile the project directly from CMake using the native build scripts:
            $ cmake --build build

    Install the binary in your environment:
            $ sudo cmake --install build

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Also the following tools and libraries are necessary with a version equal or 
highter the specified one:
- GCC 8.3.0 (sudo apt-get install g++)
- CMake 3.15 (http://www.cmake.org/)
- speed (https://github.com/killianValverde/speed)
- boost 1.58 (sudo apt-get install libboost-dev)
- libglib 2.0 (sudo apt-get install libglib2.0-dev)
- libgio 2.0 (sudo apt-get install libgio2.0-cil-dev)
