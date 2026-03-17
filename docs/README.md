# Documentation

This folder contains **debugging notes, issue tracking, and technical observations** related to the FPGA-based CNN accelerator project.

---

## Contents

* `debug_log.md`
  Detailed log of debugging steps, identified issues, and system behavior during bring-up and integration.

---

## Project Context

The project implements a **custom CNN inference accelerator** using:

* Vivado (hardware design & system integration)
* Vitis HLS (Conv2D accelerator)
* Vitis (embedded software on MicroBlaze)

The system architecture follows:

```text
MicroBlaze → AXI-Lite → Conv2D Accelerator → AXI Master → BRAM
```

---

## Current Status

| Stage                     | Status       |
| ------------------------- | ------------ |
| Hardware integration      | ✅ Completed  |
| AXI-Lite control          | ✅ Verified   |
| Single-layer execution    | ✅ Successful |
| Pointwise layer execution | ❌ Blocked    |
| Full CNN pipeline         | ⏳ Pending    |

---

## Current Focus

The current debugging effort is focused on:

* BRAM access and memory mapping issues
* Validation of AXI master transactions
* Transition from synthetic test to real CNN layer execution

---

## Known Issue (Short Summary)

A **BRAM access issue** has been identified during pointwise layer testing:

* Input data is written correctly
* Other tensors (weights, bias, etc.) are not visible in memory
* Accelerator completes execution but produces no output

👉 Full analysis is documented in `debug_log.md`

---

## Purpose of This Folder

This documentation serves to:

* Track debugging progress
* Provide reproducibility for issues
* Enable project handover to another engineer
* Support thesis documentation

---

## Notes

This folder will be continuously updated as:

* Issues are resolved
* New bugs are identified
* System progresses toward full CNN inference
