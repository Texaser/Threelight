//确定图片中光源个数、三个光源的像素坐标和范围

#include"Source_loc.h"
#include"math.h"

#define LAMP_LEN1 380 //根据实际情况调整
#define LAMP_LEN2 90
#define PI 3.14159265
#define MIN_VAL 20

int pic[1024][1536];
int P[1024][1536];//临时储存pic
int t1,t2,t3,t4;
//double x1,x2,y2; //旋转
int height = 1024;
int width = 1536;
int BWlabelpic[1024][1536];
//图片里可能不止3个光源，最多认为有10个光源
int label[10][1024*1536][2];  //label[i][k][0]保存第k个，标记为i+1的像素横坐标,...[1]保存纵坐标
pic_divide threelight;
double d;
double angle;
double e,f,g,h;


Mat dilation_dst; 
int bit_distance = 3;
int dilation_elem = 0;
int dilation_size = bit_distance*5;
/** @function Dilation */
//膨胀运算后的结果存入dilation_dst

//定义arctan(x)函数;
double atan(double x);

void Dilation(Mat gray_image)
{
	int dilation_type;
	if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
	else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
	else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( dilation_type,
		Size( 2*dilation_size + 1, 2*dilation_size+1 ),
		Point( dilation_size, dilation_size ) );
	///膨胀操作
	dilate(gray_image, dilation_dst, element );
	//imshow( "Dilation Demo", dilation_dst );
	//waitKey(0);
}

/**********************调整光心位置*********************/
//input: N:上边缘坐标  S:下边缘坐标
//output: 调整后光心坐标
int Adjust_Core1(int N,int S)
{
	//灯在上边缘
	if(S<LAMP_LEN1) return S-LAMP_LEN1/2;
	//灯在下边缘
	else if(N>height-LAMP_LEN1) return N+LAMP_LEN1/2;
	else return (N+S)/2;
}

//input: W:左边缘坐标  E:右边缘坐标
//output: 调整后光心坐标

int Adjust_Core2(int W, int E)
{
	//灯在左边缘
	if(E<LAMP_LEN2) return E-LAMP_LEN2/2;
	//灯在右边缘
	else if(W>width-LAMP_LEN2) return W+LAMP_LEN2/2;
	else return (W+E)/2;
}


double Degree(int BWlabelpic[1024][1536])
{
	int i,j;
	int loc1[2],loc2[2];
	//从左到右，从上到下扫描，找到第一个标记点
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if(BWlabelpic[i][j]==2) break;
		}
		if(BWlabelpic[i][j]==2) break;
	}
	loc1[0] = i; loc1[1] = j;
	e = i;
	g = j;
	//往下找到100个像素处的标记点坐标
	i+=100;
	for(j=0;j<width;j++)
	{
		if(BWlabelpic[i][j]==2) break;
	}
	loc2[0] = i; loc2[1] = j;
	f = i;
	h = j;
	double c;
	d = loc2[1]-loc1[1];
	c = sqrt(pow(100.0,2.0)+pow(loc2[1]-loc1[1],2.0));
	angle = asin((loc1[1]-loc2[1])/c)*180/PI;
	if(loc1[1]-loc2[1]>=0) return asin((loc1[1]-loc2[1])/c)*180/PI;
	else return asin((loc2[1]-loc1[1])/c)*180/PI;

	/*else{
		c = sqrt(pow(50.0,2.0)+pow(loc1[1]-loc2[1],2.0));
		return asin((loc1[1]-loc2[1])/c)*180/PI;
	}*/

	//return 50/(loc2[1]-loc1[1]>0?loc2[1]-loc1[1]:loc1[1]-loc2[1]);
	//角度可以取负值

	//return angle;
}


//angle为逆时针旋转的角度 x3,y3为旋转之前的坐标
double rotate_point_x(double x1, double y1, double degree)
{
	double x3;
	x1 = x1;
	double x2 = 1536/2;
	y1 = 1024 - y1;
	double y2 = 1024/2;
	x3 = (x1-x2)*cos(degree * PI / 180) - (y1-y2)*sin(degree * PI / 180) + x2;
	return x3;
}

double rotate_point_y(double x1, double y1, double degree)
{
	double y3;
	x1 = x1;
	double x2 = width/2;
	y1 = height - y1;
	double y2 = height/2;
	y3 = (y1-y2)*cos(degree * PI / 180) + (x1-x2)*sin(degree * PI / 180) + y2;
	return height-y3;
}

