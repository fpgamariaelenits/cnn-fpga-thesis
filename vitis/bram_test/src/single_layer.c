#include <stdint.h>

#define CONV_BASE 0x00010000
#define BRAM_BASE 0xC0000000

int main()
{
    volatile uint32_t *conv = (volatile uint32_t *)CONV_BASE;
    volatile int8_t   *bram8  = (volatile int8_t *)BRAM_BASE;
    volatile int32_t  *bram32 = (volatile int32_t *)BRAM_BASE;

    // Memory map inside BRAM
    // input      @ 0x0000
    // filter     @ 0x1000
    // bias       @ 0x1800
    // multiplier @ 0x1900
    // shift      @ 0x1A00
    // output     @ 0x2000

    // --------------------------------------------------
    // 1. Clear a small output area first
    // --------------------------------------------------
    for (int i = 0; i < 16; i++) {
        bram8[0x2000 + i] = 0;
    }

    // --------------------------------------------------
    // 2. Write INPUT data: 2x2x1
    // HWC layout
    // [1 2
    //  3 4]
    // --------------------------------------------------
    bram8[0x0000] = 1;
    bram8[0x0001] = 2;
    bram8[0x0002] = 3;
    bram8[0x0003] = 4;

    // --------------------------------------------------
    // 3. Write FILTER data: pointwise 1x1, 1 out ch, 1 in ch
    // weight = 2
    // --------------------------------------------------
    bram8[0x1000] = 4;

    // --------------------------------------------------
    // 4. Write BIAS
    // --------------------------------------------------
    bram32[0x1800/4] = 0;

    // --------------------------------------------------
    // 5. Write MULTIPLIER and SHIFT
    // IMPORTANT:
    // For now use these placeholder values.
    // --------------------------------------------------
    bram32[0x1900/4] = 1073741824;   // 2^30
    bram32[0x1A00/4] = 0;

    // --------------------------------------------------
    // 6. Program pointer registers
    // --------------------------------------------------
    conv[0x10/4] = BRAM_BASE + 0x0000; // input_data
    conv[0x1C/4] = BRAM_BASE + 0x1000; // filter_data
    conv[0x28/4] = BRAM_BASE + 0x1800; // bias_data
    conv[0x34/4] = BRAM_BASE + 0x1900; // multiplier_data
    conv[0x40/4] = BRAM_BASE + 0x1A00; // shift_data
    conv[0x4C/4] = BRAM_BASE + 0x2000; // output_data

    // --------------------------------------------------
    // 7. Program runtime parameters
    // POINTWISE = 2
    // --------------------------------------------------
    conv[0x58/4] = 0;     // conv_type = POINTWISE
    conv[0x60/4] = 2;     // h_in
    conv[0x68/4] = 2;     // w_in
    conv[0x70/4] = 1;     // c_in
    conv[0x78/4] = 1;     // c_out
    conv[0x80/4] = 2;     // h_out
    conv[0x88/4] = 2;     // w_out
    conv[0x90/4] = 1;     // stride
    conv[0x98/4] = 0;     // padding = VALID
    conv[0xA0/4] = 0;     // input_offset
    conv[0xA8/4] = 0;     // weights_offset
    conv[0xB0/4] = 0;     // output_offset
    conv[0xB8/4] = -128;  // act_min
    conv[0xC0/4] = 127;   // act_max

    // input_data
    conv[0x10/4] = BRAM_BASE + 0x0000;
    conv[0x14/4] = 0x00000000;

    // filter_data
    conv[0x1C/4] = BRAM_BASE + 0x1000;
    conv[0x20/4] = 0x00000000;

    // bias_data
    conv[0x28/4] = BRAM_BASE + 0x1800;
    conv[0x2C/4] = 0x00000000;

    // multiplier_data
    conv[0x34/4] = BRAM_BASE + 0x1900;
    conv[0x38/4] = 0x00000000;

    // shift_data
    conv[0x40/4] = BRAM_BASE + 0x1A00;
    conv[0x44/4] = 0x00000000;

    // output_data
    conv[0x4C/4] = BRAM_BASE + 0x2000;
    conv[0x50/4] = 0x00000000;

    // --------------------------------------------------
    // 8. Start accelerator
    // --------------------------------------------------
    conv[0x00/4] = 0x01;
    volatile uint32_t tmp = conv[0x00/4];

    // --------------------------------------------------
    // 9. Wait for done
    // ap_done bit = bit[1]
    // --------------------------------------------------
    while ((conv[0x00/4] & 0x2) == 0);

    // Stay here so you can inspect memory/registers
    while (1);

    return 0;
}
