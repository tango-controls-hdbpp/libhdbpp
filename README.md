# Libhdbpp

Interface library for the HDB++ archiving system. Libhdbpp provides an abstract means to archive data events to either a Cassandra or MySQL database.

## Version

The current release version is 1.0.0

### **Important Changes** 0.9.1 -> 1.0.0

* The build system has been moved to CMake. Details on building it using CMake are below.

## Documentation

* See the Tango documentation [here](http://tango-controls.readthedocs.io/en/latest/administration/services/hdbpp/index.html#hdb-an-archiving-historian-service) for broader information about the HB++ archiving system and its integration into Tango Controls
* libhdbpp [CHANGELOG.md](https://github.com/tango-controls/libhdbpp/blob/master/CHANGELOG.md) contains the latest changes both released and in development.

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
