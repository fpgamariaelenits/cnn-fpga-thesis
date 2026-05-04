# RUN THIS FIRST – Verified Baseline execution guide

## Goal

Verify that the FPGA Conv2D accelerator works end-to-end on hardware.
This folder does not recreate the Vivado hardware design from source.
It provides a verified pre-built hardware baseline using the exported XSA and bitstream.
Use the provided .bit and .xsa together. Do not regenerate the bitstream unless you rebuild the Vivado project separately.

---

## Requirements

* Vivado 2024.2
* Vitis Unified IDE 2024.2
* Digilent Arty A7-35T board (must be connected)

---

## Overview

You will:

1. Create a hardware platform from the provided `.xsa`
2. Create an application project in Vitis
3. Replace the source files with the provided ones
4. Program the FPGA and run the application
5. Verify the result in memory

---

## Step 1 – Open Vitis

Create a new workspace.

---

## Step 2 – Create Hardware Platform

1. Select **Create Platform Project**
2. Choose:

   * **Platform from hardware (XSA)**
3. Browse and select:

   ```
   hardware/system_wrapper.xsa
   ```
4. Click **Finish**

---

## Step 3 – Create Application Project

1. Select **Create Application Project**
2. Choose the platform created in Step 2
3. Select:

   * Template: **Empty Application**
4. Finish the wizard

---

## Step 4 – Replace Source Files

1. Open the created application project
2. Navigate to the `src/` folder
3. Delete the existing contents
4. Copy the files from:

   ```
   software/src/
   ```

   into the project `src/` folder

---
## Step 5 – Program FPGA (REQUIRED)
In this step you load the hardware design onto the FPGA.

The bitstream file is:

   ```
   hardware/system_wrapper.bit
   ```

### What is this file?
The `.bit` file is the compiled hardware design that configures the FPGA.
Without programming the FPGA:
- The accelerator will NOT work
- The application will run but produce incorrect or no results
---
### Steps
1. Open **Vitis Unified IDE**
2. From the top menu select:

Xilinx → Program Device

3. In the dialog:
- Set **Bitstream file** to:
  
   ```
   hardware/system_wrapper.bit
   ```

4. Click:

Program

---
### Expected behavior
- The FPGA is programmed successfully
- No errors appear
---
## Step 6 – Build and Run Application
### 6.1 Build Project
1. In **Project Explorer**
2. Right-click the application project
3. Select:

Build Project

Wait until the build completes successfully.
---
### 6.2 Run on Hardware
1. Right-click the application project
2. Select:

Run As → Launch on Hardware (Single Application Debug)

---
### What happens internally
- The program (`main.c`) is loaded to the MicroBlaze
- The CPU writes input/filter/bias data to memory
- The accelerator is started via AXI-Lite
- The accelerator performs the computation
- The result is written back to memory
---
## Step 7 – Verify Result (Memory Inspector)
### Open Memory Inspector
1. Go to:

Window → Show View → Memory

2. Click:

* (Add Memory Monitor)

---
### Enter the following values

Address: 0xC0002000
Length: 16

---
### Expected Memory Content
You should see something similar to:

08060402

---
### Important (Little-endian interpretation)
The system is little-endian, so this corresponds to:

02 04 06 08

---
### Final Expected Output

2
4
6
8

---
## What this proves
If you see the expected values:
- MicroBlaze is running correctly
- AXI-Lite communication works
- BRAM read/write is functional
- The Conv2D hardware accelerator executes correctly
- End-to-end hardware/software integration is verified
---
## Common Issues
### 1. FPGA not programmed
Make sure Step 5 is completed BEFORE running the application.
---
### 2. Wrong memory address
Use exactly:

0xC0002000

---
### 3. Build errors
Re-run:

Build Project

---
## Important Note
This is a **verified minimal working baseline**.
More complex CNN configurations (multi-channel, multi-layer) are not included in this step and may require additional setup.
