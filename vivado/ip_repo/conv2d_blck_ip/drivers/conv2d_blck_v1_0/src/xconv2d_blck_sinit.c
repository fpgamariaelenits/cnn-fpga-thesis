// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2024.2 (64-bit)
// Tool Version Limit: 2024.11
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xconv2d_blck.h"

extern XConv2d_blck_Config XConv2d_blck_ConfigTable[];

#ifdef SDT
XConv2d_blck_Config *XConv2d_blck_LookupConfig(UINTPTR BaseAddress) {
	XConv2d_blck_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XConv2d_blck_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XConv2d_blck_ConfigTable[Index].Ctrl_BaseAddress == BaseAddress) {
			ConfigPtr = &XConv2d_blck_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XConv2d_blck_Initialize(XConv2d_blck *InstancePtr, UINTPTR BaseAddress) {
	XConv2d_blck_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XConv2d_blck_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XConv2d_blck_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XConv2d_blck_Config *XConv2d_blck_LookupConfig(u16 DeviceId) {
	XConv2d_blck_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XCONV2D_BLCK_NUM_INSTANCES; Index++) {
		if (XConv2d_blck_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XConv2d_blck_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XConv2d_blck_Initialize(XConv2d_blck *InstancePtr, u16 DeviceId) {
	XConv2d_blck_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XConv2d_blck_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XConv2d_blck_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

