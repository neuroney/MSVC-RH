# Unified Timetest Runner

This tool allows you to run and compare timetests from all MSVC subprojects with consistent parameters.

## Building

From the root MSVC directory:

```bash
mkdir -p build
cd build
cmake ..
make unified_timetest
```

## Usage

The unified timetest runner can be used to benchmark different algorithm implementations with consistent parameters.

### Basic Usage

```bash
./unified_timetest [options] [tests]
```

### Options

- `-d <value>`: Set polynomial degree (default: 2)
- `-m <value>`: Set number of variables (default: 100)
- `-t <value>`: Set privacy parameter (default: 1)
- `-secpar <value>`: Set security parameter (default: 128)
- `-iter <value>`: Set number of iterations (default: 10)
- `-all`: Run all available tests
- `-h, --help`: Show help message

### Available Tests

- `CH5`: Run MSVC_CH_5 timetest
- `RH4`: Run MSVC_RH_4 timetest
- `RH5`: Run MSVC_RH_5 timetest
- `SP4`: Run MSVC_SP_4 timetest
- `SP5`: Run MSVC_SP_5 timetest

### Examples

Run a single test with default parameters:
```bash
./unified_timetest RH4
```

Run multiple tests with custom parameters:
```bash
./unified_timetest -d 3 -m 50 -t 2 RH4 SP4
```

Run all available tests with custom iterations:
```bash
./unified_timetest -iter 5 -all
```

Compare degree 4 polynomials with 200 variables:
```bash
./unified_timetest -d 4 -m 200 -all
```

## Interpreting Results

The tool provides a comprehensive comparison table showing:

- **Total**: Total execution time in milliseconds
- **KeyGen**: Time for key generation phase
- **ProbGen**: Time for problem generation phase
- **MaskGen**: Time for mask generation (only applies to RH algorithms)
- **Compute**: Time for computation phase
- **Verify**: Time for verification phase
- **Reconst**: Time for reconstruction phase (only applies to RH algorithms)
- **Direct**: Time for direct computation (baseline)
- **Overhead**: Ratio of total time to direct computation time

Note: The MaskGen and Reconstruct columns only apply to RH4 and RH5 algorithms.
SP4, SP5, and CH5 algorithms do not include these steps.

## Troubleshooting

If you encounter any issues:

1. Ensure all dependencies are properly installed
2. Check that all required MSVC subprojects are built successfully
3. Make sure you are running from the build directory
4. Verify that the correct permissions are set for execution
5. Consult the documentation for any specific algorithm requirements