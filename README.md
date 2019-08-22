# Libhdbpp

[![TangoControls](https://img.shields.io/badge/-Tango--Controls-7ABB45.svg?style=flat&logo=%20data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAACAAAAAkCAYAAADo6zjiAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAALEwAACxMBAJqcGAAAAsFJREFUWIXtl01IFVEYht9zU%2FvTqOxShLowlOgHykWUGEjUKqiocB1FQURB0KJaRdGiaFM7gzZRLWpTq2olhNQyCtpYCP1gNyIoUTFNnxZzRs8dzvw4Q6564XLnfOf73vedc2a%2BmZEKALgHrC3CUUR8CxZFeEoFalsdM4uLmMgFoIlZLJp3A9ZE4S2oKehhlaR1BTnyg2ocnW%2FxsxEDhbYij4EPVncaeASMAavnS%2FwA8NMaqACNQCew3f4as3KZOYh2SuqTVJeQNiFpn6QGSRVjTH9W%2FiThvcCn6H6n4BvQDvQWFT%2BSIDIFDAKfE3KOAQeBfB0XGPeQvgE67P8ZoB44DvTHmFgJdOQRv%2BUjc%2BavA9siNTWemgfA3TwGquCZ3w8szFIL1ALngIZorndvgJOR0GlP2gtJkzH%2Bd0fGFxW07NqY%2FCrx5QRXcYjbCbmxF1dkBSbi8kpACah3Yi2Sys74cVyxMWY6bk5BTwgRe%2BYlSzLmxNpU3aBeJogk4XWWpJKUeiap3RJYCpQj4QWZDQCuyIAk19Auj%2BAFYGZZjTGjksaBESB8P9iaxUBIaJzjZcCQcwHdj%2BS2Al0xPOeBYYKHk4vfmQ3Y8YkIwRUb7wQGU7j2ePrA1URx93ayd8UpD8klyPbSQfCOMIO05MbI%2BDvwBbjsMdGTwlX21AAMZzEerkaI9zFkP4AeYCPBg6gNuEb6I%2FthFgN1KSQupqzoRELOSed4DGiJala1UmOMr2U%2Bl%2FTWEy9Japa%2Fy41IWi%2FJ3d4%2FkkaAw0Bz3AocArqApwTvet3O3GbgV8qqjAM7bf4N4KMztwTodcYVyelywKSCD5V3xphNXoezuTskNSl4bgxJ6jPGVJJqbN0aSV%2Bd0M0aO7FCs19Jo2lExphXaTkxdRVgQFK7DZVDZ8%2BcpdmQh3wuILh7ut3AEyt%2B51%2BL%2F0cUfwFOX0t0StltmQAAAABJRU5ErkJggg%3D%3D)](http://www.tango-controls.org) [![License: LGPL v3](https://img.shields.io/badge/License-LGPL%20v3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0) [![](https://img.shields.io/github/release/tango-controls-hdbpp/libhdbpp.svg)](https://github.com/tango-controls-hdbpp/libhdbpp/releases) [![Download](https://api.bintray.com/packages/tango-controls/debian/libhdb%2B%2B6/images/download.svg)](https://bintray.com/tango-controls/debian/libhdb%2B%2B6/_latestVersion)

Interface library for the HDB++ archiving system. Libhdbpp provides an abstract means to archive data events to a Cassandra database, MySQL database, or potentially other database backends.

## Version

The current release version is 1.0.0

### **Important Changes** 0.9.1 -> 1.0.0

* The build system has been moved to CMake. Details on building it using CMake are below.

## Documentation

* See the Tango documentation [here](http://tango-controls.readthedocs.io/en/latest/administration/services/hdbpp/index.html#hdb-an-archiving-historian-service) for broader information about the HB++ archiving system and its integration into Tango Controls
* libhdbpp [CHANGELOG.md](https://github.com/tango-controls-hdbpp/libhdbpp/blob/master/CHANGELOG.md) contains the latest changes both released and in development.

## Bugs Reports

Pleae file bug reports above in the issues section.

## Building and Installation

In its simplest form, clone the repository and assuming a standard install for all dependencies:

```
cd libhdbpp
mkdir build
cd build
cmake ../
make
make install
```

### pkg-config Settings

The build system uses pkg-config to find some dependencies, for example Tango. If Tango is not installed to a standard location, set PKG_CONFIG_PATH, i.e.

```bash
export PKG_CONFIG_PATH=/non/standard/tango/install/location
cmake ../
...
```

CMake is also set to search any paths added to CMAKE_PREFIX_PATH. This can be set and passed into CMake instead.

### Build Dependencies

Ensure the development version of the dependencies are installed. These are as follows:

* Tango Controls 9 or higher.
* omniORB release 4 - libomniorb4 and libomnithread.
* libzmq - libzmq3-dev or libzmq5-dev.

If they have not been installed in a standard location, then use standard CMake flags below to inform the build where to search for them.

The library can be built with Tango Controls install via debian package or source distribution. Supported Tango Controls release is currently 9.2.5a. Ensure build flags are used if tango is installed to a custom location.

### Toolchain Dependencies

If wishing to build the project, ensure the following dependencies are met:

* CMake 3.2 or higher
* C++11 compatible compiler (Tango depenency requires CX11)

### Project Flags

| Flag | Setting | Default | Description |
|------|-----|-----|-----|
| ENABLE_CLANG | ON/OFF | OFF | Clang code static analysis and cppcore guideline enforcement |

### Standard CMake Flags

The build system is CMake therefore standard CMake flags can be used to influence the build and installation process.

The following is a list of common useful CMake flags and their use:

| Flag | Use |
|------|-----|
| CMAKE_INSTALL_PREFIX | Standard CMake flag to modify the install prefix. |
| CMAKE_INCLUDE_PATH | Standard CMake flag to add include paths to the search path. |
| CMAKE_LIBRARY_PATH | Standard CMake flag to add paths to the library search path |

Using the above CMake flags above it´s possible to use tango and other libraries from non-standard locations. Just add all paths to the correct flag.

## License

The source code is released under the LGPL3 license and a copy of this license is provided with the code. 
