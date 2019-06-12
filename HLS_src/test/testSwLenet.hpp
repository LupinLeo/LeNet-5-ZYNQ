#ifndef TESTSWLENET_H
#define TESTSWLENET_H

#include <stdio.h>

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a>b)?b:a)

void ConvLayer1_soft(float feature_in[],float filter_in[],float feature_out[]);
void ConvLayer2_soft(float feature_in[],float filter_in[],float feature_out[]);
void PoolLayer1_soft(float feature_in[],float feature_out[]);
void PoolLayer2_soft(float feature_in[],float feature_out[]);
#endif
