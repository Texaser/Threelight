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
unsigned int OffSet = 0;    // bmp�ļ�ͷ��������Ϣ֮��ĳ���
long width=1536;          // ͼƬ��
long height=1024;         // ͼƬ��
//long n;       //ע������ȫ�ֱ���nʱĬ�ϸ�ֵ0������main������ʱ�Ǹ���
static unsigned int framenum = 17; //��ȡͼƬ��
int gray[17][1024][1536];
//unsigned char pixel[17][1024 * 1536];  //��ȡbmp�ļ�ʱҪ��char���飬����ʱ��int���鷽��һЩ
int gray_average[1024][1536];  //17��ͼƬ��ƽ��
unsigned char p[1024 * 1536];  //�ݴ�bmp�ļ�



void main(void)
{
    FILE* obj = NULL;
    errno_t err;
  //  int file_length = 0;


    unsigned int num = 0;
    long obj_length = width * height;
    char filename[50];//�ļ���
    int i;
    int j;
    int k;

    //ע���������з����Ǵ���ģ�����sizeof(unsigned char*) * 17
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
        bmpHeaderPartLength(obj);  //��ȡ�ļ�ͷ����
        bmpWidthHeight(obj);       //��ȡͼƬ��͸�
        
        //file_length = (sizeof(filename)) / sizeof(char);

        
        //��λͼ���ݿ�ʼ��height*width������(8λ���أ�1����=1�ֽ�)
        fseek(obj, OffSet, SEEK_SET);
        //ע�⶯̬���ɵ���ָ�룬�������治��&pixel����pixel
        fread(pixel[num - 1], sizeof(unsigned char), obj_length, obj);
        //����ͼ���С��̬������������

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
        //ȡ1��17֡ͼƬ��ƽ������ȷ����Դ��������

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
        //ע���������ʵ��ԭ�����ϼӣ�����n��������ֵ�Ļ���ÿ�ζ�֮�������0�ٶ�������һֱ�ۼ�
        //��������Դ��ͼƬ�л��ֳ���,��Ϊ����ͼ,ͬʱ����ͼ��ʱ����ת90��
        int W[3], H[3];   //����ͼƬ�Ŀ�Ⱥ͸߶�
        for (i = 0; i < 3; i++)
        {
            W[i] = threelight.light_range[i][3] - threelight.light_range[i][2];
            H[i] = threelight.light_range[i][1] - threelight.light_range[i][0];
        }
        //��̬�����ڴ�ʱ�з���ʧ�ܵķ���,Ԥ��һ����Դ
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

        //����������Դ��Χ��ѭ������17��ͼƬ
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
            /*************************����***************************/
            flag1 = EVA_dec(source1, H[0], W[0], code1);
            flag2 = EVA_dec(source2, H[1], W[1], code2);
            flag3 = EVA_dec(source3, H[2], W[2], code3);
            if (flag1 + flag2 + flag3 == 3)//������Դ������ɹ�
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



 //   /******************************������������ò���*******************************/
 //   int nRet = -1;
 //   void* m_handle = NULL;

 //   //ö��������ָ���Ĵ���Э���Ӧ�������豸
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

 //   //ѡ����ҵ��ĵ�һ̨�����豸�������豸���
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

 //       //�����豸
 //       nRet = MV_CC_OpenDevice(m_handle, nAccessMode, nSwitchoverKey);
 //       if (MV_OK != nRet)
 //       {
 //           printf("error: OpenDevice fail [%x]\n", nRet);
 //           return;
 //       }
 //       //...��������      
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
 //   //��ʼ�ɼ�ͼ��
 //   nRet = MV_CC_StartGrabbing(m_handle);
 //   if (MV_OK != nRet)
 //   {
 //       printf("error: StartGrabbing fail [%x]\n", nRet);
 //       return;
 //   }

 //   //��ȡһ֡���ݵĴ�С
 //   MVCC_INTVALUE stIntvalue = { 0 };
 //   nRet = MV_CC_GetIntValue(m_handle, "PayloadSize", &stIntvalue);
 //   if (nRet != MV_OK)
 //   {
 //       printf("Get PayloadSize failed! nRet [%x]\n", nRet);
 //       return;
 //   }
 //   int nBufSize = stIntvalue.nCurValue; //һ֡���ݴ�С

 //   unsigned int    nTestFrameSize = 0;
 //   unsigned char* pFrameBuf = NULL;
 //   pFrameBuf = (unsigned char*)malloc(nBufSize);

 //   MV_FRAME_OUT_INFO_EX stInfo;
 //   memset(&stInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));

 //   MV_DISPLAY_FRAME_INFO stDisplayOneFrame;
 //   memset(&stDisplayOneFrame, 0, sizeof(MV_DISPLAY_FRAME_INFO));

 //   //�ϲ�Ӧ�ó�����Ҫ����֡�ʣ����ƺõ��øýӿڵ�Ƶ��
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





 //   //ȡ1��17֡ͼƬ��ƽ������ȷ����Դ��������

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
 //   //ע���������ʵ��ԭ�����ϼӣ�����n��������ֵ�Ļ���ÿ�ζ�֮�������0�ٶ�������һֱ�ۼ�
 //   //��������Դ��ͼƬ�л��ֳ���,��Ϊ����ͼ,ͬʱ����ͼ��ʱ����ת90��
 //   int W[3], H[3];   //����ͼƬ�Ŀ�Ⱥ͸߶�
 //   for (i = 0; i < 3; i++)
 //   {
 //       W[i] = threelight.light_range[i][3] - threelight.light_range[i][2];
 //       H[i] = threelight.light_range[i][1] - threelight.light_range[i][0];
 //   }
 //   //��̬�����ڴ�ʱ�з���ʧ�ܵķ���,Ԥ��һ����Դ
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

 //   //����������Դ��Χ��ѭ������17��ͼƬ
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
 //       /*************************����***************************/
 //       flag1 = EVA_dec(source1, H[0], W[0], code1);
 //       flag2 = EVA_dec(source2, H[1], W[1], code2);
 //       flag3 = EVA_dec(source3, H[2], W[2], code3);
 //       if (flag1 + flag2 + flag3 == 3)//������Դ������ɹ�
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