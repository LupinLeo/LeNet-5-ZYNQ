#include "pl_axi.hpp"



void Conv1_4(d_window<float,FILTER_SIZE_C,FILTER_SIZE_C> win_buffer, float *filter_in1, float *filter_in2, float *filter_in3, float *filter_in4,float *res)
{
	uint16_t m, n, i, j;
	float store_data0 =filter_in1[FILTER_SIZE_C*FILTER_SIZE_C] ;
	float store_data1 =filter_in2[FILTER_SIZE_C*FILTER_SIZE_C] ;
	float store_data2 =filter_in3[FILTER_SIZE_C*FILTER_SIZE_C] ;
	float store_data3 =filter_in4[FILTER_SIZE_C*FILTER_SIZE_C] ;

	for(i=0; i<FILTER_SIZE_C; i++){
		for(j=0; j<FILTER_SIZE_C; j++){
			float pixel_val = win_buffer.getval(i,j);
			float filter_val0 = filter_in1[i*FILTER_SIZE_C+j];
			float filter_val1 = filter_in2[i*FILTER_SIZE_C+j];
			float filter_val2 = filter_in3[i*FILTER_SIZE_C+j];
			float filter_val3 = filter_in4[i*FILTER_SIZE_C+j];

			float res0 = pixel_val*filter_val0;
			store_data0 += res0;
			float res1 = pixel_val*filter_val1;
			store_data1 += res1;
			float res2 = pixel_val*filter_val2;
			store_data2 += res2;
			float res3 = pixel_val*filter_val3;
			store_data3 += res3;


		}
	}
	res[0] = store_data0;
	res[1] = store_data1;
	res[2] = store_data2;
	res[3] = store_data3;
}

void Pool1_20(t_window<float,Po1Layer_Num_I,FILTER_SIZE_P,FILTER_SIZE_P> win_buffer,float*res)
{
	uint16_t m, n, i, j,k;

	for(k=0;k<Po1Layer_Num_I;k++)
	{
		float store_data =-999999 ;
		for(i=0; i<FILTER_SIZE_P; i++)
		{
			for(j=0; j<FILTER_SIZE_P; j++)
			{
				float pixel_val = win_buffer.getval(k,i,j);
				if(store_data < pixel_val)
				{
					store_data = pixel_val;

				}

			}
		}
		res[k] = store_data;
	}

}

