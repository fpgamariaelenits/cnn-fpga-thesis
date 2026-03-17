// ============================================================================
// conv2d_blck.cpp  (PHASE-1: AXI-Lite regs + AXI-Stream OUT mirror)
// - Keeps original compute_* implementations (pointer-based)
// - Adds AXI-Stream output (m_axis) that streams the produced output buffer
// - Moves "control" scalars to AXI-Lite (bundle=CTRL) as per Phase-1 guide
// ============================================================================

#include "hls_stream.h"
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include <stdint.h>

#include "quant_utils.h"
#include "definitions.h"

// -----------------------------
// AXI-Stream 8-bit type
// -----------------------------
typedef ap_axiu<8, 0, 0, 0> axis8_t;

// Helper function for clamping
static inline int8_t clamp_s8(int32_t x, int32_t lo, int32_t hi) {
#pragma HLS INLINE
    if (x < lo) return (int8_t)lo;
    if (x > hi) return (int8_t)hi;
    return (int8_t)x;
}

// --------------------------------------------------------
// 1. POINTWISE IMPLEMENTATION (1x1) - OPTIMIZED
// --------------------------------------------------------
void compute_pointwise(
    const int8_t *input_data, const int8_t *filter_data, const int32_t *bias_data,
    const int32_t *multiplier_data, const int32_t *shift_data, int8_t *output_data,
    int input_h, int input_w, int input_ch,
    int output_h, int output_w, int output_ch,
    int32_t input_offset, int32_t weights_offset, int32_t output_offset,
    int32_t act_min, int32_t act_max
) {
    // --- LOCAL BUFFERS (BRAM) ---
    int8_t  local_weights[MAX_OUTPUT_CHANNELS][MAX_INPUT_CHANNELS];
    int32_t local_bias[MAX_OUTPUT_CHANNELS];
    int32_t local_mult[MAX_OUTPUT_CHANNELS];
    int32_t local_shift[MAX_OUTPUT_CHANNELS];

#pragma HLS ARRAY_PARTITION variable=local_weights dim=2 complete

    // 1. Φόρτωση Βαρών (Weight Caching)
    LOAD_WEIGHTS_OUT: for (int oc = 0; oc < output_ch; ++oc) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_OUTPUT_CHANNELS
        local_bias[oc]  = bias_data[oc];
        local_mult[oc]  = multiplier_data[oc];
        local_shift[oc] = shift_data[oc];

        LOAD_WEIGHTS_IN: for (int ic = 0; ic < input_ch; ++ic) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_INPUT_CHANNELS
            int w_idx = oc * input_ch + ic;
            local_weights[oc][ic] = filter_data[w_idx];
        }
    }

    // 2. Υπολογισμός
    for (int y = 0; y < output_h; ++y) {
        for (int x = 0; x < output_w; ++x) {

            const int base_in_idx = (y * input_w + x) * input_ch;

            for (int oc = 0; oc < output_ch; ++oc) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_OUTPUT_CHANNELS
#pragma HLS PIPELINE II=1

                int32_t acc = local_bias[oc];

                for (int ic = 0; ic < input_ch; ++ic) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_INPUT_CHANNELS
#pragma HLS UNROLL factor=16

                    const int32_t inp_val = (int32_t)input_data[base_in_idx + ic] + input_offset;
                    const int32_t w_val   = (int32_t)local_weights[oc][ic] + weights_offset;
                    acc += inp_val * w_val;
                }

                acc = MultiplyByQuantizedMultiplier(acc, local_mult[oc], local_shift[oc]);
                acc += output_offset;
                int8_t res = clamp_s8(acc, act_min, act_max);

                int out_idx = (y * output_w + x) * output_ch + oc;
                output_data[out_idx] = res;
            }
        }
    }
}

