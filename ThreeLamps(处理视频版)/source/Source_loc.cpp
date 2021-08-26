//ȷ��ͼƬ�й�Դ������������Դ����������ͷ�Χ

#include"Source_loc.h"
#include"math.h"

#define LAMP_LEN1 380 //����ʵ���������
#define LAMP_LEN2 90
#define PI 3.14159265
#define MIN_VAL 20

int pic[1024][1536];
int P[1024][1536];//��ʱ����pic
int t1,t2,t3,t4;
//double x1,x2,y2; //��ת
int height = 1024;
int width = 1536;
int BWlabelpic[1024][1536];
//ͼƬ����ܲ�ֹ3����Դ�������Ϊ��10����Դ
int label[10][1024*1536][2];  //label[i][k][0]�����k�������Ϊi+1�����غ�����,...[1]����������
pic_divide threelight;
double d;
double angle;
double e,f,g,h;


Mat dilation_dst; 
int bit_distance = 3;
int dilation_elem = 0;
int dilation_size = bit_distance*5;
/** @function Dilation */
//���������Ľ������dilation_dst

//����arctan(x)����;
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
	///���Ͳ���
	dilate(gray_image, dilation_dst, element );
	//imshow( "Dilation Demo", dilation_dst );
	//waitKey(0);
}

/**********************��������λ��*********************/
//input: N:�ϱ�Ե����  S:�±�Ե����
//output: �������������
int Adjust_Core1(int N,int S)
{
	//�����ϱ�Ե
	if(S<LAMP_LEN1) return S-LAMP_LEN1/2;
	//�����±�Ե
	else if(N>height-LAMP_LEN1) return N+LAMP_LEN1/2;
	else return (N+S)/2;
}

//input: W:���Ե����  E:�ұ�Ե����
//output: �������������

int Adjust_Core2(int W, int E)
{
	//�������Ե
	if(E<LAMP_LEN2) return E-LAMP_LEN2/2;
	//�����ұ�Ե
	else if(W>width-LAMP_LEN2) return W+LAMP_LEN2/2;
	else return (W+E)/2;
}


double Degree(int BWlabelpic[1024][1536])
{
	int i,j;
	int loc1[2],loc2[2];
	//�����ң����ϵ���ɨ�裬�ҵ���һ����ǵ�
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
	//�����ҵ�100�����ش��ı�ǵ�����
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
	//�Ƕȿ���ȡ��ֵ

	//return angle;
}


//angleΪ��ʱ����ת�ĽǶ� x3,y3Ϊ��ת֮ǰ������
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
	//��ֵ��,OTSU�������ֵ255����,��ֵ���ٲ�Ӱ�죬������ø�127
	threshold(gray_image, BW_image, 127, 255, THRESH_OTSU);
	//��������
	Dilation(BW_image);
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			//pic[i][j] = dilation_dst.data[i * width + j];
			//if(pic[i][j]>0) pic[i][j]=1;    //BWLabel����ֻ����0/1��ֵ����
			if(dilation_dst.data[i * width + j]>0&&gray_image.data[i * width + j]> MIN_VAL)
				pic[i][j] = 1;
		}
	}
	//for (i = 0; i < height; i++)
	//{
	//	for (j = 0; j < width; j++)
	//	{
	//		pic[i][j] = dilation_dst.data[i * width + j];
	//		if(pic[i][j]>0) pic[i][j]=1;    //BWLabel����ֻ����0/1��ֵ����
	//	}
	//}

	//imshow("tt",dilation_dst);
	//waitKey(0);
	//vs2019�������ֱ��pic[0]

	//һ��Ҫע����������ʼ��һ�Σ�����Ϊ��ȫ�ֱ����������ں��������룬û������ô��ռ䣩�����Ի���ۣ�һ����ͼƬ�ص���ȥ
	for(i=0;i<1024;i++)
	{
		for(j=0;j<1536;j++)
		{
			BWlabelpic[i][j] = 0;
		}
	}

	threelight.light_num = BWLabel(pic[0], BWlabelpic[0], 1536, 1024, 8);

	////�鿴��ͨ���ͼƬЧ�����ɰ��������
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
	//��Դ�������ֻ����10��
	threelight.light_num = (10<threelight.light_num) ? 10:threelight.light_num;
	for (i = 0; i < 1024; i++)
	{
		for (j = 0; j < 1536; j++)
		{
			L = BWlabelpic[i][j];
			if(L>0)   //0Ϊ��ɫ����
			{
				if(L<=threelight.light_num)
				{
					L--;//����-1
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
			if (t1 > label[j][i][1]) { t1 = label[j][i][1]; }	//�ϱ߽�
			if (t2 < label[j][i][1]) { t2 = label[j][i][1]; }	//�±߽�	
			if (t3 > label[j][i][0]) { t3 = label[j][i][0]; }	//��߽�
			if (t4 < label[j][i][0]) { t4 = label[j][i][0]; }	//�ұ߽�
		}
		//���ݳ���ɸѡ���ɽ���Ĺ�Դ,num��ʾ�ɽ����Դ����Ŀ
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
	//ȡ��������se�����Ľ���Ϊ��Դ����
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