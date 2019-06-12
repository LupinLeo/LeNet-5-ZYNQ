#include "sd.h"

static FATFS fatfs;

int SD_Init()
{
	FRESULT rc;
	rc = f_mount(&fatfs, "", 0);
	if(rc)
	{
		xil_printf("Error : f_mount returned %d\r\n.", rc);
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int LoadFilter(char *filename_w,char *filename_b,int weight_len,int bias_len,float *dst)
{
	FIL FP_w,FP_b;
	FRESULT result_w,result_b;//0: success
	UINT bw;

	result_w = f_open(&FP_w, filename_w, FA_READ);

	if (result_w)
	{
		xil_printf("Can't Open %s\r\n", filename_w);
		return XST_FAILURE;
	}
	result_b = f_open(&FP_b, filename_b, FA_READ);

	if (result_b)
	{
		xil_printf("Can't Open %s\r\n", filename_b);
		return XST_FAILURE;
	}
	for(int i =0;i<bias_len;i++)
	{
		result_w = f_read(&FP_w, dst+i*(weight_len/bias_len+1), weight_len/bias_len*sizeof(float) , &bw);
		if (result_w)
		{
			xil_printf("Can't read %s!\r\n", filename_w);
			return XST_FAILURE;
		}
		result_b = f_read(&FP_b, dst+(i+1)*weight_len/bias_len+i,1*sizeof(float) , &bw);
		if (result_b)
		{
			xil_printf("Can't read %s!\r\n", filename_b);
			return XST_FAILURE;
		}
	}


	f_close(&FP_w);
	f_close(&FP_b);
	return XST_SUCCESS;
}

int LoadWeight(char *filename,int dat_length,float *dst)
{
	FIL FP;
	FRESULT result;//0: success
	UINT bw;

	result = f_open(&FP, filename, FA_READ);
	if (result)
	{
		xil_printf("Can't Open %s\r\n", filename);
		return XST_FAILURE;
	}

	result = f_read(&FP, dst, dat_length*sizeof(float) , &bw);
	if (result)
	{
		xil_printf("Can't read %s!\r\n", filename);
		return XST_FAILURE;
	}

	f_close(&FP);
	return XST_SUCCESS;
}



