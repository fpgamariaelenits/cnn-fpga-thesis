## Files

- `conv2d_blck.cpp` → main kernel
- `definitions.h` → parameters
- `quant_utils.h` → quantization
- `DepthwiseConvPerChannel_*` → helper functions

## How to Run (Vitis HLS)

1. Open Vitis HLS
2. Create new project
3. Add all files from `src/`
4. Set top function: conv2d_blck
5. Add testbench from `test_bench/`
6. Run:
- C Simulation
- C Synthesis

## Output

The kernel is exported as RTL IP and used in Vivado.
