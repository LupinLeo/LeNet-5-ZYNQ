#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_cache.h"

#include "InnerProduct.h"
#include "Lentet.h"
#include "sd.h"

float mean [28][28];
//Weight of Conv1
float pixel_in[28][28][1];
float pixel[28][28][1];
float conv1_filter[20][26];
//Weight of Conv2
float conv2_filter[50][501];
float pool2_out[800];
float pool2_out_true[800];
float pool2_out_soft[800];

//Weight of FC1
float ip1_weight[400000];
float ip1_bias[500];
float ip1_out[500];
float ip1_out_soft[500];

//Weight of FC2
float ip2_weight[5000];
float ip2_bias[10];
float ip2_out[10];
float ip2_out_soft[10];


int rdMNISTs28_test(u32 num, float img[28][28][1],float mean[28][28], int *label);
void print_val(int size,float array[]);
int main()
{
//	char str[50];

    init_platform();
    Xil_DCacheDisable();

    XPl_axi xpl_axi;
    if(XPl_axi_Initialize(&xpl_axi,XPAR_PL_AXI_0_DEVICE_ID)!=XST_SUCCESS)
    	xil_printf("XConv device not found\r\n");


    SD_Init();
    xil_printf("Hello World\r\n");

    LoadFilter("conv1_w.bin","conv1_b.bin",5*5*20,20,conv1_filter[0]);
    LoadFilter("conv2_w.bin","conv2_b.bin",5*5*20*50,50,conv2_filter[0]);

    LoadWeight("ip1_w.bin",500*800,ip1_weight);
    LoadWeight("ip1_b.bin",500,ip1_bias);

    LoadWeight("ip2_w.bin",500*10,ip2_weight);
    LoadWeight("ip2_b.bin",10,ip2_bias);
    LoadWeight("mean.bin",28*28,mean[0]);
    //print_val(520,conv1_filter[0]);
    //print_val(50*501,conv2_filter[0]);
    //print_val(400000,ip1_weight);
    //print_val(500,ip1_bias);
    //print_val(5000,ip2_weight);
    //while(1)
    for(int i=0;i<10000;i++)
    {
		int label;
		rdMNISTs28_test(i, pixel_in,mean, &label);
		print_val(28*28,pixel_in);
		for(int i =0;i<28;i++)
			for(int j =0;j<28;j++)
			{
				pixel[i][j][0] = pixel_in[j][i][0];
			}
//		for(int j=0;j<28*28;j++)
//		{
//			unsigned char tp=inbyte();
//			image[j/28][j%28][0]= (tp*1.0)/255;
//			xil_printf("Getchar=%x\r\n",tp);
//		}
		RunLenet_soft(pixel_in[0][0],conv1_filter[0],conv2_filter[0],pool2_out_soft);
		//RunLenet(&xpl_axi,pixel_in[0][0],conv1_filter[0],conv2_filter[0],pool2_out);
		for(int i =0;i<16;i++)
			for(int j =0;j<50;j++)
			{
				pool2_out_true[j*16+i] = pool2_out[i*50+j];
			}
		//print_val(800,pool2_out_soft);
		//print_val(800,pool2_out);
		//RunInnerProduct(800,500,1,pool2_out_true,ip1_weight,ip1_bias,ip1_out);
		RunInnerProduct(800,500,1,pool2_out_soft,ip1_weight,ip1_bias,ip1_out_soft);
		//print_val(500,ip1_out);
		//RunInnerProduct(500,10,0,ip1_out,ip2_weight,ip2_bias,ip2_out);
		RunInnerProduct(500,10,0,ip1_out_soft,ip2_weight,ip2_bias,ip2_out_soft);
		//print_val(10,ip2_out);
		float max=-10000;float max_soft = -10000; int num=0; int numSoft =0;
		for(int m=0;m<10;m++)
		{
			//if(ip2_out[m]>max)
			//{
				//max=ip2_out[m];
				//num=m;
			//}

			if(ip2_out_soft[m]>max_soft)
			{
				max_soft=ip2_out_soft[m];
				numSoft=m;
			}
			//char str[50];
			//sprintf(str,"ip2_out[%d]=%f\r\n",m,ip2_out[m]);
	    	//xil_printf("ip2_out[%d]=%d\r\n",m,ip2_out[m]);
		}
		//xil_printf("predicted=%d, label=%d\r\n",num,label);
		xil_printf("soft predicted=%d, label=%d\r\n",numSoft,label);
    }

    cleanup_platform();
    return 0;
}
void print_val(int size,float array[])
{
	for(int i =0;i<size;i++)
	printf (" array[%d] = %f;\n",i,array[i]);
}
int rdMNISTs28_test(u32 num, float img[28][28][1],float mean[28][28], int *label)
{
	FIL fil;
	FRESULT rc;
	UINT bw;

	char imagefile[] = "Tsimgs.x3";
	char labelfile[] = "Tslbls.x1";
	rc = f_open(&fil, imagefile, FA_READ);
	if (rc)
	{
		xil_printf("Error %d: Can't Open %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}
	rc = f_lseek(&fil, num * 784 + 16);
	if (rc)
	{
		xil_printf("Error %d: Can't seek %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}

	u8 picture[28][28];
	int j,k;
	for (j = 0; j < 28; j++)
	{
		rc = f_read(&fil, picture[j], 28, &bw);
		if (rc)
		{
			xil_printf("Error %d: Can't read %s!\r\n", rc, imagefile);
			return XST_FAILURE;
		}
		for (k = 0; k < 28; k++)
		{
			img[j][k][0] = (picture[j][k]-mean[j][k])/255.0;
		}
	}
	rc = f_close(&fil);
	if (rc)
	{
		xil_printf("Error %d: Can't close %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}
	rc = f_open(&fil, labelfile, FA_READ);
	if (rc)
	{
		xil_printf("Error %d: Can't Open %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}
	rc = f_lseek(&fil, num + 8);
	if (rc)
	{
		xil_printf("Error %d: Can't seek %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}
	u8 rtn;
	rc = f_read(&fil, &rtn, 1, &bw);
	if (rc)
	{
		xil_printf("Error %d: Can't read %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}
	*label = rtn;

	rc = f_close(&fil);
	if (rc)
	{
		xil_printf("Error %d: Can't close %s!\r\n", rc, imagefile);
		return XST_FAILURE;
	}
	//xil_printf("Read MNIST succeed!\r\n");
	return XST_SUCCESS;

}
