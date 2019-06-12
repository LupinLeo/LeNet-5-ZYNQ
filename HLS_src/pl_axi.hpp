#ifndef _PL_AXI_H
#define _PL_AXI_H

#include <stdio.h>
#include <ap_int.h>
#include <stdint.h>
#include <hls_stream.h>
#include "buffer.hpp"
#include "param.hpp"
#include "string.h"
#include "hls_dsp.h"

using namespace std;


template<int D>
struct ap_axis{
  ap_uint<D>   data;
  ap_uint<D/8> keep;
  ap_uint<1>   last;
};



typedef struct ap_axis<256> strm_u256;
typedef struct ap_axis<512> strm_u512;
typedef struct filter_1g  FilterEE;


void PL_AXI(float *pixel_in, float *pixel_out, float *filter1_in, float *filter2_in,int32_t addr_reserved);

#endif