void Pool2_50(t_window<float,Po2Layer_Num_I,FILTER_SIZE_P,FILTER_SIZE_P> win_buffer,float*res)
{
	uint16_t m, n, i, j,k;

	for(k=0;k<Po2Layer_Num_I;k++)
	{
		float store_data =-999999;
		for(i=0; i<FILTER_SIZE_P; i++)
		{
			for(j=0; j<FILTER_SIZE_P; j++)
			{
				float pixel_val = win_buffer.getval(k,i,j);
				if(store_data < pixel_val)
				{
					store_data = pixel_val;

				}

			}
		}
		res[k] = store_data;
	}

}
void Conv2_5(t_window<float,Co2Layer_Num_I,FILTER_SIZE_C,FILTER_SIZE_C> win_buffer,float *filter_in1, float *filter_in2, float *filter_in3, float *filter_in4,
		float *filter_in5,float*res)
{
	uint16_t m, n, i, j,k;
	float store_data1 =filter_in1[Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C] ;
	float store_data2 =filter_in2[Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C] ;
	float store_data3 =filter_in3[Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C];
	float store_data4 =filter_in4[Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C];
	float store_data5 =filter_in5[Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C];
	for(k=0;k<Co2Layer_Num_I;k++)
	{

		for(i=0; i<FILTER_SIZE_C; i++)
		{
			for(j=0; j<FILTER_SIZE_C; j++)
			{
				float pixel_val = win_buffer.getval(k,i,j);
				uint16_t offset = (i*FILTER_SIZE_C+j)+k*25;
				float filter_val1 = filter_in1[offset];
				float filter_val2 = filter_in2[offset];
				float filter_val3 = filter_in3[offset];
				float filter_val4 = filter_in4[offset];
				float filter_val5 = filter_in5[offset];
#pragma HLS RESOURCE variable=store_data1 core=DSP48
				store_data1 += pixel_val*filter_val1;
#pragma HLS RESOURCE variable=store_data2 core=DSP48
				store_data2 += pixel_val*filter_val2;
#pragma HLS RESOURCE variable=store_data3 core=DSP48
				store_data3 += pixel_val*filter_val3;
#pragma HLS RESOURCE variable=store_data4 core=DSP48
				store_data4 += pixel_val*filter_val4;
#pragma HLS RESOURCE variable=store_data5 core=DSP48
				store_data5 += pixel_val*filter_val5;
			}
		}
	}
	res[0] = store_data1;
	res[1] = store_data2;
	res[2] = store_data3;
	res[3] = store_data4;
	res[4] = store_data5;
}
void convlayer1(d_linebuffer<float,FILTER_SIZE_C, Co1Layer_COLS_I> line_buffer_c1,
d_window<float,FILTER_SIZE_C,FILTER_SIZE_C> window_buffer_c1,float *pixel_in,float(*filter1_buffer)[Co1Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C+1],float (*pixel_c1_out)[Co1Layer_Num_O]){
	uint16_t d, r, c;
	uint8_t ii = 0;
	C1_ROW_LOOP: for(r=0; r<Co1Layer_ROWS_I; r++){
			C1_COL_LOOP: for(c=0; c<Co1Layer_COLS_I; c++){
			#pragma HLS PIPELINE II=4
					uint32_t in_offset;
					in_offset =r*Co1Layer_COLS_I+c;
					float load_pixel_data = pixel_in[in_offset];

					line_buffer_c1.shift_up(c);
					line_buffer_c1.insert_bottom(load_pixel_data, c);


					if(r>=4)
					{
						window_buffer_c1.shift_right();
						window_buffer_c1.insert(line_buffer_c1.getval(4,c), 0, 4);
						window_buffer_c1.insert(line_buffer_c1.getval(3,c), 1, 4);
						window_buffer_c1.insert(line_buffer_c1.getval(2,c), 2, 4);
						window_buffer_c1.insert(line_buffer_c1.getval(1,c), 3, 4);
						window_buffer_c1.insert(line_buffer_c1.getval(0,c), 4, 4);
					}

					if(r>=4 && c>=4)
					{
						float res[5][4];
						for(ii = 0; ii < Co1Layer_Num_O/4; ii++)
						{
							Conv1_4(window_buffer_c1, filter1_buffer[ii*4], filter1_buffer[4*ii+1], filter1_buffer[4*ii+2], filter1_buffer[4*ii+3],res[ii]);
						}
						for(ii=0;ii<20;ii++){
							pixel_c1_out[(r-4)*Co1Layer_COLS_O+(c-4)][ii]=res[ii/4][ii%4];
						}
					}
				}
			}
}
void poollayer1(t_linebuffer<float,Po1Layer_Num_I,FILTER_SIZE_P, Po1Layer_COLS_I> line_buffer_p1,
t_window<float,Po1Layer_Num_I,FILTER_SIZE_P,FILTER_SIZE_P> window_buffer_p1,float (*pixel_c1_out)[Co1Layer_Num_O],float (*pixel_p1_out)[Po1Layer_Num_O]){
	uint16_t d, r, c;
	uint8_t ii = 0;
	P1_ROW_LOOP: for(r=0; r<Po1Layer_ROWS_I; r++){
#pragma HLS PIPELINE II=4
			P1_COL_LOOP: for(c=0; c<Po1Layer_COLS_I; c++){
	#pragma HLS PIPELINE II=4
				P1_DEP_LOOP:for (d=0; d<Po1Layer_Num_I;d++){
		#pragma HLS PIPELINE II=4
						 	 float load_pixel_data = pixel_c1_out[r*Po1Layer_COLS_I+c][d];
						 	 if(r%2==0){
						 		 line_buffer_p1.insert_top(load_pixel_data,d, c);
						 	 }
						 	 else
						 	 {
						 		line_buffer_p1.insert_bottom(load_pixel_data,d, c);
						 	 }
						 	 if((r>=1)&&(r%2==1))
						 	 {
								 if(c%2==0){
									 window_buffer_p1.insert(line_buffer_p1.getval(d,0,c), d, 0, 0);
									 window_buffer_p1.insert(line_buffer_p1.getval(d,1,c), d, 1, 0);
								 }
								 else
								 {
									window_buffer_p1.insert(line_buffer_p1.getval(d,0,c), d, 0, 1);
									window_buffer_p1.insert(line_buffer_p1.getval(d,1,c), d, 1, 1);
								 }
						 	 }
						 }
						if(r>=1 && c>=1 &&(r%2==1) &&(c%2==1))
						{
							Pool1_20(window_buffer_p1, pixel_p1_out[(r/2)*Po1Layer_COLS_O+(c/2)]);

						}
					  }
				  }
}
void convlayer2(t_linebuffer<float,Co2Layer_Num_I,FILTER_SIZE_C, Co2Layer_COLS_I> line_buffer_c2,
		t_window<float,Co2Layer_Num_I,FILTER_SIZE_C,FILTER_SIZE_C> window_buffer_c2,float (*pixel_p1_out)[Po1Layer_Num_O],float(*filter2_buffer)[Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C+1],float (*pixel_c2_out)[Co2Layer_Num_O]){
	uint16_t d, r, c;
	uint8_t ii = 0;
	C2_ROW_LOOP: for(r=0; r<Co2Layer_ROWS_I; r++){
			C2_COL_LOOP: for(c=0; c<Co2Layer_COLS_I; c++){
		#pragma HLS PIPELINE II=4
				C2_DEP_LOOP:for (d=0; d<Co2Layer_Num_I;d++){
		#pragma HLS PIPELINE II=4

						 	 float load_pixel_data = pixel_p1_out[r*Co2Layer_COLS_I+c][d];;
						 	 line_buffer_c2.shift_up(d,c-1);
						 	 line_buffer_c2.insert_bottom(load_pixel_data,d, c);
						 	if(r>=4)
						 	{

						 		window_buffer_c2.shift_left(d);
						 		window_buffer_c2.insert(line_buffer_c2.getval(d,0,c), d, 0, 4);
						 		window_buffer_c2.insert(line_buffer_c2.getval(d,1,c), d, 1, 4);
						 		window_buffer_c2.insert(line_buffer_c2.getval(d,2,c), d, 2, 4);
						 		window_buffer_c2.insert(line_buffer_c2.getval(d,3,c), d, 3, 4);
						 		window_buffer_c2.insert(line_buffer_c2.getval(d,4,c), d, 4, 4);
						 	}

						 }
						if(r>=4 && c>=4)
						{
							float res[10][5];
#pragma HLS ARRAY_PARTITION variable=res complete dim=0
							uint8_t ii = 0;


							for(ii = 0; ii < Co2Layer_Num_O/5; ii++)
							{
								Conv2_5(window_buffer_c2,filter2_buffer[ii*5], filter2_buffer[5*ii+1], filter2_buffer[5*ii+2], filter2_buffer[5*ii+3],filter2_buffer[5*ii+4],res[ii]);
							}
							for(ii=0;ii<50;ii++){
								pixel_c2_out[(r-4)*Co2Layer_COLS_O+(c-4)][ii]=res[ii/5][ii%5];
							}
						}
					  }
				  }
}
void poollayer2(t_linebuffer<float,Po2Layer_Num_I,FILTER_SIZE_P, Po2Layer_COLS_I> line_buffer_p2,
		t_window<float,Po2Layer_Num_I,FILTER_SIZE_P,FILTER_SIZE_P> window_buffer_p2,float (*pixel_c2_out)[Co2Layer_Num_O],float (*pixel_p2_out)[Po2Layer_Num_O]){
	uint16_t d, r, c;
	uint8_t ii = 0;
	P2_ROW_LOOP: for(r=0; r<Po2Layer_ROWS_I; r++){
			P2_COL_LOOP: for(c=0; c<Po2Layer_COLS_I; c++){
		#pragma HLS PIPELINE II=4
				P2_DEP_LOOP:for (d=0; d<Po2Layer_Num_I;d++){
		#pragma HLS PIPELINE II=4

						 	 float load_pixel_data = pixel_c2_out[r*Po2Layer_COLS_I+c][d];
						 	 if(r%2==0){
						 		 line_buffer_p2.insert_top(load_pixel_data,d, c);
						 	 }
						 	 else
						 	 {
						 		line_buffer_p2.insert_bottom(load_pixel_data,d, c);
						 	 }
						 	 if((r>=1)&&(r%2==1))
						 	 {
								 if(c%2==0){
									 window_buffer_p2.insert(line_buffer_p2.getval(d,0,c), d, 0, 0);
									 window_buffer_p2.insert(line_buffer_p2.getval(d,1,c), d, 1, 0);
								 }
								 else
								 {
									window_buffer_p2.insert(line_buffer_p2.getval(d,0,c), d, 0, 1);
									window_buffer_p2.insert(line_buffer_p2.getval(d,1,c), d, 1, 1);
								 }
						 	 }
						 }
						if(r>=1 && c>=1 &&(r%2==1) &&(c%2==1))
						{
							Pool2_50(window_buffer_p2,pixel_p2_out[(r/2)*Po2Layer_COLS_O+(c/2)] );

						}

				  }
			  }
}
void outall(int outnum,int outrow,int outcol,float(*layerout)[50],float* pixel_out){
	uint16_t d,r,i = 0;
		for(d = 0; d<outnum;d++){
#pragma HLS PIPELINE II=4
			for(r=0;r<outrow*outcol;r++){
				#pragma HLS PIPELINE II=4
				pixel_out[i] = layerout[r][d];
				++i;
				}
			}
}
void PL_AXI(float *pixel_in, float *pixel_out, float *filter1_in, float *filter2_in,int32_t addr_reserved){
#pragma HLS INTERFACE m_axi depth =28*28*32 port = pixel_in offset = slave bundle =  user_axi_in  register
#pragma HLS INTERFACE s_axilite port = pixel_in  bundle = user_axi4lite_config  register

#pragma HLS INTERFACE m_axi depth =4*4*50*32 port = pixel_out offset = slave bundle =  user_axi_out  register
#pragma HLS INTERFACE s_axilite port = pixel_out  bundle = user_axi4lite_config  register

#pragma HLS INTERFACE m_axi depth=20*25*32+20*32 port = filter1_in offset = slave bundle = user_axi_filter1_in register
#pragma HLS INTERFACE s_axilite port = filter1_in bundle = user_axi4lite_config  register

#pragma HLS INTERFACE m_axi depth=20*50*25*32+50*32 port = filter2_in offset = slave bundle = user_axi_filter2_in register
#pragma HLS INTERFACE s_axilite port = filter2_in bundle = user_axi4lite_config  register

#pragma HLS INTERFACE s_axilite port = addr_reserved offset = 0xFFF0 bundle = user_axi4lite_config  register
#pragma HLS INTERFACE s_axilite port = return bundle = user_axi4lite_config  register
	float  filter1_buffer[Co1Layer_Num_O][Co1Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C+1];
	float  filter2_buffer[Co2Layer_Num_O][Co2Layer_Num_I*FILTER_SIZE_C*FILTER_SIZE_C+1];

	float  pixel_c1_out[Co1Layer_ROWS_O*Co1Layer_COLS_O][Co1Layer_Num_O];
	float  pixel_p1_out[Po1Layer_ROWS_O*Po1Layer_COLS_O][Po1Layer_Num_O];
	float  pixel_c2_out[Co2Layer_ROWS_O*Co2Layer_COLS_O][Co2Layer_Num_O];
	float  pixel_p2_out[Po2Layer_ROWS_O*Po2Layer_COLS_O][Po2Layer_Num_O];
	#pragma HLS ARRAY_PARTITION variable=filter1_buffer complete dim=0
	#pragma HLS ARRAY_PARTITION variable=filter2_buffer complete dim=0
	#pragma HLS ARRAY_PARTITION variable=pixel_c1_out complete dim=0
	#pragma HLS ARRAY_PARTITION variable=pixel_p1_out complete dim=0
	#pragma HLS ARRAY_PARTITION variable=pixel_c2_out complete dim=0
	#pragma HLS ARRAY_PARTITION variable=pixel_p2_out complete dim=0
	memcpy(filter1_buffer, filter1_in, (Co1Layer_Num_I*Co1Layer_Num_O*FILTER_SIZE_C*FILTER_SIZE_C+Co1Layer_Num_O)*sizeof(float));
	memcpy(filter2_buffer, filter2_in, (Co2Layer_Num_I*Co2Layer_Num_O*FILTER_SIZE_C*FILTER_SIZE_C+Co2Layer_Num_O)*sizeof(float));
	d_linebuffer<float,FILTER_SIZE_C, Co1Layer_COLS_I> line_buffer_c1;
	d_window<float,FILTER_SIZE_C,FILTER_SIZE_C> window_buffer_c1;

	t_linebuffer<float,Po1Layer_Num_I,FILTER_SIZE_P, Po1Layer_COLS_I> line_buffer_p1;
	t_window<float,Po1Layer_Num_I,FILTER_SIZE_P,FILTER_SIZE_P> window_buffer_p1;

	t_linebuffer<float,Co2Layer_Num_I,FILTER_SIZE_C, Co2Layer_COLS_I> line_buffer_c2;
	t_window<float,Co2Layer_Num_I,FILTER_SIZE_C,FILTER_SIZE_C> window_buffer_c2;

	t_linebuffer<float,Po2Layer_Num_I,FILTER_SIZE_P, Po2Layer_COLS_I> line_buffer_p2;
	t_window<float,Po2Layer_Num_I,FILTER_SIZE_P,FILTER_SIZE_P> window_buffer_p2;

	uint8_t ii = 0;

	convlayer1(line_buffer_c1,window_buffer_c1,pixel_in,filter1_buffer,pixel_c1_out);

//	uint16_t i = 0;
//	for(d = 0; d<Co1Layer_Num_O;d++){
//		for(r=0;r<Co1Layer_ROWS_O*Co1Layer_COLS_O;r++){
//				#pragma HLS PIPELINE II=4
//				pixel_out[i++] = pixel_c1_out[r][d];
//				}
//			}

	poollayer1(line_buffer_p1,window_buffer_p1,pixel_c1_out,pixel_p1_out);
//	uint16_t i = 0;
//	for(d = 0; d<Po1Layer_Num_O;d++){
//		for(r=0;r<Po1Layer_ROWS_O*Po1Layer_COLS_O;r++){
//				#pragma HLS PIPELINE II=4
//				pixel_out[i++] = pixel_p1_out[r][d];
//				}
//			}
	convlayer2(line_buffer_c2,window_buffer_c2,pixel_p1_out,filter2_buffer,pixel_c2_out);
//	uint16_t i = 0;
//	for(d = 0; d<Co2Layer_Num_O;d++){
//		for(r=0;r<Co2Layer_ROWS_O*Co2Layer_COLS_O;r++){
//				#pragma HLS PIPELINE II=4
//				pixel_out[i++] = pixel_c2_out[r][d];
//				}
//			}
	poollayer2(line_buffer_p2,window_buffer_p2,pixel_c2_out,pixel_p2_out);
	outall(Po2Layer_Num_O,Po2Layer_ROWS_O,Po2Layer_COLS_O,pixel_p2_out,pixel_out);
//	uint16_t i = 0;
//	for(d = 0; d<Po2Layer_Num_O;d++){
//		for(r=0;r<Po2Layer_Num_O*Po2Layer_COLS_O;r++){
//			#pragma HLS PIPELINE II=4
//			pixel_out[i] = pixel_p2_out[r][d];
//			++i;
//			}
//		}
	}


