#include<stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "Source_loc.h"
#include"EVA_dec.h"
#include "windows.h"
#include "MvCameraControl.h"
using namespace cv;
 

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
unsigned int OffSet = 0;    // bmp文件头到像素信息之间的长度
long width=1536;          // 图片宽
long height=1024;         // 图片高
//long n;       //注意声明全局变量n时默认赋值0，而在main里声明时是负的
static unsigned int framenum = 17; //读取图片数
int gray[17][1024][1536];
//unsigned char pixel[17][1024 * 1536];  //读取bmp文件时要用char数组，处理时用int数组方便一些
int gray_average[1024][1536];  //17张图片的平均
unsigned char p[1024 * 1536];  //暂存bmp文件



void main(void)
{
    FILE* obj = NULL;
    errno_t err;
  //  int file_length = 0;


    unsigned int num = 0;
    long obj_length = width * height;
    char filename[50];//文件名
    int i;
    int j;
    int k;

    //注意下面折中分配是错误的，必须sizeof(unsigned char*) * 17
    //unsigned char** pi = (unsigned char**)malloc(sizeof(unsigned char) * 17);
    //for (num = 0; num < 17; num++)
    //{
    //    pi[num] = num;
    //}
    //free(pi);

    unsigned char** pixel = (unsigned char**)malloc(sizeof(unsigned char*) * framenum);
    for (num = 1; num <= framenum; num++)
    {
        pixel[num-1] = (unsigned char*)malloc(sizeof(unsigned char)* obj_length);
        sprintf_s(filename, 50, "d:\\camera_project\\Threelight\\pic\\pic_%d.bmp", num);
        err = fopen_s(&obj, filename, "r");
        bmpHeaderPartLength(obj);  //获取文件头长度
        bmpWidthHeight(obj);       //获取图片宽和高
        
        //file_length = (sizeof(filename)) / sizeof(char);

        
        //从位图数据开始读height*width个像素(8位像素，1像素=1字节)
        fseek(obj, OffSet, SEEK_SET);
        //注意动态生成的是指针，所以下面不是&pixel而是pixel
        fread(pixel[num - 1], sizeof(unsigned char), obj_length, obj);
        //根据图像大小动态生成像素数组

        //unsigned char** gray = (unsigned char**)malloc(sizeof(unsigned char) * height);
        //for (i = 0; i<height; i++) 
        //{
        //   gray[i] = (unsigned char*)malloc(sizeof(unsigned char) * width);
        //}

        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                gray[num - 1][height - 1 - i][j] = pixel[num - 1][i * width + j];
            }
        }
    }
    
    fclose(obj);

    
    while (1)
    {
        //取1秒17帧图片的平均用来确定光源像素坐标

        for (k = 0; k < framenum; k++)
        {
            for (i = 0; i < height; i++)
            {
                for (j = 0; j < width; j++)
                {
                    gray_average[i][j] += gray[k][i][j];
                }
            }
        }
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < width; j++)
            {
                gray_average[i][j] = gray_average[i][j] / framenum;
            }
        }


        Source_loc(gray_average);
        //注意读入数据实在原基础上加，即用n承载像素值的话，每次读之后必须清0再读，否则一直累加
        //将三个光源从图片中划分出来,成为三张图,同时三张图逆时针旋转90度
        int W[3], H[3];   //三张图片的宽度和高度
        for (i = 0; i < 3; i++)
        {
            W[i] = threelight.light_range[i][3] - threelight.light_range[i][2];
            H[i] = threelight.light_range[i][1] - threelight.light_range[i][0];
        }
        //动态分配内存时有分配失败的风险,预留一点资源
        int** source1 = (int**)malloc(sizeof(int) * (W[0] + 5));
        for (i = 0; i < W[0]; i++)
        {
            source1[i] = (int*)malloc(sizeof(int) * (H[0] + 5));
        }
        int** source2 = (int**)malloc(sizeof(int) * (W[1] + 5));
        for (i = 0; i < W[1]; i++)
        {
            source2[i] = (int*)malloc(sizeof(int) * (H[1] + 5));
        }
        int** source3 = (int**)malloc(sizeof(int) * (W[2] + 5));
        for (i = 0; i < W[2]; i++)
        {
            source3[i] = (int*)malloc(sizeof(int) * (H[2] + 5));
        }
        printf("(x,y):\n");
        printf("source1("); printf("%d,%d", threelight.light_loc[0][0], threelight.light_loc[0][1]); printf(")\n");
        printf("source2("); printf("%d,%d", threelight.light_loc[1][0], threelight.light_loc[1][1]); printf(")\n");
        printf("source3("); printf("%d,%d", threelight.light_loc[2][0], threelight.light_loc[2][1]); printf(")\n");

        //划定三个光源范围后，循环处理17张图片
        for (num = 0; num < framenum; num++)
        {
            for (i = 0; i < W[0]; i++)
            {
                for (j = 0; j < H[0]; j++)
                {
                    source1[i][j] = gray[num][threelight.light_range[0][0] + j][threelight.light_range[0][3] - 1 - i];
                }
            }
            for (i = 0; i < W[1]; i++)
            {
                for (j = 0; j < H[1]; j++)
                {
                    source2[i][j] = gray[num][threelight.light_range[1][0] + j][threelight.light_range[1][3] - 1 - i];
                }
            }
            for (i = 0; i < W[2]; i++)
            {
                for (j = 0; j < H[2]; j++)
                {
                    source3[i][j] = gray[num][threelight.light_range[2][0] + j][threelight.light_range[2][3] - 1 - i];
                }
            }
            /*************************解码***************************/
            flag1 = EVA_dec(source1, H[0], W[0], code1);
            flag2 = EVA_dec(source2, H[1], W[1], code2);
            flag3 = EVA_dec(source3, H[2], W[2], code3);
            if (flag1 + flag2 + flag3 == 3)//三个光源都解码成功
            {
                printf("Code1:");
                for (i = 0; i < 8; i++)
                {
                    printf("%d", code1[i]);
                }
                printf("\n");
                printf("Code2:");
                for (i = 0; i < 8; i++)
                {
                    printf("%d", code2[i]);
                }
                printf("\n");
                printf("Code3:");
                for (i = 0; i < 8; i++)
                {
                    printf("%d", code3[i]);
                }
                printf("\n");
                break;
            }
            flag1 = 0; flag2 = 0; flag3 = 0;
        }
        free(source1);
        free(source2);
        free(source3);
        free(pixel);
        

    }



 //   /******************************连接相机，设置参数*******************************/
 //   int nRet = -1;
 //   void* m_handle = NULL;

 //   //枚举子网内指定的传输协议对应的所有设备
 //   unsigned int nTLayerType = MV_GIGE_DEVICE | MV_USB_DEVICE;
 //   MV_CC_DEVICE_INFO_LIST m_stDevList = { 0 };
 //   nRet = MV_CC_EnumDevices(nTLayerType, &m_stDevList);
 //   if (MV_OK != nRet)
 //   {
 //       printf("error: EnumDevices fail [%x]\n", nRet);
 //       return;
 //   }
 //   if (m_stDevList.nDeviceNum == 0)
 //   {
 //       printf("no camera found!\n");
 //       return;
 //   }

 //   //选择查找到的第一台在线设备，创建设备句柄
 //   int nDeviceIndex = 0;

 //   MV_CC_DEVICE_INFO m_stDevInfo = { 0 };
 //   memcpy(&m_stDevInfo, m_stDevList.pDeviceInfo[nDeviceIndex], sizeof(MV_CC_DEVICE_INFO));

 //   nRet = MV_CC_CreateHandle(&m_handle, &m_stDevInfo);

 //   if (MV_OK != nRet)
 //   {
 //       printf("error: CreateHandle fail [%x]\n", nRet);
 //       return;
 //   }

 //   else
 //   {
 //       unsigned int nAccessMode = MV_ACCESS_Exclusive;
 //       unsigned short nSwitchoverKey = 0;

 //       //连接设备
 //       nRet = MV_CC_OpenDevice(m_handle, nAccessMode, nSwitchoverKey);
 //       if (MV_OK != nRet)
 //       {
 //           printf("error: OpenDevice fail [%x]\n", nRet);
 //           return;
 //       }
 //       //...其他处理      
 //   }

 //   unsigned int nValue = 1536;
 //   nRet = MV_CC_SetIntValue(m_handle, "Width", nValue);
 //   if (MV_OK != nRet)
 //   {
 //       printf("error: SetIntValue fail [%x]\n", nRet);
 //       return;
 //   }
 //   nValue = 1024;
 //   nRet = MV_CC_SetIntValue(m_handle, "Height", nValue);
 //   if (MV_OK != nRet)
 //   {
 //       printf("error: SetIntValue fail [%x]\n", nRet);
 //       return;
 //   }
 //   float fValue = 27;
 //   nRet = MV_CC_SetFloatValue(m_handle, "ExposureTime", fValue);
 //   if (MV_OK != nRet)
 //   {
 //       printf("error: SetFloatValue fail [%x]\n", nRet);
 //       return;
 //   }
 //   //开始采集图像
 //   nRet = MV_CC_StartGrabbing(m_handle);
 //   if (MV_OK != nRet)
 //   {
 //       printf("error: StartGrabbing fail [%x]\n", nRet);
 //       return;
 //   }

 //   //获取一帧数据的大小
 //   MVCC_INTVALUE stIntvalue = { 0 };
 //   nRet = MV_CC_GetIntValue(m_handle, "PayloadSize", &stIntvalue);
 //   if (nRet != MV_OK)
 //   {
 //       printf("Get PayloadSize failed! nRet [%x]\n", nRet);
 //       return;
 //   }
 //   int nBufSize = stIntvalue.nCurValue; //一帧数据大小

 //   unsigned int    nTestFrameSize = 0;
 //   unsigned char* pFrameBuf = NULL;
 //   pFrameBuf = (unsigned char*)malloc(nBufSize);

 //   MV_FRAME_OUT_INFO_EX stInfo;
 //   memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

 //   MV_DISPLAY_FRAME_INFO stDisplayOneFrame;
 //   memset(&stDisplayOneFrame, 0, sizeof(MV_DISPLAY_FRAME_INFO));

 //   //上层应用程序需要根据帧率，控制好调用该接口的频率
 //  while(1)
 //  {
 //   while (1)
 //   {
 //       if (nTestFrameSize > framenum - 1)
 //       {
 //           break;
 //       }
 //       nRet = MV_CC_GetOneFrameTimeout(m_handle, pFrameBuf, nBufSize, &stInfo, 1000);
 //       if (MV_OK != nRet)
 //       {
 //           Sleep(10);
 //       }
 //       else
 //       {
 //           for (i = 0; i < height; i++)
 //           {
 //               for (j = 0; j < width; j++)
 //               {
 //                   gray[nTestFrameSize][height - 1 - i][j] = pFrameBuf[i * width + j];
 //               }
 //           }
 //           /*for (i = 0; i < 1536; i++)
 //           {
 //               for (j = 0; j < 1024; j++)
 //               {
 //                   pixel[i][j] = pFrameBuf[i * 1024 + j];
 //               }
 //           }*/
 //           nTestFrameSize++;
 //       }
 //   }





 //   //取1秒17帧图片的平均用来确定光源像素坐标

 //   for (k = 0; k < framenum; k++)
 //   {
 //       for (i = 0; i < height; i++)
 //       {
 //           for (j = 0; j < width; j++)
 //           {
 //               gray_average[i][j] += gray[k][i][j];
 //           }
 //       }
 //   }
 //   for (i = 0; i < height; i++)
 //   {
 //       for (j = 0; j < width; j++)
 //       {
 //           gray_average[i][j] = gray_average[i][j] / framenum;
 //       }
 //   }

 //   Source_loc(gray_average);
 //   //注意读入数据实在原基础上加，即用n承载像素值的话，每次读之后必须清0再读，否则一直累加
 //   //将三个光源从图片中划分出来,成为三张图,同时三张图逆时针旋转90度
 //   int W[3], H[3];   //三张图片的宽度和高度
 //   for (i = 0; i < 3; i++)
 //   {
 //       W[i] = threelight.light_range[i][3] - threelight.light_range[i][2];
 //       H[i] = threelight.light_range[i][1] - threelight.light_range[i][0];
 //   }
 //   //动态分配内存时有分配失败的风险,预留一点资源
 //   int** source1 = (int**)malloc(sizeof(int) * (W[0] + 5));
 //   for (i = 0; i < W[0]; i++)
 //   {
 //       source1[i] = (int*)malloc(sizeof(int) * (H[0] + 5));
 //   }
 //   int** source2 = (int**)malloc(sizeof(int) * (W[1] + 5));
 //   for (i = 0; i < W[1]; i++)
 //   {
 //       source2[i] = (int*)malloc(sizeof(int) * (H[1] + 5));
 //   }
 //   int** source3 = (int**)malloc(sizeof(int) * (W[2] + 5));
 //   for (i = 0; i < W[2]; i++)
 //   {
 //       source3[i] = (int*)malloc(sizeof(int) * (H[2] + 5));
 //   }
 //   printf("(x,y):\n");
 //   printf("source1("); printf("%d,%d", threelight.light_loc[0][0], threelight.light_loc[0][1]); printf(")\n");
 //   printf("source2("); printf("%d,%d", threelight.light_loc[1][0], threelight.light_loc[1][1]); printf(")\n");
 //   printf("source3("); printf("%d,%d", threelight.light_loc[2][0], threelight.light_loc[2][1]); printf(")\n");

 //   //划定三个光源范围后，循环处理17张图片
 //   for (num = 0; num < framenum; num++)
 //   {
 //       for (i = 0; i < W[0]; i++)
 //       {
 //           for (j = 0; j < H[0]; j++)
 //           {
 //               source1[i][j] = gray[num][threelight.light_range[0][0] + j][threelight.light_range[0][3] - 1 - i];
 //           }
 //       }
 //       for (i = 0; i < W[1]; i++)
 //       {
 //           for (j = 0; j < H[1]; j++)
 //           {
 //               source2[i][j] = gray[num][threelight.light_range[1][0] + j][threelight.light_range[1][3] - 1 - i];
 //           }
 //       }
 //       for (i = 0; i < W[2]; i++)
 //       {
 //           for (j = 0; j < H[2]; j++)
 //           {
 //               source3[i][j] = gray[num][threelight.light_range[2][0] + j][threelight.light_range[2][3] - 1 - i];
 //           }
 //       }
 //       /*************************解码***************************/
 //       flag1 = EVA_dec(source1, H[0], W[0], code1);
 //       flag2 = EVA_dec(source2, H[1], W[1], code2);
 //       flag3 = EVA_dec(source3, H[2], W[2], code3);
 //       if (flag1 + flag2 + flag3 == 3)//三个光源都解码成功
 //       {
 //           printf("Code1:");
 //           for (i = 0; i < 8; i++)
 //           {
 //               printf("%d", code1[i]);
 //           }
 //           printf("\n");
 //           printf("Code2:");
 //           for (i = 0; i < 8; i++)
 //           {
 //               printf("%d", code2[i]);
 //           }
 //           printf("\n");
 //           printf("Code3:");
 //           for (i = 0; i < 8; i++)
 //           {
 //               printf("%d", code3[i]);
 //           }
 //           printf("\n");
 //           break;
 //       }
 //       flag1 = 0; flag2 = 0; flag3 = 0;
 //   }

 //   free(source1);
 //   free(source2);
 //   free(source3);

 //}
    //  while (1);
  //   fclose(obj);
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