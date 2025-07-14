# MSVC_SP_4 Time Test

This directory contains a simple timing test for the MSVC_SP_4 protocol implementation.

## Files

- `src/MSVC_SP_4_timetest.cpp` - Main timing test implementation
- `include/MSVC_SP_4_timetest.h` - Header file for timing test utilities

## Building

From the root MSVC directory:

```bash
mkdir -p build
cd build
cmake ..
make MSVC_SP_4_timetest
```

## Running

```bash
cd build/MSVC_SP_4
./MSVC_SP_4_timetest
```

## What it does

The timetest runs the MSVC_SP_4 protocol multiple times with a fixed configuration and reports:

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

You can modify these values in the `MSVC_SP_4_TIMETEST()` function in `MSVC_SP_4_timetest.cpp`.

## Example Output

```
=========================================
    MSVC_SP_4 Simple Timing Test        
=========================================

Configuration:
- Privacy (t): 1
- Fq size (secpar): 32
- Polynomial degree (d): 2
- Number of variables (m): 100
- Iterations: 100

Running 100 iterations...
  Iteration 1/100... OK (35.86 ms)
  Iteration 2/100... OK (35.67 ms)
  ...

=========================================
              RESULTS                   
=========================================
Successful runs: 100/100

Average Times (ms):
  Initialize:           0.033
  KeyGen:              29.306
  ProbGen:              0.132
  Compute:              7.063
  Verify:               0.013
  DirectCompute:        6.845
  -----------
  Total (Protocol):    36.547

Time Breakdown (%):
  Initialize:         0.1%
  KeyGen:            80.2%
  ProbGen:            0.4%
  Compute:           19.3%
  Verify:             0.0%

Protocol vs Direct Computation:
  Protocol Total:      36.547 ms
  Direct Compute:       6.845 ms
  Overhead Factor:        5.3x

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

- MSVC_SP_4 uses different data structures (Vec<Fq> for proofs vs Mat<Fq> in SP_5)
- Different key structures (VK_F contains ell and f fields in SP_4)
- Different VK_X structure (contains vectors a, alpha and field elements in SP_4)
- The Compute function returns a single Fq value rather than a vector
