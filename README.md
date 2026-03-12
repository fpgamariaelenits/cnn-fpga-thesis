# CNN FPGA Thesis

FPGA-based conv2d accelerator thesis project using Vivado, Vitis and HLS.

## Overview

This repository contains the hardware, HLS, software, test data, and exported platform files for a CNN/conv2d FPGA implementation.

## Repository Structure

- `docs/` documentation, notes, debug logs, architecture notes
- `vivado/` Vivado hardware design sources
- `hls/` HLS kernel implementation
- `vitis/` Vitis software application and platform-related notes
- `test_data/` input data, weights, bias, and reference outputs
- `exports/` exported hardware files such as `.xsa` and optional stable bitstreams

## Current Status

- Repository initialized
- Folder structure created
- `.gitignore` added for Vivado/Vitis/HLS generated files

## Next Steps

- Add Vivado project sources
- Add HLS conv2d source files
- Add Vitis bare-metal application
- Add test vectors and reference outputs
- Export stable `.xsa` from Vivado

## Notes

This repository is intended to store source files and essential project artifacts, not generated tool outputs.