// --------------------------------------------------------
// 2. STANDARD IMPLEMENTATION (3x3) - OPTIMIZED (cached weights)
// --------------------------------------------------------
void compute_standard(
    const int8_t *input_data, const int8_t *filter_data, const int32_t *bias_data,
    const int32_t *multiplier_data, const int32_t *shift_data, int8_t *output_data,
    int input_h, int input_w, int input_ch,
    int output_h, int output_w, int output_ch,
    int kernel_h, int kernel_w, int stride_h, int stride_w, int pad_h, int pad_w,
    int32_t input_offset, int32_t weights_offset, int32_t output_offset,
    int32_t act_min, int32_t act_max
) {
    int8_t  local_weights[MAX_OUTPUT_CHANNELS][MAX_KERNEL_SIZE * MAX_KERNEL_SIZE][MAX_INPUT_CHANNELS];
    int32_t local_bias[MAX_OUTPUT_CHANNELS];
    int32_t local_mult[MAX_OUTPUT_CHANNELS];
    int32_t local_shift[MAX_OUTPUT_CHANNELS];

#pragma HLS ARRAY_PARTITION variable=local_weights dim=3 complete

    // 1. Weight caching
    LOAD_WEIGHTS_STD_OUT: for (int oc = 0; oc < output_ch; ++oc) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_OUTPUT_CHANNELS
        local_bias[oc]  = bias_data[oc];
        local_mult[oc]  = multiplier_data[oc];
        local_shift[oc] = shift_data[oc];

        for (int kh = 0; kh < kernel_h; ++kh) {
            for (int kw = 0; kw < kernel_w; ++kw) {
                for (int ic = 0; ic < input_ch; ++ic) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_INPUT_CHANNELS
                    int w_idx = (((oc * kernel_h) + kh) * kernel_w + kw) * input_ch + ic;
                    local_weights[oc][kh * kernel_w + kw][ic] = filter_data[w_idx];
                }
            }
        }
    }

    // 2. Main compute
    for (int y = 0; y < output_h; ++y) {
        for (int x = 0; x < output_w; ++x) {

            for (int oc = 0; oc < output_ch; ++oc) {
#pragma HLS PIPELINE II=1

                int32_t acc = local_bias[oc];

                const int in_y_origin = y * stride_h - pad_h;
                const int in_x_origin = x * stride_w - pad_w;

                for (int ky = 0; ky < kernel_h; ++ky) {
                    int in_y = in_y_origin + ky;
                    for (int kx = 0; kx < kernel_w; ++kx) {
                        int in_x = in_x_origin + kx;

                        bool is_valid = (in_y >= 0 && in_y < input_h && in_x >= 0 && in_x < input_w);

                        if (is_valid) {
                            int base_in = (in_y * input_w + in_x) * input_ch;
                            for (int ic = 0; ic < input_ch; ++ic) {
#pragma HLS UNROLL factor=16
                                int32_t inp_val = (int32_t)input_data[base_in + ic] + input_offset;
                                int32_t w_val   = (int32_t)local_weights[oc][ky * kernel_w + kx][ic] + weights_offset;
                                acc += inp_val * w_val;
                            }
                        }
                    }
                }

                acc = MultiplyByQuantizedMultiplier(acc, local_mult[oc], local_shift[oc]);
                acc += output_offset;
                int8_t res = clamp_s8(acc, act_min, act_max);

                int out_idx = (y * output_w + x) * output_ch + oc;
                output_data[out_idx] = res;
            }
        }
    }
}

