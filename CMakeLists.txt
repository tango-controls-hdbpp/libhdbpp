# Stop messy in source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

# Start Build Config -----------------------------------
cmake_minimum_required(VERSION 3.6)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

# Library output name for the final binaries
set(LIBHDBPP_NAME "libhdb++")

# Versioning
set(VERSION_MAJOR "2")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION_METADATA "")
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Add any include paths from the command line
list(APPEND INCLUDE_PATHS ${CMAKE_INCLUDE_PATH})
list(APPEND INCLUDE_PATHS ${CMAKE_SOURCE_DIR})
list(APPEND LIBRARY_PATHS ${CMAKE_LIBRARY_PATH})

# Start the project
project(libhdbpp VERSION ${VERSION_STRING} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_CLANG "Enable clang code and layout analysis" OFF)

# arch install definitions
include(GNUInstallDirs)

message(STATUS "Searching for libraries...")

# Variable to contain a list of all the libs we depend on
set(HDBPP_LIBRARIES dl) 

# allow pkg-config to search the CMAKE_PREFIX_PATH 
set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
list(APPEND CMAKE_PREFIX_PATH "/usr")

# Find Dependencies  ---------------------
include(cmake/FindLibraries.cmake)

# Attempt to find the various libraries the project is dependent on
if(HDBPP_LIBRARIES)
    find_libraries(LIBRARIES ${HDBPP_LIBRARIES} SEARCH_PATHS ${LIBRARY_PATHS})
    set(HDBPP_FOUND_LIBRARIES ${FOUND_LIBRARIES})
endif(HDBPP_LIBRARIES)

# First find tango if it has not already been found. Returns an interface library
# called TangoInterfaceLibrary
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Tango)

# Code Analysis -----------------------------------
if(ENABLE_CLANG)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # To find clang, find_program will search your PATH environment variable.
    # Ensure if you have a non-standard clang install, that it has been added
    # to your path.
    find_program(CLANG_TIDY_EXE 
        NAMES "clang-tidy" 
        DOC "Path to clang-tidy executable")

    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif(NOT CLANG_TIDY_EXE)
endif(ENABLE_CLANG)

# Export target name for subdirectory build
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(BACKEND_TARGET_SHARED "libhdbpp_shared_library"
        PARENT_SCOPE)
    set(BACKEND_TARGET_STATIC "libhdbpp_static_library"
        PARENT_SCOPE)
endif(hasParent)
set(BACKEND_TARGET_SHARED "libhdbpp_shared_library")
set(BACKEND_TARGET_STATIC "libhdbpp_static_library")

# Source -----------------------------------

# Create a list of the source files for this build to be given to
# the target later
set(LIBHDBPP_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/HdbClient.cpp)

# Build Targets  -----------------------------------

# Libhdbpp header library --------
add_library(libhdbpp_headers INTERFACE)

target_include_directories(libhdbpp_headers 
    INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

# Shared library --------
add_library(${BACKEND_TARGET_SHARED} SHARED ${LIBHDBPP_SRC_FILES})

target_link_libraries(${BACKEND_TARGET_SHARED} 
    PUBLIC ${HDBPP_FOUND_LIBRARIES} 
    PRIVATE TangoInterfaceLibrary)

target_include_directories(${BACKEND_TARGET_SHARED} 
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        ${INCLUDE_PATHS}
        "${PROJECT_BINARY_DIR}")

set_target_properties(${BACKEND_TARGET_SHARED} 
    PROPERTIES 
        OUTPUT_NAME hdb++
        LINK_FLAGS "-Wl,--no-undefined"
        CXX_STANDARD 11
        POSITION_INDEPENDENT_CODE 1
        VERSION ${VERSION_STRING}
        SOVERSION ${VERSION_MAJOR})

target_compile_options(${BACKEND_TARGET_SHARED} 
    PRIVATE "$<$<CONFIG:DEBUG>:-g>")

if(DO_CLANG_TIDY)
    set_target_properties(${BACKEND_TARGET_SHARED} 
        PROPERTIES 
            CXX_CLANG_TIDY ${DO_CLANG_TIDY})
endif(DO_CLANG_TIDY)

# Static library --------
add_library(${BACKEND_TARGET_STATIC} STATIC EXCLUDE_FROM_ALL ${LIBHDBPP_SRC_FILES})

target_include_directories(${BACKEND_TARGET_STATIC} 
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        "${PROJECT_BINARY_DIR}")

target_link_libraries(${BACKEND_TARGET_STATIC} 
    PUBLIC ${HDBPP_FOUND_LIBRARIES} 
    PRIVATE TangoInterfaceLibrary)

set_target_properties(${BACKEND_TARGET_STATIC} 
    PROPERTIES 
        OUTPUT_NAME hdb++
        LINK_FLAGS "-Wl,--no-undefined"
        CXX_STANDARD 11
        EXCLUDE_FROM_ALL 1)

target_compile_options(${BACKEND_TARGET_STATIC} 
    PRIVATE "$<$<CONFIG:DEBUG>:-g>")

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN *.h)

# install and export the library

install(TARGETS
    libhdbpp_headers
    ${BACKEND_TARGET_SHARED}
  EXPORT libhdbppTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)


set(ConfigPackageLocation lib/cmake/libhdbpp)
set(Namespace libhdbpp::)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp/libhdbppConfigVersion.cmake"
  VERSION ${VERSION_STRING}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT libhdbppTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp/libhdbppTargets.cmake"
  NAMESPACE ${Namespace}
)

configure_package_config_file(cmake/libhdbppConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp/libhdbppConfig.cmake"
  INSTALL_DESTINATION
  ${ConfigPackageLocation}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(EXPORT libhdbppTargets
  FILE
    libhdbppTargets.cmake
  NAMESPACE
    ${Namespace}
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp/libhdbppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp/libhdbppConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
) 

message(STATUS "Configured libhdbpp project")
