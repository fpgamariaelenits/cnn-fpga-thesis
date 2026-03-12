# Debug Log

## Current known issue
BRAM access issue observed during integration/testing.

## Context
The project combines:
- Vivado block design
- HLS conv2d accelerator
- Vitis software control/application

## Current assumptions
- Current board-based Vivado project is the most stable hardware baseline
- A fresh Vitis workspace will likely be used
- GitHub repo is being prepared before importing actual project files

## Next investigation steps
- Reopen stable Vivado project
- inspect memory mapping / BRAM connections
- verify address mapping in Vitis
- confirm exported `.xsa` matches current hardware
