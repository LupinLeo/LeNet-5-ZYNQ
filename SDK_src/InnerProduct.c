#include "InnerProduct.h"

void RunInnerProduct(unsigned int InputNum,unsigned int OutputNum,unsigned int relu_en,float feature_in[],float weight[],float bias[],float feature_out[])
{

	//printf("Conv:out_width=%d,out_height=%d\n",out_width,out_height);
	for(int i=0;i<OutputNum;i++)
	{
		float result = bias[i];
		for(int j=0;j<InputNum;j++)
		{
			float data;
			data=feature_in[j];
			result+=data*weight[InputNum*i+j];
		}
		if(relu_en && result<0)
			result=0;
		feature_out[i]=result;
	}
}
