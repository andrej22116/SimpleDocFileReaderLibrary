# SDRL (SimpleDocFileReaderLibrary)
Library with simple C++ interface for read MS Word 97-03 documents (.doc)
## In active development!

# Installation

## Windows
Requirements:
* Visual Studio 2013 - 2017
* CMake

Run
>.\build.bat

## Linux
Requirements:
* g++
* make
* CMake

Run
>sh ./build.sh

## As a depenency for another project

In your CMakeLists.txt, add:
>add_subdirectory(PATH_TO_THIS_LIBRARY)
>
>target_link_libraries(YOUR_PROJECT SDRL)