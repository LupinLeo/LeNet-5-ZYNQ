#ifndef __SD_H__
#define __SD_H__

#include "xstatus.h"
#include "xil_printf.h"
#include "ff.h"

int SD_Init();
int LoadWeight(char *filename,int dat_length,float *dst);
int LoadFilter(char *filename_w,char *filename_b,int weight_len,int bias_len,float *dst);
//int rdMNISTs28_test(u32 num, float img[28][28][1], int *label);

#endif
