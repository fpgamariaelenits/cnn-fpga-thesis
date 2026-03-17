# CNN FPGA Thesis

This project implements a custom CNN accelerator on FPGA using:

- HLS (Conv2D kernel)
- Vivado (hardware design)
- Vitis (software control with MicroBlaze)

## Architecture

HLS → exported as IP → integrated in Vivado → controlled by MicroBlaze via AXI → executed from Vitis

## Folder Structure

- `hls/` → HLS kernel and testbench
- `vivado/` → hardware design (block design, constraints)
- `vitis/` → application code (MicroBlaze control)
- `docs/` → notes and debugging logs

## Requirements

- Vivado 2024.2
- Vitis Unified IDE 2024.2

## Quick Start

1. Recreate hardware (Vivado)
2. Import platform in Vitis
3. Build and run application