// --------------------------------------------------------
// 3. DEPTHWISE IMPLEMENTATION - OPTIMIZED
// --------------------------------------------------------
void compute_depthwise(
    const int8_t *input_data, const int8_t *filter_data, const int32_t *bias_data,
    const int32_t *multiplier_data, const int32_t *shift_data, int8_t *output_data,
    int input_h, int input_w, int input_ch,
    int output_h, int output_w, int output_ch,
    int kernel_h, int kernel_w, int stride_h, int stride_w, int pad_h, int pad_w,
    int32_t input_offset, int32_t weights_offset, int32_t output_offset,
    int32_t act_min, int32_t act_max
) {
    int8_t  local_weights[MAX_INPUT_CHANNELS][MAX_KERNEL_SIZE * MAX_KERNEL_SIZE];
    int32_t local_bias[MAX_INPUT_CHANNELS];
    int32_t local_mult[MAX_INPUT_CHANNELS];
    int32_t local_shift[MAX_INPUT_CHANNELS];

#pragma HLS ARRAY_PARTITION variable=local_weights dim=2 complete

    // 1. Weight caching
    LOAD_WEIGHTS_DW: for (int c = 0; c < output_ch; ++c) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_OUTPUT_CHANNELS
        local_bias[c]  = bias_data[c];
        local_mult[c]  = multiplier_data[c];
        local_shift[c] = shift_data[c];

        for (int kh = 0; kh < kernel_h; ++kh) {
            for (int kw = 0; kw < kernel_w; ++kw) {
#pragma HLS LOOP_TRIPCOUNT min=1 max=9
                int w_idx = ((kh * kernel_w) + kw) * output_ch + c;
                local_weights[c][kh * kernel_w + kw] = filter_data[w_idx];
            }
        }
    }

    // 2. Compute
    for (int y = 0; y < output_h; ++y) {
        for (int x = 0; x < output_w; ++x) {

            for (int c = 0; c < output_ch; ++c) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=1 max=MAX_OUTPUT_CHANNELS

                int32_t acc = local_bias[c];
                const int in_y_origin = y * stride_h - pad_h;
                const int in_x_origin = x * stride_w - pad_w;

                for (int ky = 0; ky < kernel_h; ++ky) {
                    int in_y = in_y_origin + ky;
                    if (in_y >= 0 && in_y < input_h) {
                        for (int kx = 0; kx < kernel_w; ++kx) {
#pragma HLS UNROLL
                            int in_x = in_x_origin + kx;
                            if (in_x >= 0 && in_x < input_w) {
                                int in_idx = (in_y * input_w + in_x) * input_ch + c;
                                int32_t inp = (int32_t)input_data[in_idx] + input_offset;

                                int32_t w = (int32_t)local_weights[c][ky * kernel_w + kx] + weights_offset;
                                acc += inp * w;
                            }
                        }
                    }
                }

                acc = MultiplyByQuantizedMultiplier(acc, local_mult[c], local_shift[c]);
                acc += output_offset;
                int8_t res = clamp_s8(acc, act_min, act_max);

                int out_idx = (y * output_w + x) * output_ch + c;
                output_data[out_idx] = res;
            }
        }
    }
}

