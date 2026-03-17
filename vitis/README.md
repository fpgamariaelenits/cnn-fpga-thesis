# Vitis Application

This folder contains the software running on MicroBlaze.

## Structure

- `bram_test/src/`
  - `main.c`
  - `single_layer.c`
  - `tiny_pointwise.h`

## Purpose

- Configure accelerator
- Write/read BRAM
- Execute CNN layer

## How to Run

1. Open Vitis
2. Import platform using: vivado/exports/system_wrapper.xsa
3. Create application project
4. Add source files from `bram_test/src/`
5. Build project
6. Run on hardware (or simulation)

## Debugging

- Use Memory Inspector
