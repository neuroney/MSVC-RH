# MSVC_RH_5 Time Test

This directory contains a simple timing test for the MSVC_RH_5 protocol implementation.

## Files

- `src/MSVC_RH_5_timetest.cpp` - Main timing test implementation
- `include/MSVC_RH_5_timetest.h` - Header file for timing test utilities

## Building

From the root MSVC directory:

```bash
mkdir -p build
cd build
cmake ..
make MSVC_RH_5_timetest
```

## Running

```bash
cd build/MSVC_RH_5
./MSVC_RH_5_timetest
```

## What it does

The timetest runs the MSVC_RH_5 protocol multiple times with a fixed configuration and reports:

1. **Average execution times** for each phase:
   - Initialize: Environment setup
   - KeyGen: Key generation  
   - ProbGen: Problem generation
   - Compute: Proof computation
   - Verify: Proof verification
   - DirectCompute: Direct polynomial evaluation (for comparison)

2. **Time breakdown** showing the percentage of total time spent in each phase

3. **Protocol vs Direct Computation comparison** showing the overhead factor of using the protocol

## Configuration

The test is configured with:
- Privacy (t): 1
- Fq size (secpar): 32
- Polynomial degree (d): 2
- Number of variables (m): 100
- Iterations: 100

You can modify these values in the `MSVC_RH_5_TIMETEST()` function in `MSVC_RH_5_timetest.cpp`.

## Example Output

```
=========================================
    MSVC_RH_5 Simple Timing Test        
=========================================

Configuration:
- Privacy (t): 1
- Fq size (secpar): 32
- Polynomial degree (d): 2
- Number of variables (m): 100
- Iterations: 100

Running 100 iterations...
  Iteration 1/100... OK (16.54 ms)
  Iteration 2/100... OK (16.16 ms)
  ...

=========================================
              RESULTS                   
=========================================
Successful runs: 100/100

Average Times (ms):
  Initialize:           0.033
  KeyGen:               9.564
  ProbGen:              0.038
  MaskGen:              0.001
  Compute:              6.889
  Verify:               0.014
  Reconstruct:          0.002
  DirectCompute:        6.802
  -----------
  Total (Protocol):    16.540

Time Breakdown (%):
  Initialize:         0.2%
  KeyGen:            57.8%
  ProbGen:            0.2%
  MaskGen:            0.0%
  Compute:           41.6%
  Verify:             0.1%
  Reconstruct:        0.0%

Protocol vs Direct Computation:
  Protocol Total:      16.540 ms
  Direct Compute:       6.802 ms
  Overhead Factor:        2.4x
```

## Notes

- The test uses a polynomial with all combinations of d-degree monomials over m variables for consistent timing
- Input values are sequential integers (1, 2, 3, ...) for reproducibility
- All successful runs are averaged for the final results
- The test will continue even if some iterations fail, reporting only successful runs
- DirectCompute measures the time for direct polynomial evaluation without using the protocol
- The overhead factor shows how much slower the protocol is compared to direct computation
- The protocol verification result is compared with the direct computation result to ensure correctness
