#ifndef SRC_LENTET_H_
#define SRC_LENTET_H_

#include <stdio.h>
#include "xpl_axi.h"
#include "xil_printf.h"

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a>b)?b:a)

void RunLenet(XPl_axi *InstancePtr,float feature_in[],float filter1_in[],float filter2_in[],float feature_out[]);//mode: 0:VALID, 1:SAME

void RunLenet_soft(float feature_in[],float filter1_in[],float filter2_in[],float feature_out[]);//mode: 0:VALID, 1:SAME

#endif
