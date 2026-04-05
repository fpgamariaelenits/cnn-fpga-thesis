// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef XCONV2D_BLCK_H
#define XCONV2D_BLCK_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xconv2d_blck_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
#else
typedef struct {
#ifdef SDT
    char *Name;
#else
    u16 DeviceId;
#endif
    u64 Ctrl_BaseAddress;
} XConv2d_blck_Config;
#endif

typedef struct {
    u64 Ctrl_BaseAddress;
    u32 IsReady;
} XConv2d_blck;

typedef u32 word_type;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XConv2d_blck_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XConv2d_blck_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XConv2d_blck_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XConv2d_blck_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
#ifdef SDT
int XConv2d_blck_Initialize(XConv2d_blck *InstancePtr, UINTPTR BaseAddress);
XConv2d_blck_Config* XConv2d_blck_LookupConfig(UINTPTR BaseAddress);
#else
int XConv2d_blck_Initialize(XConv2d_blck *InstancePtr, u16 DeviceId);
XConv2d_blck_Config* XConv2d_blck_LookupConfig(u16 DeviceId);
#endif
int XConv2d_blck_CfgInitialize(XConv2d_blck *InstancePtr, XConv2d_blck_Config *ConfigPtr);
#else
int XConv2d_blck_Initialize(XConv2d_blck *InstancePtr, const char* InstanceName);
int XConv2d_blck_Release(XConv2d_blck *InstancePtr);
#endif

void XConv2d_blck_Start(XConv2d_blck *InstancePtr);
u32 XConv2d_blck_IsDone(XConv2d_blck *InstancePtr);
u32 XConv2d_blck_IsIdle(XConv2d_blck *InstancePtr);
u32 XConv2d_blck_IsReady(XConv2d_blck *InstancePtr);
void XConv2d_blck_EnableAutoRestart(XConv2d_blck *InstancePtr);
void XConv2d_blck_DisableAutoRestart(XConv2d_blck *InstancePtr);

void XConv2d_blck_Set_input_data(XConv2d_blck *InstancePtr, u64 Data);
u64 XConv2d_blck_Get_input_data(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_filter_data(XConv2d_blck *InstancePtr, u64 Data);
u64 XConv2d_blck_Get_filter_data(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_bias_data(XConv2d_blck *InstancePtr, u64 Data);
u64 XConv2d_blck_Get_bias_data(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_multiplier_data(XConv2d_blck *InstancePtr, u64 Data);
u64 XConv2d_blck_Get_multiplier_data(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_shift_data(XConv2d_blck *InstancePtr, u64 Data);
u64 XConv2d_blck_Get_shift_data(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_output_data(XConv2d_blck *InstancePtr, u64 Data);
u64 XConv2d_blck_Get_output_data(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_conv_type(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_conv_type(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_h_in(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_h_in(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_w_in(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_w_in(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_c_in(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_c_in(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_c_out(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_c_out(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_h_out(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_h_out(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_w_out(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_w_out(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_stride(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_stride(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_padding(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_padding(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_input_offset(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_input_offset(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_weights_offset(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_weights_offset(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_output_offset(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_output_offset(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_act_min(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_act_min(XConv2d_blck *InstancePtr);
void XConv2d_blck_Set_act_max(XConv2d_blck *InstancePtr, u32 Data);
u32 XConv2d_blck_Get_act_max(XConv2d_blck *InstancePtr);

void XConv2d_blck_InterruptGlobalEnable(XConv2d_blck *InstancePtr);
void XConv2d_blck_InterruptGlobalDisable(XConv2d_blck *InstancePtr);
void XConv2d_blck_InterruptEnable(XConv2d_blck *InstancePtr, u32 Mask);
void XConv2d_blck_InterruptDisable(XConv2d_blck *InstancePtr, u32 Mask);
void XConv2d_blck_InterruptClear(XConv2d_blck *InstancePtr, u32 Mask);
u32 XConv2d_blck_InterruptGetEnabled(XConv2d_blck *InstancePtr);
u32 XConv2d_blck_InterruptGetStatus(XConv2d_blck *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
