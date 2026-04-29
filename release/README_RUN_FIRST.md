# RUN THIS FIRST – Verified Baseline

## Goal

Verify that the FPGA Conv2D accelerator works end-to-end.

---

## Step 1 – Open Vitis

Create a new workspace.

---

## Step 2 – Create Platform

Import hardware platform from:

```
hardware/system_wrapper.xsa
```

---

## Step 3 – Create Application

* Create empty application project
* Replace the `src/` folder with the provided one

---

## Step 4 – Program FPGA

Use:

```
hardware/system_wrapper.bit
```

---

## Step 5 – Run Application

---

## Expected Result

Check memory at:

```
0xC0002000
```

Expected values:

```
02 04 06 08
```

---

## What this proves

* MicroBlaze is running
* AXI-Lite configuration works
* BRAM access works
* Conv2D accelerator computes correctly

---

## Notes

This is a verified minimal system.
More complex CNN layers are under development.

