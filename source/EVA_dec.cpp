#include "malloc.h"
#include "stdio.h"
#include"math.h"
#include"EVA_dec.h"
static float bit_distance = 3; //5k时1bit宽度
//static float bit_distance2 = 7; //2.5k时1bit宽度
int code1[16];
int code2[16];
int code3[16];
int flag1;
int flag2;
int flag3;
static unsigned char LEN = 7*2;   //信息码的长度，7*2，2为manchester编码
//四舍五入取整函数
double round(double r)
{
	return(r>0.0)?floor(r+0.5):ceil(r-0.5);
}
unsigned char Manchester_Check(unsigned char *code,int len)
{
	for(int i=0;i<len/2;i++)
	{
		if(code[2*i]==code[2*i+1])
		{
			return 0;
		}
	}
	return 1;
}
//int* findpeaks(int* average_col, int w);
/*输入：需要解码的区域灰度图，图片行数，图片列数，保存码的数组(2维，多列码);输出：解出来的码个数*/
int EVA_dec(int** gray,int h,int w,unsigned char code[CODE_NUM_ONEPIC_MAX][CODE_LEN])
{
	int* average_col = (int*)malloc(sizeof(int) * (w+5));  //预留一部分空间
	//循环可能用到的变量
	int i;
	int j;
	int k;
	int index;
	//列均值
	int average;
	int peak[1000][2];//一列指示峰值在图像中的位置locs，一列指示峰值处的值pks
	int noise_PP = 255 * 0.05;//小于这个值的认为是假波峰/波谷
	int num = 0;
	int realPeak[1000][2];
	int peak_num = 1;
	int code_head[1000];
	int code_loc[1000];    //指示头部位置
	int L1 = 0, L2 = 0;   //确定连续比特的宽度
	int bit_length;
	int head_code[4] = { 1,1,1,1 };
	unsigned char cov = 0;
	unsigned char flag_head = 0; //是否找到头部序列
	unsigned char code_info[14];
	unsigned char flag_info = 0;
	unsigned char* code_original;
	int aa[1000];
	int head_num = 0;  //有几个头
	//22*3=66, 一般长度不超过250，最多5个码,但是考虑干扰，可能出现多个连1，就会判断出多个头
	int Head[100];  
	unsigned char is_Manc = 0;
	int addr;
	//unsigned char flag = 0; //解码是否成功标志
	for (i = 0; i < w-3; i++)
	{
		average = 0;
		for(j=0;j<h-3;j++)
		{ 
			average += gray[j][i];
		}
		average_col[i] = average/h;
	}
	/*int aa[1000];
	for (i = 0; i < w; i++)
	{
	aa[i] = average_col[i];
	}*/


	/************EVA方法判断原图每列的逻辑0/1****************/

	for (i = 1; i < w; i++)
	{
		//极值点
		if (((average_col[i] >= average_col[i - 1]) && (average_col[i] >= average_col[i + 1]))\
			|| ((average_col[i] <= average_col[i - 1]) && (average_col[i] <= average_col[i + 1])))
		{
			peak[num][0] = i;
			peak[num][1] = average_col[i];
			num++;
		}
	}

	//极值一般出现在几十列之后，这之前的图像也需要用到(原图每列要能对应原码)，所以第一个极值取在最左边，大小根据average_col[0]确定
	realPeak[0][0] = 0;
	if (average_col[0] > 255 / 2)	realPeak[0][1] = 1;
	else realPeak[0][1] = 0;

	for (i = 0; i < num - 1; i++)
	{
		if (abs(peak[i][1] - peak[i + 1][1]) > noise_PP)
		{
			realPeak[peak_num][0] = peak[i][0];
			realPeak[peak_num][1] = peak[i][1];
			peak_num++;
		}
	}

	//记录下每个波峰/谷的位置后，根据EVA划定每一列的逻辑
	num = 0;
	code_original = (unsigned char*)malloc(sizeof(unsigned char) * (w + 5));   //原码，和average_col一一对应
	for (i = 0; i < peak_num+1; i++)
	{
		for (j = realPeak[i][0]; j < realPeak[i + 1][0]; j++)
		{

			if (average_col[j] > (realPeak[i][1] + realPeak[i + 1][1]) / 2)
			{
				code_original[num] = 1;
			}
			else
			{
				code_original[num] = 0;
			}
			num++;
		}
	}
	//上面只处理到最后一个波峰，到图像最右边还有一部分像素没有赋值，这部分不考虑进译码，统一赋值-1
	for (i = realPeak[peak_num-1][0]; i < w; i++)
	{
		code_original[num] = -1;
		num++;
	}


	for (i = 0; i < w; i++)
	{
		aa[i] = code_original[i];
	}
	/*********************************解码***********************************/
	/***************************定位头部序列*******************************/

	i = 0; j = 0; k = 0;
	while (i < w-1)
	{
		L1 = i;
		while (i < w - 1)
		{
			if (abs(code_original[i+1]-code_original[i]) == 0)  //找到变化沿
				i++;
			else
				break;
		}
		i=i+1;
		L2 = i;
		bit_length = round((L2 - L1) / bit_distance);
		if (bit_length > 0)
		{
			for (j = 0; j < bit_length; j++)
			{
				//采样处的0/1
				code_head[k] = code_original[L1];
				//每个采样处对应的列坐标,和code_head一一对应
				code_loc[k] = L1 + ceil(bit_distance * (j-1));
				k++;   //k记录降采样后，还有几个采样点
			}
		}
	}
	//k用来记录降采样后的点数，不要改动，后面都用i、j作为循环数
	/*以上降采样得到序列后，开始找包头*/
	for (i = 0; i < k; i++)
	{
		cov = 0;
		for (j = i; j < i+4; j++)
		{
			cov += (code_head[j] == head_code[j - i]);
		}
		if (cov == 4)
		{
			
			flag_head = 1;
			head_num++;
			//通常一盏灯含的码数不会超过5，若超过了就置6，防止后续Head越界
			//if(head_num>6) head_num = 6; 
			//头部点对应的降采样序列索引
			Head[head_num-1]=i;
			//之前找到第一个头的地方就停手，现在尽量把一张图片所有信息都用上
			//break;
		}
	}

	//没找到头部，就返回全0码(默认灯id从1开始记,不会出现id为0的灯)
	if (flag_head == 0)
	{
		//printf("Error:Cannot find the head.");
		for (i = 0; i < 7; i++)
		{
			code[0][i] = 0;
		}
		free(code_original);
		return 0;
	}
	//如果找到头部，紧接之前循环到的i，确定信息部分在原码中的位置
	//如果头在图片最后，解不出14位码，则直接返回0
	/*if (code_loc[Head[0]]+4*bit_distance+4> w - 1)
	{
	j = code_loc[i];
	}
	else
	{
	j = code_loc[i] + 8 * bit_distance + 4 - 2;
	}*/
	if (code_loc[Head[0]]+4*bit_distance+4> w - 1)
	{
		for (i = 0; i < 7; i++)
		{
			code[0][i] = 0;
		}
		free(code_original);
		return 0;
	}


	/*处理信息部分*/
	//用index保存原来k记录的索引号，k没什么用了，可以赋值
	index = k;
	k = head_num;
	addr = 0; flag_info = 0;
	for(head_num=0;head_num<k;head_num++)
	{
		i = Head[head_num]+4; //定位到信息码第一位的索引
		if(i>index-1-LEN) break;
		for(j=0;j<LEN;j++)
		{
			code_info[j] = code_head[i+j];
		}
		is_Manc = Manchester_Check(code_info,LEN);
		if(is_Manc)
		{
			for(j=0;j<LEN/2;j++)
			{
				if(code_info[2*j]==0) code[addr][j] = 0;
				else code[addr][j] = 1;
			}
			addr++;
			flag_info = 1;
		}
	}
	//如果没有一列码通过Manchester_Check,则返回7个0的码
	if(flag_info==0)
	{
		for (i = 0; i < 7; i++)
		{
			code[0][i] = 0;
		}
		free(code_original);
		return 0;
	}
	//解码成功，返回解出的码数量
	free(code_original);
	return addr;
}

