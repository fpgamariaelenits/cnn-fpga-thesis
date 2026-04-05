// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xconv2d_blck.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XConv2d_blck_CfgInitialize(XConv2d_blck *InstancePtr, XConv2d_blck_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Ctrl_BaseAddress = ConfigPtr->Ctrl_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XConv2d_blck_Start(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL) & 0x80;
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL, Data | 0x01);
}

u32 XConv2d_blck_IsDone(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XConv2d_blck_IsIdle(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XConv2d_blck_IsReady(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XConv2d_blck_EnableAutoRestart(XConv2d_blck *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL, 0x80);
}

void XConv2d_blck_DisableAutoRestart(XConv2d_blck *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_AP_CTRL, 0);
}

void XConv2d_blck_Set_input_data(XConv2d_blck *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_INPUT_DATA_DATA, (u32)(Data));
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_INPUT_DATA_DATA + 4, (u32)(Data >> 32));
}

u64 XConv2d_blck_Get_input_data(XConv2d_blck *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_INPUT_DATA_DATA);
    Data += (u64)XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_INPUT_DATA_DATA + 4) << 32;
    return Data;
}

void XConv2d_blck_Set_filter_data(XConv2d_blck *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_FILTER_DATA_DATA, (u32)(Data));
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_FILTER_DATA_DATA + 4, (u32)(Data >> 32));
}

u64 XConv2d_blck_Get_filter_data(XConv2d_blck *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_FILTER_DATA_DATA);
    Data += (u64)XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_FILTER_DATA_DATA + 4) << 32;
    return Data;
}

void XConv2d_blck_Set_bias_data(XConv2d_blck *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_BIAS_DATA_DATA, (u32)(Data));
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_BIAS_DATA_DATA + 4, (u32)(Data >> 32));
}

u64 XConv2d_blck_Get_bias_data(XConv2d_blck *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_BIAS_DATA_DATA);
    Data += (u64)XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_BIAS_DATA_DATA + 4) << 32;
    return Data;
}

void XConv2d_blck_Set_multiplier_data(XConv2d_blck *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_MULTIPLIER_DATA_DATA, (u32)(Data));
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_MULTIPLIER_DATA_DATA + 4, (u32)(Data >> 32));
}

u64 XConv2d_blck_Get_multiplier_data(XConv2d_blck *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_MULTIPLIER_DATA_DATA);
    Data += (u64)XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_MULTIPLIER_DATA_DATA + 4) << 32;
    return Data;
}

void XConv2d_blck_Set_shift_data(XConv2d_blck *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_SHIFT_DATA_DATA, (u32)(Data));
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_SHIFT_DATA_DATA + 4, (u32)(Data >> 32));
}

u64 XConv2d_blck_Get_shift_data(XConv2d_blck *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_SHIFT_DATA_DATA);
    Data += (u64)XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_SHIFT_DATA_DATA + 4) << 32;
    return Data;
}

void XConv2d_blck_Set_output_data(XConv2d_blck *InstancePtr, u64 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_OUTPUT_DATA_DATA, (u32)(Data));
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_OUTPUT_DATA_DATA + 4, (u32)(Data >> 32));
}

u64 XConv2d_blck_Get_output_data(XConv2d_blck *InstancePtr) {
    u64 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_OUTPUT_DATA_DATA);
    Data += (u64)XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_OUTPUT_DATA_DATA + 4) << 32;
    return Data;
}

void XConv2d_blck_Set_conv_type(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_CONV_TYPE_DATA, Data);
}

u32 XConv2d_blck_Get_conv_type(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_CONV_TYPE_DATA);
    return Data;
}

void XConv2d_blck_Set_h_in(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_H_IN_DATA, Data);
}

u32 XConv2d_blck_Get_h_in(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_H_IN_DATA);
    return Data;
}

void XConv2d_blck_Set_w_in(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_W_IN_DATA, Data);
}

u32 XConv2d_blck_Get_w_in(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_W_IN_DATA);
    return Data;
}

void XConv2d_blck_Set_c_in(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_C_IN_DATA, Data);
}

u32 XConv2d_blck_Get_c_in(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_C_IN_DATA);
    return Data;
}

void XConv2d_blck_Set_c_out(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_C_OUT_DATA, Data);
}

u32 XConv2d_blck_Get_c_out(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_C_OUT_DATA);
    return Data;
}

void XConv2d_blck_Set_h_out(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_H_OUT_DATA, Data);
}

u32 XConv2d_blck_Get_h_out(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_H_OUT_DATA);
    return Data;
}

void XConv2d_blck_Set_w_out(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_W_OUT_DATA, Data);
}

u32 XConv2d_blck_Get_w_out(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_W_OUT_DATA);
    return Data;
}

void XConv2d_blck_Set_stride(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_STRIDE_DATA, Data);
}

u32 XConv2d_blck_Get_stride(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_STRIDE_DATA);
    return Data;
}

void XConv2d_blck_Set_padding(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_PADDING_DATA, Data);
}

u32 XConv2d_blck_Get_padding(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_PADDING_DATA);
    return Data;
}

void XConv2d_blck_Set_input_offset(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_INPUT_OFFSET_DATA, Data);
}

u32 XConv2d_blck_Get_input_offset(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_INPUT_OFFSET_DATA);
    return Data;
}

void XConv2d_blck_Set_weights_offset(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_WEIGHTS_OFFSET_DATA, Data);
}

u32 XConv2d_blck_Get_weights_offset(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_WEIGHTS_OFFSET_DATA);
    return Data;
}

void XConv2d_blck_Set_output_offset(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_OUTPUT_OFFSET_DATA, Data);
}

u32 XConv2d_blck_Get_output_offset(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_OUTPUT_OFFSET_DATA);
    return Data;
}

void XConv2d_blck_Set_act_min(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_ACT_MIN_DATA, Data);
}

u32 XConv2d_blck_Get_act_min(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_ACT_MIN_DATA);
    return Data;
}

void XConv2d_blck_Set_act_max(XConv2d_blck *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_ACT_MAX_DATA, Data);
}

u32 XConv2d_blck_Get_act_max(XConv2d_blck *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_ACT_MAX_DATA);
    return Data;
}

void XConv2d_blck_InterruptGlobalEnable(XConv2d_blck *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_GIE, 1);
}

void XConv2d_blck_InterruptGlobalDisable(XConv2d_blck *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_GIE, 0);
}

void XConv2d_blck_InterruptEnable(XConv2d_blck *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_IER);
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_IER, Register | Mask);
}

void XConv2d_blck_InterruptDisable(XConv2d_blck *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_IER);
    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_IER, Register & (~Mask));
}

void XConv2d_blck_InterruptClear(XConv2d_blck *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XConv2d_blck_WriteReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_ISR, Mask);
}

u32 XConv2d_blck_InterruptGetEnabled(XConv2d_blck *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_IER);
}

u32 XConv2d_blck_InterruptGetStatus(XConv2d_blck *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XConv2d_blck_ReadReg(InstancePtr->Ctrl_BaseAddress, XCONV2D_BLCK_CTRL_ADDR_ISR);
}

