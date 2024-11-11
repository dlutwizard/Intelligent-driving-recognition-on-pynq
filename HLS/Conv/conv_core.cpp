#include "conv_core.h"

//Feature: [H][W][C]
//kernel: [Ky][Kx][CHin][CHout]


//CHin:输入通道 Hin:输入高度 Win:输入宽度 CHout:输出通道 Kx、Ky:卷积核大小? Sx、Sy：移动步长 mode:选择padding模式 relu_en:激活使能 feature_in[]:图像输入 W[]:权重 
//bias[]:偏置 feature_out[]:输出
void Conv(ap_uint<16> CHin,ap_uint<16> Hin,ap_uint<16> Win,ap_uint<16> CHout,
		ap_uint<8> Kx,ap_uint<8> Ky,ap_uint<8> Sx,ap_uint<8> Sy,ap_uint<1> mode,ap_uint<1> relu_en,
		Dtype_f feature_in[],Dtype_w W[],Dtype_w bias[],Dtype_f feature_out[]
	)//mode: 0:VALID, 1:SAME 
{
#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_out offset=slave
#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=bias offset=slave
#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=W offset=slave
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=m_axi depth=2147483647 port=feature_in offset=slave
#pragma HLS INTERFACE mode=s_axilite port=relu_en
#pragma HLS INTERFACE mode=s_axilite port=mode
#pragma HLS INTERFACE mode=s_axilite port=Sy
#pragma HLS INTERFACE mode=s_axilite port=Sx
#pragma HLS INTERFACE mode=s_axilite port=Ky
#pragma HLS INTERFACE mode=s_axilite port=Kx
#pragma HLS INTERFACE mode=s_axilite port=CHout
#pragma HLS INTERFACE mode=s_axilite port=Win
#pragma HLS INTERFACE mode=s_axilite port=Hin
#pragma HLS INTERFACE mode=s_axilite port=CHin
	ap_uint<8> pad_x,pad_y;
	if(mode==0)
	{
		pad_x=0;pad_y=0;
	}
	else
	{
		pad_x=(Kx-1)/2;
		pad_y=(Ky-1)/2;
	}
	ap_uint<16> Hout,Wout;
	Wout=(Win+2*pad_x-Kx)/Sx+1;
	Hout=(Hin+2*pad_y-Ky)/Sy+1;

	for(int cout=0;cout<CHout;cout++) //输出通道
		for(int i=0;i<Hout;i++) //输出高度
			for(int j=0;j<Wout;j++) //输出宽度
			{
				//下面是完成单个窗口的卷积
				Dtype_acc sum=0; 
				for(int ii=0;ii<Ky;ii++) //卷积核大�?
					for(int jj=0;jj<Kx;jj++) //卷积核大�?
					{
						//h和w是为了确定图片中哪些像素和卷积核卷积
						ap_int<16> h=i*Sy-pad_y+ii;
						ap_int<16> w=j*Sx-pad_x+jj;
						if(h>=0 && w>=0 && h<Hin && w<Win)
						{
							for(int cin=0;cin<CHin;cin++) //输入通道
							{
								
								Dtype_mul tp=feature_in[h*CHin*Win+w*CHin+cin]*W[ii*Kx*CHin*CHout+jj*CHin*CHout+cin*CHout+cout];
								sum+=tp;
							}
						}
					}

				sum+=bias[cout];
				if(relu_en & sum<0)
					sum=0;
				
				feature_out[i*Wout*CHout+j*CHout+cout]=sum;
			}
}
