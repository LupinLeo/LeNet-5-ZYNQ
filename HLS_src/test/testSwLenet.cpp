#include "testSwLenet.hpp"
void ConvLayer1_soft(float feature_in[],float filter_in[],float feature_out[])
{
	for(int i=0;i<20;i++)
		for(int j=0;j<24;j++)
			for(int k=0;k<24;k++)
			{
				float result=filter_in[26*i+25];
				for(int ki=0;ki<5;ki++)
					for(int kj=0;kj<5;kj++)
						for(int chi=0;chi<1;chi++)
						{
							float data;
							data=feature_in[chi*28*28+(j+ki)*28+(k+kj)];
							result+=data*filter_in[i*(1*25+1)+chi*25+5*ki+kj];
						}
				feature_out[j*24+k+i*24*24]=result;
				//printf ("c1out[%d] = %f\n",j*24+k+i*24*24,c1out[j*24+k+i*24*24]);
			}

}
void PoolLayer1_soft(float feature_in[],float feature_out[])
{
	for(int c=0;c<20;c++)
		for(int i=0;i<12;i++)
			for(int j=0;j<12;j++)
			{
				float sum = -99999999999999999;
				for(int ii=0;ii<2;ii++)
					for(int jj=0;jj<2;jj++)
					{
						int h=i*2+ii;
						int w=j*2+jj;
						sum = max(sum,feature_in[c*24*24+h*24+w]);
					}
				feature_out[c*12*12+i*12+j]=sum;
				//printf ("f1out[%d] = %f\n",c*12*12+i*12+j,f1out[c*12*12+i*12+j]);
			}
}
void ConvLayer2_soft(float feature_in[],float filter_in[],float feature_out[])
{
	for(int i=0;i<50;i++)
	{
			for(int j=0;j<8;j++)
			{
				for(int k=0;k<8;k++)
				{
					float result=filter_in[501*i+500];
					for(int ki=0;ki<5;ki++)
					{
						for(int kj=0;kj<5;kj++)
						{
							for(int chi=0;chi<20;chi++)
							{
								float data;
								data=feature_in[chi*12*12+(j+ki)*12+(k+kj)];
								//printf("filter[%d]:%f\t",i*(20*25+1)+chi*25+5*ki+kj,filter2_in[i*(20*25+1)+chi*25+5*ki+kj]);
								result+=data*filter_in[i*(20*25+1)+chi*25+5*ki+kj];
							}

						}
					}
					feature_out[j*8+k+i*8*8]=result;
					//printf ("c2out[%d] = %f\n",j*8+k+i*8*8,result);
				}
			}
	}
}
void PoolLayer2_soft(float feature_in[],float feature_out[])
{
		for(int c=0;c<50;c++)
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
				{
					float sum = -99999999999999999;
					for(int ii=0;ii<2;ii++)
						for(int jj=0;jj<2;jj++)
						{
							int h=i*2+ii;
							int w=j*2+jj;
							sum = max(sum,feature_in[c*8*8+h*8+w]);
						}
					feature_out[c*4*4+i*4+j]=sum;
					//printf ("p2out[%d] = %f\n",c*4*4+i*4+j,feature_out[c*4*4+i*4+j]);
				}

}
