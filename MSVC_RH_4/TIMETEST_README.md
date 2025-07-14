# MSVC_RH_4 Time Test

This directory contains a simple timing test for the MSVC_RH_4 protocol implementation.

## Files

- `src/MSVC_RH_4_timetest.cpp` - Main timing test implementation
- `include/MSVC_RH_4_timetest.h` - Header file for timing test utilities

## Building

From the root MSVC directory:

```bash
mkdir -p build
cd build
cmake ..
make MSVC_RH_4_timetest
```

## Running

```bash
cd build/MSVC_RH_4
./MSVC_RH_4_timetest
```

## What it does

The timetest runs the MSVC_RH_4 protocol multiple times with a fixed configuration and reports:

1. **Average execution times** for each phase:
   - Initialize: Environment setup
   - KeyGen: Key generation  
   - ProbGen: Problem generation
   - MaskGen: Mask generation and verification key setup
   - Compute: Proof computation
   - Verify: Proof verification
   - Reconstruct: Final result reconstruction from protocol output
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

You can modify these values in the `MSVC_RH_4_TIMETEST()` function in `MSVC_RH_4_timetest.cpp`.

## Example Output

```
=========================================
    MSVC_RH_4 Simple Timing Test        
=========================================

Configuration:
- Privacy (t): 1
- Fq size (secpar): 32
- Polynomial degree (d): 2
- Number of variables (m): 100
- Iterations: 100

Running 100 iterations...
  Iteration 1/100... OK (36.35 ms)
  Iteration 2/100... OK (36.07 ms)
  ...

=========================================
              RESULTS                   
=========================================
=========================================
Successful runs: 100/100

Average Times (ms):
  Initialize:           0.030
  KeyGen:              29.222
  ProbGen:              0.139
  MaskGen:              0.002
  Compute:              6.945
  Verify:               0.013
  Reconstruct:          0.002
  DirectCompute:        6.708
  -----------
  Total (Protocol):    36.353

Time Breakdown (%):
  Initialize:         0.1%
  KeyGen:            80.4%
  ProbGen:            0.4%
  MaskGen:            0.0%
  Compute:           19.1%
  Verify:             0.0%
  Reconstruct:        0.0%

Protocol vs Direct Computation:
  Protocol Total:      36.353 ms
  Direct Compute:       6.708 ms
  Overhead Factor:        5.4x

Test completed successfully!
```

## Notes

- The test uses a polynomial with all combinations of d-degree monomials over m variables for consistent timing
- Input values are sequential integers (1, 2, 3, ...) for reproducibility
- All successful runs are averaged for the final results
- The test will continue even if some iterations fail, reporting only successful runs
- DirectCompute measures the time for direct polynomial evaluation without using the protocol
- The overhead factor shows how much slower the protocol is compared to direct computation
- The protocol verification result is compared with the direct computation result to ensure correctness

## Differences from MSVC_SP_5

- MSVC_RH_4 uses different data structures (Vec<Fq> for proofs vs Mat<Fq> in SP_5)
- Different key structures (VK_F contains ell and f fields in SP_4)
- Different VK_X structure (contains vectors a, alpha and field elements in SP_4)
- The Compute function returns a single Fq value rather than a vector
