#include "malloc.h"
#include "stdio.h"
#include"math.h"
#include"EVA_dec.h"
static float bit_distance = 3; //5kʱ1bit���
//static float bit_distance2 = 7; //2.5kʱ1bit���
int code1[16];
int code2[16];
int code3[16];
int flag1;
int flag2;
int flag3;
static unsigned char LEN = 7*2;   //��Ϣ��ĳ��ȣ�7*2��2Ϊmanchester����
//��������ȡ������
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
/*���룺��Ҫ���������Ҷ�ͼ��ͼƬ������ͼƬ�����������������(2ά��������);�����������������*/
int EVA_dec(int** gray,int h,int w,unsigned char code[CODE_NUM_ONEPIC_MAX][CODE_LEN])
{
	int* average_col = (int*)malloc(sizeof(int) * (w+5));  //Ԥ��һ���ֿռ�
	//ѭ�������õ��ı���
	int i;
	int j;
	int k;
	int index;
	//�о�ֵ
	int average;
	int peak[1000][2];//һ��ָʾ��ֵ��ͼ���е�λ��locs��һ��ָʾ��ֵ����ֵpks
	int noise_PP = 255 * 0.05;//С�����ֵ����Ϊ�Ǽٲ���/����
	int num = 0;
	int realPeak[1000][2];
	int peak_num = 1;
	int code_head[1000];
	int code_loc[1000];    //ָʾͷ��λ��
	int L1 = 0, L2 = 0;   //ȷ���������صĿ��
	int bit_length;
	int head_code[4] = { 1,1,1,1 };
	unsigned char cov = 0;
	unsigned char flag_head = 0; //�Ƿ��ҵ�ͷ������
	unsigned char code_info[14];
	unsigned char flag_info = 0;
	unsigned char* code_original;
	int aa[1000];
	int head_num = 0;  //�м���ͷ
	//22*3=66, һ�㳤�Ȳ�����250�����5����,���ǿ��Ǹ��ţ����ܳ��ֶ����1���ͻ��жϳ����ͷ
	int Head[100];  
	unsigned char is_Manc = 0;
	int addr;
	//unsigned char flag = 0; //�����Ƿ�ɹ���־
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


	/************EVA�����ж�ԭͼÿ�е��߼�0/1****************/

	for (i = 1; i < w; i++)
	{
		//��ֵ��
		if (((average_col[i] >= average_col[i - 1]) && (average_col[i] >= average_col[i + 1]))\
			|| ((average_col[i] <= average_col[i - 1]) && (average_col[i] <= average_col[i + 1])))
		{
			peak[num][0] = i;
			peak[num][1] = average_col[i];
			num++;
		}
	}

	//��ֵһ������ڼ�ʮ��֮����֮ǰ��ͼ��Ҳ��Ҫ�õ�(ԭͼÿ��Ҫ�ܶ�Ӧԭ��)�����Ե�һ����ֵȡ������ߣ���С����average_col[0]ȷ��
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

	//��¼��ÿ������/�ȵ�λ�ú󣬸���EVA����ÿһ�е��߼�
	num = 0;
	code_original = (unsigned char*)malloc(sizeof(unsigned char) * (w + 5));   //ԭ�룬��average_colһһ��Ӧ
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
	//����ֻ�������һ�����壬��ͼ�����ұ߻���һ��������û�и�ֵ���ⲿ�ֲ����ǽ����룬ͳһ��ֵ-1
	for (i = realPeak[peak_num-1][0]; i < w; i++)
	{
		code_original[num] = -1;
		num++;
	}


	for (i = 0; i < w; i++)
	{
		aa[i] = code_original[i];
	}
	/*********************************����***********************************/
	/***************************��λͷ������*******************************/

	i = 0; j = 0; k = 0;
	while (i < w-1)
	{
		L1 = i;
		while (i < w - 1)
		{
			if (abs(code_original[i+1]-code_original[i]) == 0)  //�ҵ��仯��
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
				//��������0/1
				code_head[k] = code_original[L1];
				//ÿ����������Ӧ��������,��code_headһһ��Ӧ
				code_loc[k] = L1 + ceil(bit_distance * (j-1));
				k++;   //k��¼�������󣬻��м���������
			}
		}
	}
	//k������¼��������ĵ�������Ҫ�Ķ������涼��i��j��Ϊѭ����
	/*���Ͻ������õ����к󣬿�ʼ�Ұ�ͷ*/
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
			//ͨ��һյ�ƺ����������ᳬ��5���������˾���6����ֹ����HeadԽ��
			//if(head_num>6) head_num = 6; 
			//ͷ�����Ӧ�Ľ�������������
			Head[head_num-1]=i;
			//֮ǰ�ҵ���һ��ͷ�ĵط���ͣ�֣����ھ�����һ��ͼƬ������Ϣ������
			//break;
		}
	}

	//û�ҵ�ͷ�����ͷ���ȫ0��(Ĭ�ϵ�id��1��ʼ��,�������idΪ0�ĵ�)
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
	//����ҵ�ͷ��������֮ǰѭ������i��ȷ����Ϣ������ԭ���е�λ��
	//���ͷ��ͼƬ��󣬽ⲻ��14λ�룬��ֱ�ӷ���0
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


	/*������Ϣ����*/
	//��index����ԭ��k��¼�������ţ�kûʲô���ˣ����Ը�ֵ
	index = k;
	k = head_num;
	addr = 0; flag_info = 0;
	for(head_num=0;head_num<k;head_num++)
	{
		i = Head[head_num]+4; //��λ����Ϣ���һλ������
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
	//���û��һ����ͨ��Manchester_Check,�򷵻�7��0����
	if(flag_info==0)
	{
		for (i = 0; i < 7; i++)
		{
			code[0][i] = 0;
		}
		free(code_original);
		return 0;
	}
	//����ɹ������ؽ����������
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
//		if (abs(average_col[j + 1] - average_col[j]) < noise_PP)  //�ҵ��仯��
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
//	//����ɹ�
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
//	int peak[1000][2];//һ��ָʾ��ֵ��ͼ���е�λ��pks��һ��ָʾ��ֵ����ֵlocs
//	int noise_PP=255*0.05;//С�����ֵ����Ϊ�Ǽٲ���/����
//	int PP_distance = 3;  //5k
//	for(i=0;i<w;i++)
//	{
//		//��ֵ��
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
//	realPeak[0][0] = n;//realPeak[0][0]��¼����ĸ���
//	return realPeak;
//}