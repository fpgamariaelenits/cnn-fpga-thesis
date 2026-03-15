# Vivado Hardware Design

This directory contains the FPGA hardware design of the CNN accelerator system implemented in **Xilinx Vivado**.

The design integrates a **MicroBlaze processor**, AXI interconnect infrastructure and a custom **Conv2D hardware accelerator** generated from HLS.

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

### project/

Contains the minimal sources required to open the hardware project in Vivado.

* **conv2d_board.xpr**
  Vivado project file.

* **system.bd**
  Block Design describing the system architecture.
  It includes the MicroBlaze processor, AXI peripherals, memory system and the hardware accelerator.

* **arty_constraints.xdc**
  FPGA constraint file defining board pins and clock configuration.

### exports/

Contains the hardware export files generated after synthesis and implementation.

* **system_wrapper.bit**
  FPGA bitstream used to program the FPGA device.

* **system_wrapper.xsa**
  Hardware platform description used by **Vitis** to build the software application.

## How to Open the Project

1. Open **Vivado**.
2. Select **Open Project**.
3. Navigate to:

```
vivado/project/conv2d_board.xpr
```

4. Open the project.

Vivado may regenerate missing build folders automatically.

## How to Rebuild the Hardware

After opening the project:

1. Validate the block design.
2. Run **Synthesis**.
3. Run **Implementation**.
4. Generate **Bitstream**.

This will recreate the hardware design and regenerate the export files if needed.

## Hardware Export

The generated hardware platform for the software environment is provided as:

```
vivado/exports/system_wrapper.xsa
```

This file is used in **Vitis** to create the software platform and run the CNN accelerator application on the MicroBlaze processor.

## Notes

Temporary Vivado build directories (`.runs`, `.gen`, `.cache`, `.Xil`) are intentionally not included in the repository since they are automatically recreated by Vivado.