//k = 0;
//while (j < w - 1)
//{
//	j = j + 2;
//	L1 = j;
//	while (j < w - 1)
//	{
//		if (abs(average_col[j + 1] - average_col[j]) < noise_PP)  //找到变化沿
//			j++;
//		else
//			break;
//	}
//	L2 = j;
//	bit_length = round((L2 - L1 + 1) / bit_distance2);
//	if (bit_length > 0)
//	{
//		for (i = 0; i < bit_length; i++)
//		{
//			code_info[k] = code_original[L1];
//			k++;
//		}
//		if (k >= 16) { flag_info = 1; break; }
//	}
//}
//	if(k<16)
//	{
//		printf("Error:The length of information is less than 16bits.");
//		free(code_original);
//		return 0;
//	}
//	//解码成功
//	for (i = 0; i < 16; i++)
//	{
//		code[i] = code_info[i];
//	}
//	free(code_original);
//	return 1;
//}

//int* findpeaks(int* average_col,int w)
//{
//	int i;
//	int num=0;
//	int peak[1000][2];//一列指示峰值在图像中的位置pks，一列指示峰值处的值locs
//	int noise_PP=255*0.05;//小于这个值的认为是假波峰/波谷
//	int PP_distance = 3;  //5k
//	for(i=0;i<w;i++)
//	{
//		//极值点
//		if (((average_col[i] >= average_col[i - 1]) && (average_col[i] >= average_col[i + 1]))\
//			|| ((average_col[i] <= average_col[i - 1]) && (average_col[i] <= average_col[i + 1])))
//		{
//			num++;
//			peak[num][0] = i;
//			peak[num][1] = average_col[i];
//		}
//	}
//	int realPeak[1000][2];
//	int n = 0;
//	for (i = 1; i < num-1; i++)
//	{
//		if ((peak[i][0] - peak[i - 1][0] >= bit_distance1) && (abs(peak[i][1] - peak[i + 1][1]) > noise_PP))
//		{
//			realPeak[i][0] = peak[i][0];
//			realPeak[i][1] = peak[i][1];
//			n++;
//		}
//	}
//	realPeak[0][0] = n;//realPeak[0][0]记录波峰的个数
//	return realPeak;
//}