#include<stdio.h>
#include <iostream>
//#include <opencv2\core\core.hpp>
//#include <opencv2\imgproc\imgproc.hpp>
//#include <opencv2\highgui\highgui.hpp>
#include "Source_loc.h"
#include"EVA_dec.h"
#include "windows.h"
#include "MvCameraControl.h"
#include"Best_Code.h"
#include"PNPSolver.h"
#include"abc.h"
#include "WLAN_socket.h"
//using namespace cv;
Mat src,gray_image,code1_image,code2_image,code3_image,code4_image,code5_image; 

//static const char folderName[50] = { 'D', ':', '\\', 'c', 'a', 'm', 'e', 'r',
//    'a', '_', 'p', 'r', 'o', 'j', 'e', 'c', 't', '\\', 'T', 'h', 'r', 'e', 'e',
//    'L', 'i', 'g', 'h', 't', '\\', 'p', 'i','c','\\','p','i','c','_'};

extern int BWlabelpic;
void bmpHeaderPartLength(FILE* fpbmp);
/* To get the width and height of the bmp file */
void bmpWidthHeight(FILE* fpbmp);
//get r,g,b data
//void bmpDataPart(FILE* fpbmp);
/* Test the file is bmp file or not */
unsigned int OffSet = 0;    // bmp�ļ�ͷ��������Ϣ֮��ĳ���
long width=1536;          // ͼƬ��
long height=1024;         // ͼƬ��
//long n;       //ע������ȫ�ֱ���nʱĬ�ϸ�ֵ0������main������ʱ�Ǹ���

int gray[17][1024][1536];
//unsigned char pixel[17][1024 * 1536];  //��ȡbmp�ļ�ʱҪ��char���飬����ʱ��int���鷽��һЩ
int gray_average[1024][1536];  //17��ͼƬ��ƽ��
unsigned char p[1024 * 1536];  //�ݴ�bmp�ļ�

//extern int code1[16],code2[16],code3[16];
//extern int flag1,flag2,flag3;

unsigned char buffer1[CODE_NUM_ONEPIC_MAX][CODE_LEN];
unsigned char buffer2[CODE_NUM_ONEPIC_MAX][CODE_LEN];
unsigned char buffer3[CODE_NUM_ONEPIC_MAX][CODE_LEN];
unsigned char buffer4[CODE_NUM_ONEPIC_MAX][CODE_LEN];
unsigned char buffer5[CODE_NUM_ONEPIC_MAX][CODE_LEN];

unsigned char decode1[CODE_NUM_MAX][CODE_LEN];
unsigned char decode2[CODE_NUM_MAX][CODE_LEN];
unsigned char decode3[CODE_NUM_MAX][CODE_LEN];
unsigned char decode4[CODE_NUM_MAX][CODE_LEN];
unsigned char decode5[CODE_NUM_MAX][CODE_LEN];


extern pic_divide threelight;

unsigned char Light1_Code[CODE_LEN];
unsigned char Light2_Code[CODE_LEN];
unsigned char Light3_Code[CODE_LEN];
unsigned char Light4_Code[CODE_LEN];
unsigned char Light5_Code[CODE_LEN];

unsigned char id[5];
int u1,u2,u3,u4,u5;
int v1,v2,v3,v4,v5;
//ID
unsigned char id1,id2,id3,id4,id5;

int u[5],v[5];

int X,Y,Z;

int buf[3];

