# Building and Installation

## Dependencies

Libhdbpp depends on libdl.so, libomniORB4.so and libomnithread.so. If they have not been installed in a standard location, then use standard CMake flags below to inform the build where to search for them.

It is dependent on a Tango Controls install via debian package or source distribution. Supported Tango Controls release is 9.2.5a.

CMake 3.0.0 or greater is required to perform the build.

## Standard flags

The build system is CMake therefore standard CMake flags can be used to influence the build and installation process. Several custom flags are defined to build the correct library. They are:

| Flag | Default | Use |
|------|---------|-----|
| HDBPP_BUILD_SHARED | ON | Build the shared library. This will also be installed if make install is run. |
| HDBPP_BUILD_STATIC | OFF | Build the static library. This will also be installed if make install is run. |
| HDBPP_DEV_INSTALL | OFF | Install development files and libraries |

The following is a list of common useful CMake flags and their use:

| Flag | Use |
|------|-----|
| CMAKE_INSTALL_PREFIX | Standard CMake flag to modify the install prefix. |
| CMAKE_INCLUDE_PATH | Standard CMake flag to add include paths to the search path. |
| CMAKE_LIBRARY_PATH | Standard CMake flag to add paths to the library search path |

Using the above CMake flags above its possible to use tango and other libraries from non-standard locations. Just add all paths to the correct flag.

### Passing CMake Lists

Note: to pass multiple paths (i.e. a string list to cmake), either an escaped semi colon must be used, or the list must be enclosed in quotes. Examples: 

* `-DCMAKE_INCLUDE_PATH=/here/there\;/some/where/else`
* `"-DCMAKE_INCLUDE_PATH=/here/there;/some/where/else"`
* `'-DCMAKE_INCLUDE_PATH=/here/tehre;/some/where/else'`

## Building

Build using CMake version 3.0.0 or greater. An out of source build is required by the CMakeLists file.

### **Important Note:** - Building Against Standard Tango Controls 9.2.5a

Tango Controls 9.2.5a as source distribution or debian package on Debian Stretch installs the header files into `/usr/include/tango`, but `tango.h` includes tango headers (example `tango_config.h`) from  `/usr/include`. This means the build fails. To work around this until the next release of Tango Controls (which fixes this problem) add `/usr/include/tango` to the CMAKE_INCLUDE_PATH, eg:

```bash
cmake -DCMAKE_INCLUDE_PATH=/usr/include/tango ..
```

### Example Build Sequence

First clone the repository:

```bash
git clone http://github.com/tango-controls/libhdbpp.git
```

Create a build directory to run CMake from:

```bash
mkdir libhdbpp/build
cd libhdbpp/build
```

Then configure with cmake:

```bash
cmake ..
```

Or for something with non-standard dependencies:

```bash
cmake \
    -DCMAKE_INSTALL_PREFIX=/my/custom/location \
    -DCMAKE_INCLUDE_PATH=/path/to/custom/include \
    -DCMAKE_LIBRARY_PATH=/path/to/custom/library \
    ..
```

Then build:

```bash
make
```

## Installation

After the build has completed, simply run:

```
make install
```