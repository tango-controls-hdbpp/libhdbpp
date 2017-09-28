# Libhdbpp

Interface library for the HDB++ archiving system. Libhdbpp provides an abstract means to archive data events to either a Cassandra or MySQL database.

## **Important Changes** 0.9.1 -> 1.0.0

This release version of the library has made a few changes to standardize its use and deployment.
* The build system has been moved to CMake. Details on building it using CMake are below.
* The main tango.h include has been changed from `include <tango.h>` to `include <tango/tango.h>`. This reflects the standard install path of the tango headers in /usr/include/tango and is more consistent with header include patterns. What this means for building the library is we no longer need to specify the tango install directory if its installed in a standard location. If its installed non-standard, then standard CMake flags can used to specify the include and library location (See the [INSTALL.md](https://github.com/tango-controls/libhdbpp/blob/master/INSTALL.md))

## Documentation

* See the tango documentation [here](http://tango-controls.readthedocs.io/en/latest/administration/services/hdbpp/index.html#hdb-an-archiving-historian-service) for broader information about the HB++ archiving system and its integration into Tango Controls
* Libhdbpp [CHANGELOG.md](https://github.com/tango-controls/libhdbpp/blob/master/CHANGELOG.md) contains the latest changes both released and in development.

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

See the [INSTALL.md](https://github.com/tango-controls/libhdbpp/blob/master/INSTALL.md) file for more detailed instructions on how to build and install libhdbpp.

## License

The source code is released under the LGPL3 license and a copy of this license is provided with the code. 
