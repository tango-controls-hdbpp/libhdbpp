# Libhdbpp

Interface library for the HDB++ archiving system. Libhdbpp provides an abstract means to archive data events to either a Cassandra or MySQL database.

### IMPORTANT CHANGES 0.9.1 -> Y.Y

This release version of the library has made a few changes to standardize its use and deployment.
* The build system has been moved to CMake. Details on building it using CMake are below.
* The main tango.h include has been changed from `include <tango.h>` to `include <tango/tango.h>`. This reflects the standard install path of the tango headers in /usr/include/tango and is more consistent with header include patterns. What this means for building the library is we no longer need to specify the tango install directory if its installed in a standard location. If its installed non-standard, then standard CMake flags can used to specify the include and library location (see below)

### Documentation

* See the tango documentation [here](http://tango-controls.readthedocs.io/en/latest/administration/services/hdbpp/index.html#hdb-an-archiving-historian-service) for broader information about the HB++ archiving system.
* Libhdbpp [change log](https://github.com/tango-controls/libhdbpp/CHANGELOG.md)

### Bugs Reports

File bug reports above in the issues section.

## Building and Installation

### Dependencies

Libhdbpp depends on libdl.so, libomniORB4.so and libomnithread.so. If they have not been installed in a standard location, then use standard CMake flags to inform the build where to search for them.

CMake 3.0.0 or greater is required to perform the build.

### Standard flags

The build system is CMake therefore standard CMake flags can be used to influence the build and installation process. Several custom flags are defined to build the correct library. They are:

| Flag | Defaulr | Use |
|------|---------|-----|
| HDBPP_BUILD_SHARED | ON | Build the shared library. This will also be installed if make install is run. |
| HDBPP_BUILD_STATIC | OFF | Build the static library. This will also be installed if make install is run. |
| HDBPP_INSTALL_HEADERS | OFF | On running make install, install the header files. |

The following is a list of common useful CMake flags and their use:

| Flag | Use |
|------|-----|
| CMAKE_INSTALL_PREFIX | Standard CMake flag to modify the install prefix. |
| CMAKE_INCLUDE_PATH | Standard CMake flag to add include paths to the search path. |
| CMAKE_LIBRARY_PATH | Standard CMake flag to add paths to the library search path |

Using the above CMake flags its possible to use tango and other libraries from non-standard locations.

#### Passing CMake Lists

Note: to pass multiple paths (i.e. a string list to cmake), either an escaped semi colon must be used, or the list must be enclosed in quotes. Examples: 

* `-DCMAKE_INCLUDE_PATH=/here/there\;/some/where/else`
* `"-DCMAKE_INCLUDE_PATH=/here/there;/some/where/else"`
* `'-DCMAKE_INCLUDE_PATH=/here/tehre;/some/where/else'`

### Building

Build using CMake version 3.0.0 or greater. An out of source build is required by the CMakeLists file.

#### Example Build Sequence

First clone the repository:

```
git clone http://github.com/tango-controls/libhdbpp.git
```

Create a build directory to run CMake from:

```
mkdir libhdbpp/build
cd libhdbpp/build
```

Then configure with cmake and build:

```
cmake \
    -DCMAKE_INSTALL_PREFIX=/my/custom/location \
    -DCMAKE_INCLUDE_PATH=/path/to/custom/tango/headers \
    -DCMAKE_LIBRARY_PATH=/path/to/custom/libraries \
    ../

make
```

### Installation

After the build has completed, simply run:

```
make install
```

## License

The source code is released under the LGPL3 license and a copy of this license is provided with the code. 
