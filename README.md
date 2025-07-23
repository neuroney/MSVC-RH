# MSVC-RH

This codebase is part of the MSVC-RH paper.

**What this codebase includes**: example and benchmark implementations in C++17 for some of the schemes in the MSVC-RH paper.

**What this codebase is not**: it is not for production use; it is not extensively tested.

## Setup and Building Instructions

First, install the library [NTL](https://libntl.org/doc/tour-unix.html) required by MSVC-RH. On several Ubuntu systems this can be done directly through the link above.

To build library and executables:
```shell
mkdir build
cd build
cmake ..
make
```

To try an example, run e.g.:
```shell
MSVC-RH_RH_4/MSVC-RH_RH_4 
```

To run timing tests, use:
```shell
MSVC-RH_RH_4/MSVC-RH_RH_4_timetest
```