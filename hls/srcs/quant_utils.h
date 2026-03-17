#ifndef QUANT_UTILS_H
#define QUANT_UTILS_H

#include <stdint.h>
#include <limits.h>

inline int32_t MultiplyByQuantizedMultiplier(int32_t x,
                                             int32_t quantized_multiplier,
                                             int shift) {
    int left_shift  = (shift > 0) ? shift : 0;
    int right_shift = (shift > 0) ? 0     : -shift;

    int64_t prod = static_cast<int64_t>(x) * static_cast<int64_t>(quantized_multiplier);
    // Fixed-point shift for TFLite
    prod = (prod + (1ll << 30)) >> 31; 

    if (left_shift > 0) {
        prod <<= left_shift;
    }

    if (right_shift > 0) {
        const int64_t offset = (1ll << (right_shift - 1));
        // Symmetric rounding for TFLite compatibility
        if (prod >= 0) {
            prod += offset;
        } else {
            prod += (offset - 1);
        }
        prod >>= right_shift;
    }

    if (prod > INT32_MAX) prod = INT32_MAX;
    if (prod < INT32_MIN) prod = INT32_MIN;

    return static_cast<int32_t>(prod);
}

#endif