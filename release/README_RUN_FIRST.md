# RUN THIS FIRST – Verified Baseline

## Goal

Verify that the FPGA Conv2D accelerator works end-to-end on hardware.

---

## Requirements

* Vivado 2024.2
* Vitis Unified IDE 2024.2
* Digilent Arty A7-35T board

---

## Step 1 – Open Vitis

Create a new workspace.

---

## Step 2 – Create Hardware Platform

* Select **Create Platform Project**
* Import hardware from:

```
hardware/system_wrapper.xsa
```
### Detailed Vitis Steps (for first-time setup)

#### Create Platform Project

1. Open Vitis
2. Select **Create Platform Project**
3. Choose:

   * Platform from hardware (XSA)
4. Browse and select:

   ```
   hardware/system_wrapper.xsa
   ```
5. Click **Finish**

---

#### Create Application Project

1. Select **Create Application Project**
2. Choose the platform created above
3. Select:

   * Template: **Empty Application**
4. Finish the wizard

---

#### Replace Source Files

1. Open the created application project
2. Navigate to the `src/` folder
3. Delete the existing contents
4. Copy the files from:

   ```
   software/src/
   ```

   into the project `src/` folder

---

#### Build & Run

1. Right-click project → **Build Project**
2. Program FPGA with `.bit`
3. Right-click → **Run As → Launch on Hardware**

---

## Step 3 – Create Application Project

* Create a new **Empty Application Project**
* Select the platform created in Step 2
* Replace the contents of the `src/` folder with:

```
software/src/main.c
software/src/tiny_pointwise.h
```

---

## Step 4 – Program FPGA

Program the device using:

```
hardware/system_wrapper.bit
```

---

## Step 5 – Build and Run

* Build the application
* Run it on hardware

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
