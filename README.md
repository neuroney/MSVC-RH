# MSVC-RH

This repository contains the implementation of algorithms from the MSVC-RH paper, focusing on multivariate verifiable computation (MSVC) schemes.

**What this codebase includes**: 
- C++17 implementations of various multivariate verifiable computation schemes (SP, CH, RH)
- Benchmark tools for performance comparison between different schemes

**What this codebase is not**:
- Production-ready code
- Extensively tested for all edge cases
- Optimized for all possible use cases

## Implemented Schemes

This repository includes implementations of multiple schemes:
- **SP4/SP5**: Standard Protocol implementation in [Zhang and Wang](https://ieeexplore.ieee.org/document/9833792)
- **CH5**: Standard Protocol implementation in [Chen, Li and Zhang](https://ieeexplore.ieee.org/document/10619256)
- **RH4/RH5**: MSVC-RH Protocol implementation in our paper [Author et al.](https://link-to-our-paper)

## Setup and Building Instructions

### Prerequisites

- C++17 compatible compiler
- CMake (version 3.25 or higher)
- [NTL Library](https://libntl.org/doc/tour-unix.html)
- GMP Library

### Building

```shell
mkdir build
cd build
cmake ..
make
```

## Running Examples

Individual scheme examples can be executed with:

```shell
cd build
MSVC_RH_4/MSVC_RH_4
```

To run timing tests for a specific scheme:

```shell
cd build
MSVC_RH_4/MSVC_RH_4_timetest
```

## Unified Benchmark Tool

For detailed information on the unified benchmark tool, please refer to the [unified_test README](unified_test/README.md).

In short, the tool allows you to compare all implemented MSVC schemes with consistent parameters:

```shell
cd build
unified_test/unified_timetest -all
```

## Project Structure

- `common/`: Shared utilities and data structures
- `MSVC_XX/`: Implementation of scheme XX (RH/SP/CH) 
- `unified_test/`: Unified benchmarking tool
- `cmake/`: CMake modules for dependencies

## License

Please refer to the included license file for usage terms.