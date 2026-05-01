#ifndef TINY_POINTWISE_DATA_H
#define TINY_POINTWISE_DATA_H

#include <stdint.h>

static const int32_t stride_width = 1;
static const int32_t input_offset = 0;
static const int32_t weights_offset = 0;
static const int32_t output_offset = 0;
static const int32_t quantized_activation_min = -128;
static const int32_t quantized_activation_max = 127;

static const int32_t input_height = 2;
static const int32_t input_width  = 2;
static const int32_t input_depth  = 2;

static const int32_t output_height = 2;
static const int32_t output_width  = 2;
static const int32_t output_depth  = 2;

/* HWC layout: 2x2x2 = 8 values */
static const int8_t input_data[8] = {
    1, 2,
    3, 4,
    5, 6,
    7, 8
};

/* Pointwise weights: 1x1, Cout=2, Cin=2 => 4 values */
static const int8_t filter_data[4] = {
    1, 0,
    0, 1
};

static const int32_t bias_data[2] = {
    0, 0
};

static const int32_t multiplier_data[2] = {
    1073741824, 1073741824
};

static const int32_t shift_data[2] = {
    0, 0
};

#endif