void main(void)
{
	unsigned int num = 0;
	long obj_length = width * height;
	char filename[100];//�ļ���
	int i,j,k;
	//int W[5], H[5];   //����ͼƬ�Ŀ�Ⱥ͸߶�
	//int** source1;
	//int** source2;
	//int** source3;
	//Mat test,gray_image;
	Mat source1,source2,source3,source4,source5;
	int addr1,addr2,addr3,addr4,addr5;
	int code_num1=0,code_num2=0,code_num3=0,code_num4=0,code_num5=0;
	//��ȡͼƬ�ߴ�
	//sprintf_s(filename, 100, "D:\\����ͷ��λ��Ŀ\\����\\VS2010\\ThreeLamps\\test\\pic_1.bmp");
	//src = imread(filename);
	//width = src.cols;
	//height = src.rows;
	//test = Mat::zeros(height,width, CV_8UC1);
	//
	//VideoCapture cap("C:\\Users\\67081\\Documents\\WeChat Files\\wxid_znut3kdb81hz22\\FileStorage\\File\\2021-08\\ThreeLamps(������Ƶ��)\\ThreeLamps(������Ƶ��)\\test\\Video_1.avi");
	//VideoCapture cap("D:\\����ͷ��λ��Ŀ\\����\\VS2010\\ThreeLamps(������Ƶ��)\\test\\Video_1.avi");
	VideoCapture cap("C:\\Users\\67081\\Desktop\\�½��ļ���\\Video_2.avi");
	if(!cap.isOpened())
	{
		printf("error:cannot open video");
	}
	 //int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);  //֡���
  //   int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //֡�߶�
     int frameRate = cap.get(CV_CAP_PROP_FPS);  //֡�� x frames/s
     int totalFrames = cap.get(CV_CAP_PROP_FRAME_COUNT); //��֡��
	 frameRate = frameRate<17? frameRate:17;
	 Mat frame;
	 int now_num=0;
	 //for(i = 0;i<170;i++)
	 //{
		// cap.read(frame);
		// imshow("video", frame);
		// waitKey(0);
	 //}


	//ÿ17��ͼƬ����һ��
	for (num = 1; num <= totalFrames; num++)
	{
		//sprintf_s(filename, 100, "D:\\����ͷ��λ��Ŀ\\����\\VS2010\\ThreeLamps\\test\\pic_%d.bmp", num);
		//src = imread(filename);
		cap.read(src);

		cvtColor( src, gray_image, CV_BGR2GRAY );
		Source_loc(gray_image);
		//��һάͼ������תΪ��ά���������
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				gray[now_num][i][j] = gray_image.data[i * width + j];
			}
		}
		//threelight.light_num���Ե���
		if(threelight.light_num>=1)
				{
					//����һ����Ƭ���ĵ���յ�ƣ�ֻȡ����3յ

					//��Ȼ��Mat����ʹ��cv�Ķ��ֺ�������C������CV��������1ά�ģ������㴦�����Ի����ö�ά����װ������
					//��������Դ��ͼƬ�л��ֳ���,��Ϊ����ͼ,ͬʱ����ͼ��ʱ����ת90��
					int W[5], H[5];   //����ͼƬ�Ŀ�Ⱥ͸߶�
					for (i = 0; i < 4; i++)
					{
						W[i] = abs(threelight.light_range[i][3] - threelight.light_range[i][2]);
						H[i] = abs(threelight.light_range[i][1] - threelight.light_range[i][0]);
					}

					//��̬�����ڴ�ʱ�з���ʧ�ܵķ���,Ԥ��һ����Դ
					int** source1 = (int**)malloc(sizeof(int*) * (W[0] + 5));
					for (i = 0; i < W[0]; i++)
					{
						source1[i] = (int*)malloc(sizeof(int) * (H[0] + 5));
					}
					int** source2 = (int**)malloc(sizeof(int*) * (W[1] + 5));
					for (i = 0; i < W[1]; i++)
					{
						source2[i] = (int*)malloc(sizeof(int) * (H[1] + 5));
					}
					int** source3 = (int**)malloc(sizeof(int*) * (W[2] + 5));
					for (i = 0; i < W[2]; i++)
					{
						source3[i] = (int*)malloc(sizeof(int) * (H[2] + 5));
					}
					int** source4 = (int**)malloc(sizeof(int*) * (W[3] + 5));
					for (i = 0; i < W[3]; i++)
					{
						source4[i] = (int*)malloc(sizeof(int) * (H[3] + 5));
					}

					if(threelight.light_num>=5)
					{
						W[4] = abs(threelight.light_range[4][3] - threelight.light_range[4][2]);
						H[4] = abs(threelight.light_range[4][1] - threelight.light_range[4][0]);
						int** source5 = (int**)malloc(sizeof(int*) * (W[4] + 5));
						for (i = 0; i < W[4]; i++)
						{
							source5[i] = (int*)malloc(sizeof(int) * (H[4] + 5));
						}
						code5_image = Mat::zeros(W[4],H[4], CV_8UC1);
						for (i = 0; i < W[4]; i++)
						{
							for (j = 0; j < H[4]; j++)
							{
								source5[i][j] = gray[now_num][threelight.light_range[4][0] + j][threelight.light_range[4][3] - 1 - i];
								code5_image.data[H[4]*i+j] = source5[i][j];
							}
						}
						addr5 = EVA_dec(source5, W[4], H[4], buffer5);
						for(i=0;i<addr5;i++)
						{
							for(j=0;j<CODE_LEN;j++) 
								decode5[code_num5][j] = buffer5[i][j];
							code_num5++;
						}
						free(source5);
					}


					//���Էָ�Ч������Ϊ��ת��90��������ᣬ���Կ�͸߶Ե�
					code1_image = Mat::zeros(W[0],H[0], CV_8UC1);
					code2_image = Mat::zeros(W[1],H[1], CV_8UC1);
					code3_image = Mat::zeros(W[2],H[2], CV_8UC1);
					code4_image = Mat::zeros(W[3],H[3], CV_8UC1);
					

					for (i = 0; i < W[0]; i++)
					{
						for (j = 0; j < H[0]; j++)
						{
							source1[i][j] = gray[now_num][threelight.light_range[0][0] + j][threelight.light_range[0][3] - 1 - i];
							code1_image.data[H[0]*i+j] = source1[i][j];
						}
					}

					for (i = 0; i < W[1]; i++)
					{
						for (j = 0; j < H[1]; j++)
						{
							source2[i][j] = gray[now_num][threelight.light_range[1][0] + j][threelight.light_range[1][3] - 1 - i];
							code2_image.data[H[1]*i+j] = source2[i][j];
						}
					}
					for (i = 0; i < W[2]; i++)
					{
						for (j = 0; j < H[2]; j++)
						{
							source3[i][j] = gray[now_num][threelight.light_range[2][0] + j][threelight.light_range[2][3] - 1 - i];
							code3_image.data[H[2]*i+j] = source3[i][j];
						}
					}
					for (i = 0; i < W[3]; i++)
					{
						for (j = 0; j < H[3]; j++)
						{
							source4[i][j] = gray[now_num][threelight.light_range[3][0] + j][threelight.light_range[3][3] - 1 - i];
							code4_image.data[H[3]*i+j] = source4[i][j];
						}
					}
					//imshow("code1_image",code1_image);
					//waitKey(0);
					//imshow("code1_image",code2_image);
					//waitKey(0);
					//imshow("code1_image",code3_image);
					//waitKey(0);


					addr1 = EVA_dec(source1, W[0], H[0], buffer1);
					addr2 = EVA_dec(source2, W[1], H[1], buffer2);
					addr3 = EVA_dec(source3, W[2], H[2], buffer3);
					addr4 = EVA_dec(source4, W[3], H[3], buffer4);
					for(i=0;i<addr1;i++)
					{
						for(j=0;j<CODE_LEN;j++) 
							decode1[code_num1][j] = buffer1[i][j];
						code_num1++;
					}
					for(i=0;i<addr2;i++)
					{
						for(j=0;j<CODE_LEN;j++) 
							decode2[code_num2][j] = buffer2[i][j];
						code_num2++;
					}
					for(i=0;i<addr3;i++)
					{
						for(j=0;j<CODE_LEN;j++) 
							decode3[code_num3][j] = buffer3[i][j];
						code_num3++;
					}
					for(i=0;i<addr4;i++)
					{
						for(j=0;j<CODE_LEN;j++) 
							decode4[code_num4][j] = buffer4[i][j];
						code_num4++;
					}

					free(source1);
					free(source2);
					free(source3);
					free(source4);
				

		now_num++;
		//if(now_num>=frameRate) 
		if(now_num>=1) 
		{

			now_num = 0;

			Best_Code(Light1_Code,decode1,code_num1);
			Best_Code(Light2_Code,decode2,code_num2);
			Best_Code(Light3_Code,decode3,code_num3);
			Best_Code(Light4_Code,decode4,code_num4);
			
			for(j=0;j<5;j++) id[j]=0;
			for(j = 0;j<7;j++)
			{
				if(Light1_Code[j]) id[0]+=Light1_Code[j]<<(7-1-j);	
			}
			for(j = 0;j<7;j++)
			{
				if(Light2_Code[j]) id[1]+=Light2_Code[j]<<(7-1-j);	
			}
			for(j = 0;j<7;j++)
			{
				if(Light3_Code[j]) id[2]+=Light3_Code[j]<<(7-1-j);	
			}
			for(j = 0;j<7;j++)
			{
				if(Light4_Code[j]) id[3]+=Light4_Code[j]<<(7-1-j);	
			}


			printf("Light1:(%d,%d)    ",threelight.light_loc[0][0],threelight.light_loc[0][1]);
			printf("Code1:");
			printf("%d",id[0]);		
			printf("\n");

			printf("Light2:(%d,%d)    ",threelight.light_loc[1][0],threelight.light_loc[1][1]);
			printf("Code2:");
			printf("%d",id[1]);	
			printf("\n");

			printf("Light3:(%d,%d)    ",threelight.light_loc[2][0],threelight.light_loc[2][1]);
			printf("Code3:");
			printf("%d",id[2]);	
			printf("\n");

			printf("Light4:(%d,%d)    ",threelight.light_loc[3][0],threelight.light_loc[3][1]);
			printf("Code4:");
			printf("%d",id[3]);	
			printf("\n");

			if(threelight.light_num>4)
			{
				Best_Code(Light5_Code,decode5,code_num5);
				for(j = 0;j<7;j++)
				{
					if(Light5_Code[j]) id[4]+=Light5_Code[j]<<(7-1-j);	
				}
				printf("Light5:(%d,%d)    ",threelight.light_loc[4][0],threelight.light_loc[4][1]);
				printf("Code5:");
				printf("%d",id[4]);	
				printf("\n");

			}
			/*printf("Light1:(%d,%d)    ",threelight.light_loc[0][1],threelight.light_loc[0][0]);
			printf("Code1:");
			for(i = 0;i<CODE_LEN;i++)
			{
				printf("%d",Light1_Code[i]);
			}
			printf("\n");
			printf("Light2:(%d,%d)    ",threelight.light_loc[1][1],threelight.light_loc[1][0]);
			printf("Code2:");
			for(i = 0;i<CODE_LEN;i++)
			{
				printf("%d",Light2_Code[i]);
			}
			printf("\n");
			printf("Light3:(%d,%d)    ",threelight.light_loc[2][1],threelight.light_loc[2][0]);
			printf("Code3:");
			for(i = 0;i<CODE_LEN;i++)
			{
				printf("%d",Light3_Code[i]);
			}
			printf("\n");
			printf("Light4:(%d,%d)    ",threelight.light_loc[3][1],threelight.light_loc[3][0]);
			printf("Code4:");
			for(i = 0;i<CODE_LEN;i++)
			{
				printf("%d",Light4_Code[i]);
			}
			if(threelight.light_num>4)
			{
				Best_Code(Light5_Code,decode5,code_num5);
				printf("\n");
				printf("Light5:(%d,%d)    ",threelight.light_loc[4][1],threelight.light_loc[4][0]);
				printf("Code5:");
				for(i = 0;i<CODE_LEN;i++)
				{
					printf("%d",Light5_Code[i]);
				}
			}*/
			/*u1 = threelight.light_loc[0][1];
			v1 = threelight.light_loc[0][0];
			u2 = threelight.light_loc[1][1];
			v2 = threelight.light_loc[1][0];
			u3 = threelight.light_loc[2][1];
			v3 = threelight.light_loc[2][0];
			u4 = threelight.light_loc[3][1];
			v4 = threelight.light_loc[3][0];
			u5 = threelight.light_loc[4][1];
			v5 = threelight.light_loc[4][0];*/

			
			u1 = threelight.light_loc[0][0];
			v1 = threelight.light_loc[0][1];
			u2 = threelight.light_loc[1][0];
			v2 = threelight.light_loc[1][1];
			u3 = threelight.light_loc[2][0];
			v3 = threelight.light_loc[2][1];
			u4 = threelight.light_loc[3][0];
			v4 = threelight.light_loc[3][1];
			u5 = threelight.light_loc[4][0];
			v5 = threelight.light_loc[4][1];


			u[0] = u1;u[1] = u2;u[2] = u3;u[3] = u4; u[4] = u5;
			v[0] = v1;v[1] = v2;v[2] = v3;v[3] = v4; v[4] = v5;

			printf("\n");
			printf("\n");
			code_num1 = 0;
			code_num2 = 0;
			code_num3 = 0;
			code_num4 = 0;
			code_num5 = 0;

			/*imshow("ԭͼ",src);
			waitKey(0);*/
			//imshow("ͼ4",code4_image);
			////imshow("ͼ3",code3_image);
			////imshow("ͼ4",code4_image);
			////imshow("ͼ5",code5_image);
			//waitKey(0);

			if(threelight.light_num==6)threelight.light_num--;
			//namedWindow("enhanced",0);
			//resizeWindow("enhanced", 640, 480);
			//imshow("enhanced",code4_image);
			//waitKey(0);
			//if(threelight.light_loc[0][0]==62&&threelight.light_loc[0][1]==472){
			///*imshow("code3_image",code3_image);
			//waitKey(0);*/
			//	//imshow("code3_image",code5_image);
			//	//waitkey(0);
			///*namedWindow("enhanced",0);
			//resizeWindow("enhanced", 640, 480);*/
			//imshow("enhanced",src);
			//waitKey(0);
			//}
			/*namedWindow("enhanced",0);
			resizeWindow("enhanced", 640, 480);
			imshow("enhanced",src);
			waitKey(0);*/
			//	/*imshow("code2_image",src);
			//	waitKey(0);*/
			//	//imshow("code3_image",code5_image);
			//	//waitkey(0);
			//}
			Location(u,v,id,&X,&Y,&Z,threelight.light_num);
			X = X*1536/4000-10;
			Y = Y*1536/4000-20;
			Z = Z;
			buf[0] = 1; buf[1] = X; buf[2] = Y;
			/*if(-3500<Z&&Z<-2000)*/ WLAN_SEND(buf,0);
			cout<<threelight.light_num;
			//if(threelight.light_loc[0][0]==62&&threelight.light_loc[0][1]==472){
			/*imshow("code3_image",code3_image);
			waitKey(0);*/
				//imshow("code3_image",code5_image);
				//waitkey(0);
			/*namedWindow("enhanced",0);
			resizeWindow("enhanced", 640, 480);
			imshow("enhanced",src);
			waitKey(0);*/
			//}
			cout<<endl<<endl;

		}

		}
		//imshow("ԭͼ",src);
		//waitKey(0);

	}
	//Best_Code(Light1_Code,decode1,code_num1);
	//Best_Code(Light2_Code,decode2,code_num2);
	//Best_Code(Light3_Code,decode3,code_num3);

	/*imshow("ԭͼ",src);
	waitKey(0);*/

while (1)
{

}

}


/* To get the OffSet of header to data part */
void bmpHeaderPartLength(FILE* fpbmp)
{
	fseek(fpbmp, 10L, SEEK_SET);
	fread(&OffSet, sizeof(char), 4, fpbmp);
	//  printf("The Header Part is of length %d.\n", OffSet);
}


/* To get the width and height of the bmp file */
void bmpWidthHeight(FILE* fpbmp)
{
	fseek(fpbmp, 18L, SEEK_SET);
	fread(&width, sizeof(char), 4, fpbmp);
	fseek(fpbmp, 22L, SEEK_SET);
	fread(&height, sizeof(char), 4, fpbmp);
	// printf("The Width of the bmp file is %ld.\n", width);
	//  printf("The Height of the bmp file is %ld.\n", height);
}