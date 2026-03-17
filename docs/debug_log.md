# Debug Log

## Current Issue: BRAM Access Failure During Pointwise Execution

### Summary

During the transition from a verified **single-layer execution flow** to a more realistic **pointwise CNN layer execution**, a critical issue has been identified related to **BRAM memory access and data visibility**.

Although the accelerator successfully starts and completes execution, the expected data is **not written or read correctly** from specific BRAM regions.

---

## Verified Working State (Baseline)

Before introducing the pointwise test, the system successfully executed a **single-layer demo**, confirming that the full hardware/software pipeline is functional.

### Confirmed Functional Components

* MicroBlaze successfully programs AXI-Lite control registers
* `conv2d_blck` accelerator receives configuration and starts execution
* AXI master interface performs memory transactions
* BRAM is accessible and readable from software
* Output buffer was correctly written and validated

### Conclusion

The following pipeline was verified as **fully operational**:

```
MicroBlaze → AXI-Lite → conv2d_blck → AXI Master → BRAM → Output
```

This confirms that:

* Control path is correct
* Accelerator integration is correct
* Memory interface is functional (at least for base region)

---

## Current Test Scenario

The system was extended to execute a **tiny pointwise convolution layer** with:

* Input: 2×2×2 (8 elements)
* Output: 2×2×2 (2 channels)
* Filter: 1×1 pointwise (Cout=2, Cin=2)
* Quantization parameters included (bias, multiplier, shift)

Memory layout:

| Tensor     | Address Range |
| ---------- | ------------- |
| Input      | 0xC0000000    |
| Filter     | 0xC0010000    |
| Bias       | 0xC0020000    |
| Multiplier | 0xC0030000    |
| Shift      | 0xC0034000    |
| Output     | 0xC0040000    |

---

## Observed Behavior

### 1. Control Flow (Correct)

* `ap_start` is successfully written
* `ap_done` is asserted
* Execution completes without hanging

➡️ The accelerator **runs successfully**

---

### 2. Input Memory (Correct)

At address `0xC0000000`:

```
01 02 03 04 05 06 07 08
```

➡️ Input tensor is correctly written to BRAM

---

### 3. Other Buffers (Incorrect)

At the following regions:

* `0xC0010000` (filter)
* `0xC0020000` (bias)
* `0xC0030000` (multiplier)
* `0xC0034000` (shift)

Observed value:

```
dec0dee3 dec0dee3 dec0dee3 ...
```

➡️ Indicates **uninitialized / unwritten memory**

---

### 4. Output Buffer (Incorrect)

At `0xC0040000`:

```
dec0dee3 ...
```

➡️ Accelerator **did not produce output**

---

## Key Observation

* Input buffer (offset = 0) is written correctly
* All other buffers (non-zero offsets) are **not written**
* Accelerator completes execution but operates on invalid data

---

## Root Cause Hypothesis

This behavior strongly suggests a **BRAM address mapping or access issue** beyond the base address.

Possible causes include:

* BRAM address space not fully mapped beyond base region
* Incorrect address decoding in Vivado block design
* AXI interconnect not forwarding higher address ranges correctly
* Memory region size mismatch (e.g., only first segment valid)
* Software writes to addresses that are not physically backed by BRAM

---

## Supporting Evidence

* Manual inspection shows valid data only at base address
* All higher offsets return debug pattern (`0xdec0dee3`)
* Accelerator execution completes (no crash or stall)
* AXI-Lite register configuration is correct

---

## Impact

* Accelerator operates on invalid weights/bias
* No meaningful computation is performed
* Prevents transition to:

  * multi-channel execution
  * full CNN inference pipeline

---

## Next Debug Steps

1. Perform manual memory write/read test via XSDB:

   ```tcl
   mwr 0xC0010000 0x11223344
   mrd 0xC0010000
   ```

2. Verify BRAM address range in Vivado:

   * Check Address Editor
   * Confirm size and mapping of BRAM controller

3. Validate AXI interconnect routing:

   * Ensure full address space is reachable

4. Confirm BRAM size vs required offsets:

   * Required space ≥ 0x40000 (256KB)

---

## Status

| Stage                    | Status                 |
| ------------------------ | ---------------------- |
| Hardware integration     | ✅ Done                 |
| AXI-Lite control         | ✅ Done                 |
| Single-layer execution   | ✅ Verified             |
| Pointwise execution      | ❌ Blocked (BRAM issue) |
| CNN multi-layer pipeline | ⏳ Pending              |

---

## Conclusion

The system has successfully passed the **single-layer validation phase**, confirming correct integration of the accelerator.

However, the transition to a realistic CNN execution flow is currently blocked due to a **memory access issue affecting BRAM regions beyond the base address**.

This is now the primary blocker before proceeding to full CNN inference.
