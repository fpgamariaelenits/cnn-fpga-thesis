# RUN THIS FIRST – Verified Baseline

## Goal

Verify that the FPGA Conv2D accelerator works end-to-end on hardware.

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

## Step 5 – Program FPGA

Program the device using:

```
hardware/system_wrapper.bit
```

---

## Step 6 – Build and Run

1. Right-click project → **Build Project**
2. Right-click → **Run As → Launch on Hardware**

---

## Expected Result

After execution, inspect memory at:

```
0xC0002000
```

Expected values:

```
02 04 06 08
```

---

## What this proves

* MicroBlaze is running correctly
* AXI-Lite communication with the accelerator works
* BRAM read/write is functional
* The Conv2D hardware accelerator produces correct output

---

## Notes

This is a **verified minimal working baseline**.

More complex CNN layers (e.g., multi-channel / pointwise / depthwise) are under development and may not yet be fully validated.