void Source_loc(Mat gray_image)
{
	int num=0;
	unsigned char flag=0;
	int i,j;
	int k[10] = {0};
	int L;
	Mat BW_image;
	//二值化,OTSU设置最大值255就行,阈值多少不影响，随便设置个127
	threshold(gray_image, BW_image, 127, 255, THRESH_OTSU);
	//膨胀运算
	Dilation(BW_image);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			//pic[i][j] = dilation_dst.data[i * width + j];
			//if(pic[i][j]>0) pic[i][j]=1;    //BWLabel函数只处理0/1二值区域
			if(dilation_dst.data[i * width + j]>0&&gray_image.data[i * width + j]> MIN_VAL)
				pic[i][j] = 1;
		}
	}
	//for (i = 0; i < height; i++)
	//{
	//	for (j = 0; j < width; j++)
	//	{
	//		pic[i][j] = dilation_dst.data[i * width + j];
	//		if(pic[i][j]>0) pic[i][j]=1;    //BWLabel函数只处理0/1二值区域
	//	}
	//}

	//imshow("tt",dilation_dst);
	//waitKey(0);
	//vs2019这里可以直接pic[0]

	//一定要注意这里必须初始化一次！！因为是全局变量（不能在函数里申请，没法给那么大空间），所以会积累，一张张图片重叠下去
	for(i=0;i<1024;i++)
	{
		for(j=0;j<1536;j++)
		{
			BWlabelpic[i][j] = 0;
		}
	}

	threelight.light_num = BWLabel(pic[0], BWlabelpic[0], 1536, 1024, 8);

	////查看联通后的图片效果，由暗到明标记
	/*for(i=0;i<1024;i++)
	{
		for(j=0;j<1536;j++)
		{
			if(BWlabelpic[i][j]>0) BWlabelpic[i][j] = BWlabelpic[i][j]*10;
		}
	}
	Mat PIC;
	PIC = Mat::zeros(1024,1536, CV_8UC1);
	for (i = 0; i < 1024; i++)
			{
				for (j = 0; j < 1536; j++)
				{
					
					PIC.data[1536*i+j] = BWlabelpic[i][j];
				}
			}*/
	//namedWindow("enhanced",0);
	//resizeWindow("enhanced", 640, 480);
	//imshow("enhanced",PIC);
	//waitKey(0);

	if(threelight.light_num<3)
	{
		//printf("Error:light_num is less than three", threelight.light_num);
		return;
	}
	//光源数量最大只处理10个
	threelight.light_num = (10<threelight.light_num) ? 10:threelight.light_num;
	for (i = 0; i < 1024; i++)
	{
		for (j = 0; j < 1536; j++)
		{
			L = BWlabelpic[i][j];
			if(L>0)   //0为黑色区域
			{
				if(L<=threelight.light_num)
				{
					L--;//索引-1
					label[L][k[L]][0] = j;
					label[L][k[L]][1] = i;
					k[L]++;
				}
			}
			//switch (BWlabelpic[i][j])
			//{
			//case 1:
			//	label[0][k[0]][0] = j;
			//	label[0][k[0]][1] = i;
			//	k[0]++;
			//	break;
			//case 2:
			//	label[1][k[1]][0] = j;
			//	label[1][k[1]][1] = i;
			//	k[1]++;
			//	break;
			//case 3:
			//	label[2][k[2]][0] = j;
			//	label[2][k[2]][1] = i;
			//	k[2]++;
			//	break;
			//default:
			//	break;
			//}
		}
	}

	for(j=0;j<threelight.light_num;j++)
	{ 
		t1 = 1023, t2 = 0, t3 = 1535, t4 = 0;
		for (i = 0; i <= k[j] - 1; i++)
		{
			if (t1 > label[j][i][1]) { t1 = label[j][i][1]; }	//上边界
			if (t2 < label[j][i][1]) { t2 = label[j][i][1]; }	//下边界	
			if (t3 > label[j][i][0]) { t3 = label[j][i][0]; }	//左边界
			if (t4 < label[j][i][0]) { t4 = label[j][i][0]; }	//右边界
		}
		//根据长度筛选出可解码的光源,num表示可解码光源的数目
		if(t2-t1>100)
		{
			threelight.light_range[num][0] = t1;
			threelight.light_range[num][1] = t2;
			threelight.light_range[num][2] = t3;
			threelight.light_range[num][3] = t4;
			num++;
		}
	}
	threelight.light_num = num;
	//取矩形区域se的质心近似为光源质心
	if((abs(threelight.light_loc[0][0]-Adjust_Core2(threelight.light_range[0][2] , threelight.light_range[0][3])) >15)||
		(abs(threelight.light_loc[0][1]-Adjust_Core1(threelight.light_range[0][0] , threelight.light_range[0][1])) >15)){
	for (i = 0; i < threelight.light_num; i++)
	{
		threelight.light_loc[i][0] = (threelight.light_range[i][2] + threelight.light_range[i][3]) / 2;
		threelight.light_loc[i][1] = (threelight.light_range[i][0] + threelight.light_range[i][1]) / 2;
		threelight.light_loc[i][0] = Adjust_Core2(threelight.light_range[i][2],threelight.light_range[i][3]);
		threelight.light_loc[i][1] = Adjust_Core1(threelight.light_range[i][0],threelight.light_range[i][1]);
		double a = rotate_point_x(threelight.light_loc[i][0], threelight.light_loc[i][1], Degree(BWlabelpic));
		double b = rotate_point_y(threelight.light_loc[i][0], threelight.light_loc[i][1], Degree(BWlabelpic));
		threelight.light_loc[i][0] = a;
		threelight.light_loc[i][1] = b;
	}
	}
}


//