// ============================================================================
// TOP LEVEL (PHASE-1 IP)
// - AXI-Lite regs (bundle=CTRL) for runtime configuration
// - AXI-Stream OUT (m_axis) : streams the computed output buffer
// - Keeps existing m_axi ports so your current flow still works
// ============================================================================
void conv2d_blck(
    const int8_t  *input_data,
    const int8_t  *filter_data,
    const int32_t *bias_data,
    const int32_t *multiplier_data,
    const int32_t *shift_data,
    int8_t        *output_data,
    hls::stream<axis8_t> &out_stream,
    int conv_type,
    int h_in, int w_in,
    int c_in, int c_out,
    int h_out, int w_out,
    int stride,
    int padding,
    int32_t input_offset,
    int32_t weights_offset,
    int32_t output_offset,
    int act_min, int act_max
){
    // -------------------------
    // Interfaces
    // -------------------------
#pragma HLS INTERFACE m_axi     port=input_data      bundle=gmem0
#pragma HLS INTERFACE m_axi     port=output_data     bundle=gmem0
#pragma HLS INTERFACE m_axi     port=filter_data     bundle=gmem1
#pragma HLS INTERFACE m_axi     port=bias_data       bundle=gmem1
#pragma HLS INTERFACE m_axi     port=multiplier_data bundle=gmem1
#pragma HLS INTERFACE m_axi     port=shift_data      bundle=gmem1

#pragma HLS INTERFACE axis     port=out_stream

#pragma HLS INTERFACE s_axilite port=conv_type bundle=CTRL
#pragma HLS INTERFACE s_axilite port=h_in      bundle=CTRL
#pragma HLS INTERFACE s_axilite port=w_in      bundle=CTRL
#pragma HLS INTERFACE s_axilite port=c_in      bundle=CTRL
#pragma HLS INTERFACE s_axilite port=c_out     bundle=CTRL
#pragma HLS INTERFACE s_axilite port=h_out     bundle=CTRL
#pragma HLS INTERFACE s_axilite port=w_out     bundle=CTRL
#pragma HLS INTERFACE s_axilite port=stride    bundle=CTRL
#pragma HLS INTERFACE s_axilite port=padding   bundle=CTRL
#pragma HLS INTERFACE s_axilite port=act_min   bundle=CTRL
#pragma HLS INTERFACE s_axilite port=act_max   bundle=CTRL

// keep pointers controllable too (Vivado HLS will add them to reg map)
#pragma HLS INTERFACE s_axilite port=input_offset    bundle=CTRL
#pragma HLS INTERFACE s_axilite port=weights_offset  bundle=CTRL
#pragma HLS INTERFACE s_axilite port=output_offset   bundle=CTRL

#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    // -------------------------
    // Sanity checks (Phase-1 suggestion: keep MAX_* compile-time bounds)
    // -------------------------
    if (h_in  > MAX_IMAGE_WIDTH) return;
    if (w_in  > MAX_IMAGE_WIDTH) return;
    if (c_in  > MAX_INPUT_CHANNELS) return;
    if (c_out > MAX_OUTPUT_CHANNELS) return;
    if (h_out > MAX_IMAGE_WIDTH) return;
    if (w_out > MAX_IMAGE_WIDTH) return;

    // Derive stride/padding to existing compute_* expectations
    const int stride_h = stride;
    const int stride_w = stride;

    // Kernel selection:
    // - POINTWISE: 1x1
    // - STANDARD/DEPTHWISE: 3x3 (MAX_KERNEL_SIZE)
    const int kernel_h = (conv_type == POINTWISE) ? 1 : MAX_KERNEL_SIZE;
    const int kernel_w = (conv_type == POINTWISE) ? 1 : MAX_KERNEL_SIZE;

    int pad_h = 0;
    int pad_w = 0;
    if (padding == PADDING_SAME) {
        // SAME padding for odd kernels: pad = floor(k/2)
        pad_h = kernel_h / 2;
        pad_w = kernel_w / 2;
    }

    // -------------------------
    // Dispatcher (same as before)
    // -------------------------
    if (conv_type == POINTWISE) {
        compute_pointwise(
            input_data, filter_data, bias_data, multiplier_data, shift_data, output_data,
            h_in, w_in, c_in,
            h_out, w_out, c_out,
            input_offset, weights_offset, output_offset,
            act_min, act_max
        );
    } else if (conv_type == DEPTHWISE) {
        // Depthwise typically expects c_in == c_out
        compute_depthwise(
            input_data, filter_data, bias_data, multiplier_data, shift_data, output_data,
            h_in, w_in, c_in,
            h_out, w_out, c_out,
            kernel_h, kernel_w, stride_h, stride_w, pad_h, pad_w,
            input_offset, weights_offset, output_offset,
            act_min, act_max
        );
    } else { // STANDARD
        compute_standard(
            input_data, filter_data, bias_data, multiplier_data, shift_data, output_data,
            h_in, w_in, c_in,
            h_out, w_out, c_out,
            kernel_h, kernel_w, stride_h, stride_w, pad_h, pad_w,
            input_offset, weights_offset, output_offset,
            act_min, act_max
        );
    }

    // -------------------------
    // Stream out the result buffer (AXI-Stream mirror)
    // -------------------------
    const int out_elems = h_out * w_out * c_out;

    STREAM_OUT: for (int i = 0; i < out_elems; ++i) {
#pragma HLS PIPELINE II=1
    axis8_t t;
    t.data = (ap_int<8>)output_data[i];
    t.keep = 1;
    t.strb = 1;
    t.last = (i == out_elems - 1) ? 1 : 0;
    out_stream.write(t);
}
}
