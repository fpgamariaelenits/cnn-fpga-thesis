# Debug Log

## Issue: BRAM Access Failure During Pointwise Execution

---

## 1. Background

The system has successfully completed the **single-layer execution phase**, confirming that:

* MicroBlaze correctly programs AXI-Lite registers
* The `conv2d_blck` accelerator starts and completes execution
* AXI master interface is functional
* BRAM is accessible (at least at base address)
* Output was correctly produced in a synthetic test

This validated the core pipeline:

```text
MicroBlaze → AXI-Lite → Conv2D Accelerator → AXI Master → BRAM
```

---

## 2. Current Test

A **tiny pointwise convolution test** was introduced to move toward real CNN execution.

### Configuration

* Input: 2×2×2 (8 elements)
* Output: 2×2×2
* Filter: 1×1 (pointwise)
* Channels: Cin=2, Cout=2

### Memory Layout

| Tensor     | Address    |
| ---------- | ---------- |
| Input      | 0xC0000000 |
| Filter     | 0xC0010000 |
| Bias       | 0xC0020000 |
| Multiplier | 0xC0030000 |
| Shift      | 0xC0034000 |
| Output     | 0xC0040000 |

---

## 3. Observations

### 3.1 Accelerator Execution

* `ap_start` is triggered
* `ap_done` is asserted
* Execution completes normally

✔ Accelerator is running correctly

---

### 3.2 Input Memory (Correct)

Memory at `0xC0000000`:

```text
01 02 03 04 05 06 07 08
```

✔ Input tensor is correctly written

---

### 3.3 Other Tensors (Incorrect)

Memory at:

* `0xC0010000` (filter)
* `0xC0020000` (bias)
* `0xC0030000` (multiplier)
* `0xC0034000` (shift)

Observed:

```text
dec0dee3 dec0dee3 ...
```

❌ Indicates uninitialized memory

---

### 3.4 Output Buffer (Incorrect)

Memory at `0xC0040000`:

```text
dec0dee3 ...
```

❌ No output written by accelerator

---

## 4. Key Finding

* Base address region (`0xC0000000`) is writable
* Higher offset regions are not reflecting writes
* Accelerator runs but operates on invalid data

---

## 5. Root Cause Hypothesis

The issue is likely related to **BRAM address mapping or memory accessibility**.

Possible causes:

* BRAM size smaller than expected
* Address range not fully mapped in Vivado
* AXI interconnect not forwarding full address space
* Incorrect address decoding in block design
* Memory region partially valid (only base segment)

---

## 6. Evidence

* Input region works (offset = 0)
* All other regions return debug pattern
* AXI-Lite configuration verified correct
* Pointer registers verified correct
* Execution flow verified correct

---

## 7. Impact

* Accelerator reads invalid weights and parameters
* No valid computation occurs
* Blocks transition to:

  * multi-channel execution
  * full CNN inference

---

## 8. Next Steps

1. Perform manual memory test (XSDB):

   ```tcl
   mwr 0xC0010000 0x11223344
   mrd 0xC0010000
   ```

2. Verify BRAM configuration in Vivado:

   * Address Editor
   * BRAM size and range

3. Check AXI interconnect:

   * Address decoding
   * Connectivity

4. Confirm required memory size:

   * Minimum ≥ 0x40000 (256KB)

---

## 9. Status

| Component              | Status |
| ---------------------- | ------ |
| AXI-Lite configuration | ✅ OK   |
| Accelerator execution  | ✅ OK   |
| Input memory           | ✅ OK   |
| Other tensors          | ❌ FAIL |
| Output generation      | ❌ FAIL |

---

## 10. Conclusion

The system successfully passed the **single-layer validation stage**, proving correct accelerator integration.

However, the transition to realistic CNN execution is currently blocked by a **BRAM access issue affecting non-zero offset regions**.

This is the primary blocker before proceeding to full CNN inference.
