#include <stdio.h>
#include "pl_axi.hpp"
#include "param.hpp"
#include "testSwLenet.hpp"
#include "dataOperate.hpp"
//#define STREAM

int main(int argc, char** argv){
	int i, j, k;
	FILE *fp_in, *fp_out;
	fp_in = fopen("in.txt", "wb");
	fp_out = fopen("out.txt", "wb");

#ifndef STREAM
	float* in_data= (float*)malloc(Co1Layer_ROWS_I*Co1Layer_COLS_I*sizeof(float));
	float *out_data1= (float*)malloc(Co1Layer_ROWS_O*Co1Layer_COLS_O*Co1Layer_Num_O*sizeof(float));
	float *out_data2= (float*)malloc(Po1Layer_ROWS_O*Po1Layer_COLS_O*Po1Layer_Num_O*sizeof(float));
	float *out_data3= (float*)malloc(Co2Layer_ROWS_O*Co2Layer_COLS_O*Co2Layer_Num_O*sizeof(float));
	float *out_data4= (float*)malloc(Po2Layer_ROWS_O*Po2Layer_COLS_O*Po2Layer_Num_O*sizeof(float));
#else
	strm_u256 *in_data= (strm_u256*)malloc(ROWS_I*COLS_I*sizeof(strm_u256));
	strm_u512 *out_data = (strm_u512*)malloc(ROWS_O*COLS_O*sizeof(strm_u512));
#endif
	float *filter1 = (float *)malloc((Co1Layer_Num_I*Co1Layer_Num_O*FILTER_SIZE_C*FILTER_SIZE_C+Co1Layer_Num_O)*sizeof(float));
	float *filter2 = (float *)malloc((Co2Layer_Num_O*Co2Layer_Num_O*FILTER_SIZE_C*FILTER_SIZE_C+Co2Layer_Num_O)*sizeof(float));
	float conv1_out[11520];
	float pool1_out[2880];
	float conv2_out[3200];
	float pool2_out[800];
	printf("Initialize input data......\n");
	initPixelIn(in_data);
	LoadFilter("conv1_w.bin","conv1_b.bin",5*5*20,20,filter1);
	LoadFilter("conv2_w.bin","conv2_b.bin",5*5*20*50,50,filter2);
	//print_val(28*28,in_data);
	printf("Completed Initialize input data\n");
	fflush(stdout);
	PL_AXI(in_data,out_data4,filter1,filter2,0);
	ConvLayer1_soft(in_data,filter1,conv1_out);
	PoolLayer1_soft(conv1_out,pool1_out);
	ConvLayer2_soft(pool1_out,filter2,conv2_out);
	PoolLayer2_soft(conv2_out,pool2_out);
	printf("FPGA out_put\n");
	print_val(500,out_data4);
	printf("HLS out_put\n");
	print_val(500,pool2_out);

	/*	int offset = 0;
	printf("[HLS] Run pooling......"); fflush(stdout);
#ifndef STREAM
	PL_AXI(in_data,out_data4,filter1,filter2,0);
#else
	Hw_2DConv_32_16_Strm(in_data, out_data, filter64_in, 0);
#endif
	printf("Completed!\n"); fflush(stdout);


	printf("Print results of pl......\n"); fflush(stdout);
	for(k=0; k<Po2Layer_Num_O; k++){
		fprintf(fp_out, "====================================================== Output Feature Map %d ============================================================\n", k);
		for(i=0; i<Po2Layer_ROWS_O; i++){
			for(j=0; j<Po2Layer_COLS_O; j++){
				offset = Po2Layer_Num_O*(i*Po2Layer_COLS_O+j)+k;
#ifndef STREAM
				fprintf(fp_out, "%f ", out_data4[offset]);
#else
				fprintf(fp_out, "%d ", out_data[offset].data.range(k*32+31,k*32).to_int());
#endif
			}
			fprintf(fp_out, "\n");
		}
	}


	fclose(fp_in);
	fclose(fp_out);*/

	return 0;
}

