#ifndef DATAOPERATE_H
#define DATAOPERATE_H
#include <stdio.h>
#include <stdlib.h>

int LoadFilter(char *filename_w,char *filename_b,int weight_len,int bias_len,float *dst);
void print_val(int size,float array[]);
void initPixelIn(float*pixel_in);

#endif
