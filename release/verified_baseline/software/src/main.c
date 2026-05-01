#include <stdint.h>
#include "tiny_pointwise.h"

#define CONV_BASE 0x00010000u
#define BRAM_BASE 0xC0000000u

/* Large, safe BRAM layout for real pointwise test */
#define IN_OFF   0x0000u
#define W_OFF    0x1000u
#define B_OFF    0x1800u
#define M_OFF    0x1900u
#define S_OFF    0x1A00u
#define OUT_OFF  0x2000u

/* AXI-Lite register offsets */
#define REG_AP_CTRL        0x00u

#define REG_INPUT_DATA_L   0x10u
#define REG_INPUT_DATA_H   0x14u
#define REG_FILTER_DATA_L  0x1Cu
#define REG_FILTER_DATA_H  0x20u
#define REG_BIAS_DATA_L    0x28u
#define REG_BIAS_DATA_H    0x2Cu
#define REG_MULT_DATA_L    0x34u
#define REG_MULT_DATA_H    0x38u
#define REG_SHIFT_DATA_L   0x40u
#define REG_SHIFT_DATA_H   0x44u
#define REG_OUTPUT_DATA_L  0x4Cu
#define REG_OUTPUT_DATA_H  0x50u

#define REG_CONV_TYPE      0x58u
#define REG_H_IN           0x60u
#define REG_W_IN           0x68u
#define REG_C_IN           0x70u
#define REG_C_OUT          0x78u
#define REG_H_OUT          0x80u
#define REG_W_OUT          0x88u
#define REG_STRIDE         0x90u
#define REG_PADDING        0x98u
#define REG_INPUT_OFFSET   0xA0u
#define REG_WEIGHTS_OFFSET 0xA8u
#define REG_OUTPUT_OFFSET  0xB0u
#define REG_ACT_MIN        0xB8u
#define REG_ACT_MAX        0xC0u

#define POINTWISE_TYPE     2u
#define PADDING_VALID      0u

static void write_ptr64(volatile uint32_t *conv, uint32_t reg_low, uint32_t addr32)
{
    conv[reg_low / 4u] = addr32;
    conv[(reg_low + 4u) / 4u] = 0x00000000u;
}

int main()
{
    volatile uint32_t *conv = (volatile uint32_t *)CONV_BASE;

    const uint32_t input_addr  = BRAM_BASE + IN_OFF;
    const uint32_t filter_addr = BRAM_BASE + W_OFF;
    const uint32_t bias_addr   = BRAM_BASE + B_OFF;
    const uint32_t mult_addr   = BRAM_BASE + M_OFF;
    const uint32_t shift_addr  = BRAM_BASE + S_OFF;
    const uint32_t output_addr = BRAM_BASE + OUT_OFF;

    const int input_elems  = (int)(sizeof(input_data) / sizeof(input_data[0]));
    const int filter_elems = (int)(sizeof(filter_data) / sizeof(filter_data[0]));
    const int bias_elems   = (int)(sizeof(bias_data) / sizeof(bias_data[0]));
    const int mult_elems   = (int)(sizeof(multiplier_data) / sizeof(multiplier_data[0]));
    const int shift_elems  = (int)(sizeof(shift_data) / sizeof(shift_data[0]));
    const int output_elems = output_height * output_width * output_depth;

    int i;

    /* 1. Clear output buffer */
    for (i = 0; i < output_elems; i++) {
        ((volatile int8_t *)(BRAM_BASE + OUT_OFF))[i] = 0;
    }

    /* 2. Copy input tensor to BRAM */
    for (i = 0; i < input_elems; i++) {
        ((volatile int8_t *)(BRAM_BASE + IN_OFF))[i] = input_data[i];
    }

    /* 3. Copy filter tensor to BRAM */
    for (i = 0; i < filter_elems; i++) {
        ((volatile int8_t *)(BRAM_BASE + W_OFF))[i] = filter_data[i];
    }

    /* 4. Copy bias tensor to BRAM */
    for (i = 0; i < bias_elems; i++) {
        ((volatile int32_t *)(BRAM_BASE + B_OFF))[i] = bias_data[i];
    }

    /* 5. Copy multiplier tensor to BRAM */
    for (i = 0; i < mult_elems; i++) {
        ((volatile int32_t *)(BRAM_BASE + M_OFF))[i] = multiplier_data[i];
    }

    /* 6. Copy shift tensor to BRAM */
    for (i = 0; i < shift_elems; i++) {
        ((volatile int32_t *)(BRAM_BASE + S_OFF))[i] = shift_data[i];
    }

    /* 7. Program pointer registers (64-bit args -> low/high words) */
    write_ptr64(conv, REG_INPUT_DATA_L,  input_addr);
    write_ptr64(conv, REG_FILTER_DATA_L, filter_addr);
    write_ptr64(conv, REG_BIAS_DATA_L,   bias_addr);
    write_ptr64(conv, REG_MULT_DATA_L,   mult_addr);
    write_ptr64(conv, REG_SHIFT_DATA_L,  shift_addr);
    write_ptr64(conv, REG_OUTPUT_DATA_L, output_addr);

    /* 8. Program runtime parameters from pointwise header */
    conv[REG_CONV_TYPE      / 4u] = POINTWISE_TYPE;
    conv[REG_H_IN           / 4u] = (uint32_t)input_height;
    conv[REG_W_IN           / 4u] = (uint32_t)input_width;
    conv[REG_C_IN           / 4u] = (uint32_t)input_depth;
    conv[REG_C_OUT          / 4u] = (uint32_t)output_depth;
    conv[REG_H_OUT          / 4u] = (uint32_t)output_height;
    conv[REG_W_OUT          / 4u] = (uint32_t)output_width;
    conv[REG_STRIDE         / 4u] = (uint32_t)stride_width;
    conv[REG_PADDING        / 4u] = PADDING_VALID;
    conv[REG_INPUT_OFFSET   / 4u] = (uint32_t)input_offset;
    conv[REG_WEIGHTS_OFFSET / 4u] = (uint32_t)weights_offset;
    conv[REG_OUTPUT_OFFSET  / 4u] = (uint32_t)output_offset;
    conv[REG_ACT_MIN        / 4u] = (uint32_t)quantized_activation_min;
    conv[REG_ACT_MAX        / 4u] = (uint32_t)quantized_activation_max;

    /* 9. Start accelerator */
    conv[REG_AP_CTRL / 4u] = 0x01u;

    /* 10. Wait for done (bit1 = ap_done) */
    while ((conv[REG_AP_CTRL / 4u] & 0x2u) == 0u) {
    }

    /* 11. Stay here for inspection */
    while (1) {
    }

    return 0;
}