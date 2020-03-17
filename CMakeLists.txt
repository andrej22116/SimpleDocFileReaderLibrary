cmake_minimum_required (VERSION 2.8.12)

project("SDRL")

set(SDRL_INCLUDE "include/")

option(BUILD_EXAMPLE "BUILD_EXAMPLE" OFF)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SDRLsrc #Select all files in src/
    "src/*.cpp"
	"src/DocDocument/*.cpp"
	"src/DocDocument/WordBinaryFileFormat/*.cpp"
	"src/DocDocument/WordBinaryFileStructures/*.cpp"
)

add_library(SDRL ${SDRLsrc}) # Create library

target_include_directories(SDRL PUBLIC ${SDRL_INCLUDE})

if (BUILD_EXAMPLE)

    #Main.cpp
    add_executable(SDRLmain "src/examples/main.cpp")
    target_link_libraries(SDRLmain SDRL)
    
endif()