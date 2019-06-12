#ifndef SRC_INNERPRODUCT_H_
#define SRC_INNERPRODUCT_H_

#include <stdio.h>
#include "xil_printf.h"



void RunInnerProduct(unsigned int InputNum,unsigned int OutputNum,unsigned int relu_en,float feature_in[],float weight[],float bias[],float feature_out[]);//mode: 0:MEAN, 1:MIN, 2:MAX

#endif /* SRC_INNERPRODUCT_H_ */
