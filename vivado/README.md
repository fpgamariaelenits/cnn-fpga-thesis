# Vivado Hardware Design

This directory contains the FPGA hardware design of the CNN accelerator system implemented in **Xilinx Vivado 2024.2**.

The design integrates:

- MicroBlaze soft processor
- AXI interconnect infrastructure
- BRAM memory system
- Custom **Conv2D hardware accelerator** generated from HLS

---

## Folder Structure

```
vivado
├── project
│   ├── conv2d_board.xpr
│   ├── system.bd
│   └── arty_constraints.xdc
│
└── exports
    ├── system_wrapper.bit
    └── system_wrapper.xsa
```

---

## project/

Contains the minimal sources required to open and rebuild the hardware design.

### Files

- **conv2d_board.xpr**  
  Vivado project file (can be used to open the design directly).

- **system.bd**  
  Block Design describing the system architecture.  
  Includes:
  - MicroBlaze processor
  - AXI interconnect
  - BRAM controllers
  - HLS Conv2D accelerator IP

- **arty_constraints.xdc**  
  FPGA constraint file (pins, clocks, board configuration).

- **recreate.tcl (recommended)**  
  TCL script to fully recreate the project in a clean environment.  
  This is the **preferred reproducible method**.

---

## exports/

Contains generated hardware artifacts after synthesis & implementation.

- **system_wrapper.bit**  
  FPGA bitstream used to program the FPGA.

- **system_wrapper.xsa**  
  Hardware platform description used by **Vitis**.
