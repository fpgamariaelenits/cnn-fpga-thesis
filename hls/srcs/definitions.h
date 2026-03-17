#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define MAX_INPUT_CHANNELS  64
#define MAX_OUTPUT_CHANNELS 64
#define MAX_KERNEL_SIZE     3

#define MAX_IMAGE_WIDTH 64

#define KERNEL_SIZE 1
#define HALF_KERNEL (KERNEL_SIZE/2)

#define STRIDE 2
#define PADDING_VALID 0
#define PADDING_SAME  1
#define PADDING_MODE  PADDING_SAME
  
typedef enum {
    STANDARD = 0,
    DEPTHWISE = 1,
    POINTWISE = 2
} ConvType;

typedef int data_t; 
#endif