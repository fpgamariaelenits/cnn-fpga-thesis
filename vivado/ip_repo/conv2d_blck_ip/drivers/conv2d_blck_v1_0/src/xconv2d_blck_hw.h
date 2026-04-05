// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
// CTRL
// 0x00 : Control signals
//        bit 0  - ap_start (Read/Write/COH)
//        bit 1  - ap_done (Read/COR)
//        bit 2  - ap_idle (Read)
//        bit 3  - ap_ready (Read/COR)
//        bit 7  - auto_restart (Read/Write)
//        bit 9  - interrupt (Read)
//        others - reserved
// 0x04 : Global Interrupt Enable Register
//        bit 0  - Global Interrupt Enable (Read/Write)
//        others - reserved
// 0x08 : IP Interrupt Enable Register (Read/Write)
//        bit 0 - enable ap_done interrupt (Read/Write)
//        bit 1 - enable ap_ready interrupt (Read/Write)
//        others - reserved
// 0x0c : IP Interrupt Status Register (Read/TOW)
//        bit 0 - ap_done (Read/TOW)
//        bit 1 - ap_ready (Read/TOW)
//        others - reserved
// 0x10 : Data signal of input_data
//        bit 31~0 - input_data[31:0] (Read/Write)
// 0x14 : Data signal of input_data
//        bit 31~0 - input_data[63:32] (Read/Write)
// 0x18 : reserved
// 0x1c : Data signal of filter_data
//        bit 31~0 - filter_data[31:0] (Read/Write)
// 0x20 : Data signal of filter_data
//        bit 31~0 - filter_data[63:32] (Read/Write)
// 0x24 : reserved
// 0x28 : Data signal of bias_data
//        bit 31~0 - bias_data[31:0] (Read/Write)
// 0x2c : Data signal of bias_data
//        bit 31~0 - bias_data[63:32] (Read/Write)
// 0x30 : reserved
// 0x34 : Data signal of multiplier_data
//        bit 31~0 - multiplier_data[31:0] (Read/Write)
// 0x38 : Data signal of multiplier_data
//        bit 31~0 - multiplier_data[63:32] (Read/Write)
// 0x3c : reserved
// 0x40 : Data signal of shift_data
//        bit 31~0 - shift_data[31:0] (Read/Write)
// 0x44 : Data signal of shift_data
//        bit 31~0 - shift_data[63:32] (Read/Write)
// 0x48 : reserved
// 0x4c : Data signal of output_data
//        bit 31~0 - output_data[31:0] (Read/Write)
// 0x50 : Data signal of output_data
//        bit 31~0 - output_data[63:32] (Read/Write)
// 0x54 : reserved
// 0x58 : Data signal of conv_type
//        bit 31~0 - conv_type[31:0] (Read/Write)
// 0x5c : reserved
// 0x60 : Data signal of h_in
//        bit 31~0 - h_in[31:0] (Read/Write)
// 0x64 : reserved
// 0x68 : Data signal of w_in
//        bit 31~0 - w_in[31:0] (Read/Write)
// 0x6c : reserved
// 0x70 : Data signal of c_in
//        bit 31~0 - c_in[31:0] (Read/Write)
// 0x74 : reserved
// 0x78 : Data signal of c_out
//        bit 31~0 - c_out[31:0] (Read/Write)
// 0x7c : reserved
// 0x80 : Data signal of h_out
//        bit 31~0 - h_out[31:0] (Read/Write)
// 0x84 : reserved
// 0x88 : Data signal of w_out
//        bit 31~0 - w_out[31:0] (Read/Write)
// 0x8c : reserved
// 0x90 : Data signal of stride
//        bit 31~0 - stride[31:0] (Read/Write)
// 0x94 : reserved
// 0x98 : Data signal of padding
//        bit 31~0 - padding[31:0] (Read/Write)
// 0x9c : reserved
// 0xa0 : Data signal of input_offset
//        bit 31~0 - input_offset[31:0] (Read/Write)
// 0xa4 : reserved
// 0xa8 : Data signal of weights_offset
//        bit 31~0 - weights_offset[31:0] (Read/Write)
// 0xac : reserved
// 0xb0 : Data signal of output_offset
//        bit 31~0 - output_offset[31:0] (Read/Write)
// 0xb4 : reserved
// 0xb8 : Data signal of act_min
//        bit 31~0 - act_min[31:0] (Read/Write)
// 0xbc : reserved
// 0xc0 : Data signal of act_max
//        bit 31~0 - act_max[31:0] (Read/Write)
// 0xc4 : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XCONV2D_BLCK_CTRL_ADDR_AP_CTRL              0x00
#define XCONV2D_BLCK_CTRL_ADDR_GIE                  0x04
#define XCONV2D_BLCK_CTRL_ADDR_IER                  0x08
#define XCONV2D_BLCK_CTRL_ADDR_ISR                  0x0c
#define XCONV2D_BLCK_CTRL_ADDR_INPUT_DATA_DATA      0x10
#define XCONV2D_BLCK_CTRL_BITS_INPUT_DATA_DATA      64
#define XCONV2D_BLCK_CTRL_ADDR_FILTER_DATA_DATA     0x1c
#define XCONV2D_BLCK_CTRL_BITS_FILTER_DATA_DATA     64
#define XCONV2D_BLCK_CTRL_ADDR_BIAS_DATA_DATA       0x28
#define XCONV2D_BLCK_CTRL_BITS_BIAS_DATA_DATA       64
#define XCONV2D_BLCK_CTRL_ADDR_MULTIPLIER_DATA_DATA 0x34
#define XCONV2D_BLCK_CTRL_BITS_MULTIPLIER_DATA_DATA 64
#define XCONV2D_BLCK_CTRL_ADDR_SHIFT_DATA_DATA      0x40
#define XCONV2D_BLCK_CTRL_BITS_SHIFT_DATA_DATA      64
#define XCONV2D_BLCK_CTRL_ADDR_OUTPUT_DATA_DATA     0x4c
#define XCONV2D_BLCK_CTRL_BITS_OUTPUT_DATA_DATA     64
#define XCONV2D_BLCK_CTRL_ADDR_CONV_TYPE_DATA       0x58
#define XCONV2D_BLCK_CTRL_BITS_CONV_TYPE_DATA       32
#define XCONV2D_BLCK_CTRL_ADDR_H_IN_DATA            0x60
#define XCONV2D_BLCK_CTRL_BITS_H_IN_DATA            32
#define XCONV2D_BLCK_CTRL_ADDR_W_IN_DATA            0x68
#define XCONV2D_BLCK_CTRL_BITS_W_IN_DATA            32
#define XCONV2D_BLCK_CTRL_ADDR_C_IN_DATA            0x70
#define XCONV2D_BLCK_CTRL_BITS_C_IN_DATA            32
#define XCONV2D_BLCK_CTRL_ADDR_C_OUT_DATA           0x78
#define XCONV2D_BLCK_CTRL_BITS_C_OUT_DATA           32
#define XCONV2D_BLCK_CTRL_ADDR_H_OUT_DATA           0x80
#define XCONV2D_BLCK_CTRL_BITS_H_OUT_DATA           32
#define XCONV2D_BLCK_CTRL_ADDR_W_OUT_DATA           0x88
#define XCONV2D_BLCK_CTRL_BITS_W_OUT_DATA           32
#define XCONV2D_BLCK_CTRL_ADDR_STRIDE_DATA          0x90
#define XCONV2D_BLCK_CTRL_BITS_STRIDE_DATA          32
#define XCONV2D_BLCK_CTRL_ADDR_PADDING_DATA         0x98
#define XCONV2D_BLCK_CTRL_BITS_PADDING_DATA         32
#define XCONV2D_BLCK_CTRL_ADDR_INPUT_OFFSET_DATA    0xa0
#define XCONV2D_BLCK_CTRL_BITS_INPUT_OFFSET_DATA    32
#define XCONV2D_BLCK_CTRL_ADDR_WEIGHTS_OFFSET_DATA  0xa8
#define XCONV2D_BLCK_CTRL_BITS_WEIGHTS_OFFSET_DATA  32
#define XCONV2D_BLCK_CTRL_ADDR_OUTPUT_OFFSET_DATA   0xb0
#define XCONV2D_BLCK_CTRL_BITS_OUTPUT_OFFSET_DATA   32
#define XCONV2D_BLCK_CTRL_ADDR_ACT_MIN_DATA         0xb8
#define XCONV2D_BLCK_CTRL_BITS_ACT_MIN_DATA         32
#define XCONV2D_BLCK_CTRL_ADDR_ACT_MAX_DATA         0xc0
#define XCONV2D_BLCK_CTRL_BITS_ACT_MAX_DATA         32

