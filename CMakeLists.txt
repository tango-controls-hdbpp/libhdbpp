# Stop messy in source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

# Start Build Config -----------------------------------
cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

# To build both the static and shared version, we require two projects, we do
# this by just defining the name appended with the target
project(shared_library)
project(static_library)

# We only support Release for now
set(CMAKE_BUILD_TYPE "Release")

# Project version
set(LIBHDBPP_VERSION_MAJOR "0")
set(LIBHDBPP_VERSION_MINOR "10")
set(LIBHDBPP_VERSION_REVISION "0")
string(TIMESTAMP LIBHDBPP_TIMESTAMP "%Y-%m-%d %H:%M:%S")

# Version the shared library
set(LIBRARY_VERSION_MAJOR 6)
set(LIBRARY_VERSION_MINOR 0)
set(LIBRARY_VERSION_PATCH 0)

set(LIBRARY_VERSION_STRING 
    ${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH})

# Check any custom flags
# Check any custom flags
option(HDBPP_BUILD_SHARED "Build shared library" ON)
option(HDBPP_BUILD_STATIC "Build static library" OFF)
option(HDBPP_INSTALL_HEADERS "Install header files on install" OFF)

# Debug the flags
if(NOT HDBPP_BUILD_SHARED AND NOT HDBPP_BUILD_STATIC)
    message(FATAL "Must select to build static and/or shared library")
endif(NOT HDBPP_BUILD_SHARED AND NOT HDBPP_BUILD_STATIC)

if(HDBPP_INSTALL_HEADERS)
    message(STATUS "Header install requested")
endif(HDBPP_INSTALL_HEADERS)

# arch install definitions
include(GNUInstallDirs)

# Create a list of the source files for this build to be given to
# the target later
set(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/LibHdb++.cpp)

# Set the output directory to lib to stay consistent with the old build
set(OUTPUT_DIR "lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/${OUTPUT_DIR})

# set up a configuration file to pass variables into the build
configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/LibHdb++Config.h.in"
    "${PROJECT_BINARY_DIR}/LibHdb++Config.h")

message(STATUS "Building include paths...")

# Add any include paths from the command line
set(INCLUDE_PATHS ${INCLUDE_PATHS} ${CMAKE_INCLUDE_PATH})

foreach(INC_PATH ${CMAKE_INCLUDE_PATH})
    message(STATUS "Extra include path: " ${INC_PATH})
endforeach()

message(STATUS "Building library paths...")

foreach(LIB_PATH ${CMAKE_LIBRARY_PATH})
    message(STATUS "Extra library search path: " ${LIB_PATH})
endforeach()

message(STATUS "Searching for libraries...")

# Variable to contain a list of all the libs we depend on
set(LIBRARIES 
    libdl.so
    libomniORB4.so
    libomnithread.so)

# Now load the libraries and check each is found, if not
# this is an error condition, so halt the cmake config
foreach(LIB ${LIBRARIES})
    # try the user provided paths first
    find_library(FOUND_LIB_${LIB} ${LIB} PATHS ${LIBRARY_PATHS} NO_DEFAULT_PATH)

    # if we could not find it, drop to the system paths
    if (NOT FOUND_LIB_${LIB})
        find_library(FOUND_LIB_${LIB} ${LIB})
    endif()

    list(APPEND LIBS_DIR ${FOUND_LIB_${LIB}})

    if (FOUND_LIB_${LIB})
        message(STATUS "Found " ${LIB} " at: " ${FOUND_LIB_${LIB}})
    else()
        message(FATAL_ERROR "Could not find " ${LIB})
    endif()
endforeach(LIB)

# This is the "object library" target: compiles the sources only once
# for both the static and shared library
add_library(object_library OBJECT ${SRC_FILES})
set_property(TARGET object_library PROPERTY POSITION_INDEPENDENT_CODE 1)

target_compile_options(object_library 
    PUBLIC -std=c++11 -Wall -Wextra)

target_include_directories(object_library 
    PUBLIC ${INCLUDE_PATHS}
    PRIVATE "${PROJECT_BINARY_DIR}")

if(HDBPP_BUILD_SHARED)
    add_library(shared_library SHARED $<TARGET_OBJECTS:object_library>)
    target_link_libraries(shared_library PUBLIC ${LIBS_DIR})

    set_target_properties(shared_library PROPERTIES 
        OUTPUT_NAME hdb++
        LINK_FLAGS "-Wl,--no-undefined")

    # Setting the target includes will allow us to link to this shared
    # library via the cmake project and not require us to include all
    # its depenencies
    target_include_directories(shared_library 
        PUBLIC ${INCLUDE_PATHS}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src
        PUBLIC "${PROJECT_BINARY_DIR}")

    # This will version the shared library
    set_target_properties(shared_library
        PROPERTIES VERSION ${LIBRARY_VERSION_STRING} 
        SOVERSION ${LIBRARY_VERSION_MAJOR})
endif(HDBPP_BUILD_SHARED)

if(HDBPP_BUILD_STATIC)
    add_library(static_library STATIC $<TARGET_OBJECTS:object_library>)
    target_link_libraries(static_library PUBLIC ${LIBS_DIR})

    # Ensure the output is a library.a
    set_target_properties(static_library PROPERTIES 
        OUTPUT_NAME hdb++
        LINK_FLAGS "-Wl,--no-undefined")

endif(HDBPP_BUILD_STATIC)

# Install Config -----------------------------------
if(HDBPP_BUILD_SHARED)
    install(TARGETS shared_library LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(HDBPP_BUILD_SHARED)

if(HDBPP_BUILD_STATIC)
    install(TARGETS static_library ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(HDBPP_BUILD_STATIC)

if(HDBPP_INSTALL_HEADERS)
    install(FILES src/LibHdb++.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libhdb++)
endif(HDBPP_INSTALL_HEADERS)
