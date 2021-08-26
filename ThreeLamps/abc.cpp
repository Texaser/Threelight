// abc.cpp : �������̨Ӧ�ó������ڵ㡣
//
//

#include "stdafx.h"

#include "PNPSolver.h"

#include "abc.h"
/*************����������***********/
//�ڲ���
#define f 2750
double fx = 61*f/200;//3.3;//6800.7;
double fy = 362*f/1200;/*11600*/;//4;//6798.1;
double u0 = 768;/*0*/;//3065.8;
double v0 = 512;/*0*/;//1667.6;
//��ͷ�������
double k1 = 0;//-0.189314;
double k2 = 0;//0.444657;
double p1 = 0;//-0.00116176;
double p2 = 0;//0.00164877;
double k3 = 0;//-2.57547;
//ÿյ�ƶ�Ӧ����������
double x_1, x_2, x_3, x_4, y_1, y_2, y_3, y_4;
double x_w, y_w;
//�ڶ�����Ƶ 9�ŵ���7�ŵƵ�λ�� 8510->3990
double a[13][2] = { {100, 600}, {100, 2860}, {100, 5720}, {100, 7380},{2000, -530}, {2000, 1730}, {2000, 3990}, {2000, 6250}, {2000, 8510}, 
{3900, 600}, {3900, 2860}, {3900, 5120}, {3900, 7380}};
//��ÿһյ�Ƶ�������Χ�����������,ͨ������alpha1��alpha2��ֵ
double a_1[13][2] = { {100, 600}, {100, 2860}, {100, 5720}, {100, 7380},{2000, -530}, {2000, 1730}, {2000, 3990}, {2000, 6250}, {2000, 8510}, 
{3900, 600}, {3900, 2860}, {3900, 5120}, {3900, 7380}};
double a_2[13][2] = { {100, 600}, {100, 2860}, {100, 5720}, {100, 7380},{2000, -530}, {2000, 1730}, {2000, 3990}, {2000, 6250}, {2000, 8510}, 
{3900, 600}, {3900, 2860}, {3900, 5120}, {3900, 7380}};
double a_3[13][2] = { {100, 600}, {100, 2860}, {100, 5720}, {100, 7380},{2000, -530}, {2000, 1730}, {2000, 3990}, {2000, 6250}, {2000, 8510}, 
{3900, 600}, {3900, 2860}, {3900, 5120}, {3900, 7380}};
double a_4[13][2] = { {100, 600}, {100, 2860}, {100, 5720}, {100, 7380},{2000, -530}, {2000, 1730}, {2000, 3990}, {2000, 6250}, {2000, 8510}, 
{3900, 600}, {3900, 2860}, {3900, 5120}, {3900, 7380}};
//double alpha1 = 0.6, alpha2 = 0.8;
double alpha1 = 0.6, alpha2 = 0.8;
//double b[4][2] = {};
//p4psolver.Points2D.push_back(cv::Point2f(297.5, 165.5)); //P1
//p4psolver.Points2D.push_back(cv::Point2f(333.5, 828)); //P2
//p4psolver.Points2D.push_back(cv::Point2f(897.5, 463.5)); //P3
//p4psolver.Points2D.push_back(cv::Point2f(1465, 130.5));    //P4
////p4psolver.Points2D.push_back(cv::Point2f(4148, 673));  //P5
//p4psolver.Points2D.push_back(cv::Point2f(1492.5, 773));  //P5
//�Ƶ���������
double x_pixel, y_pixel;
double x, y;

//�Ƶ�id����
//int id[4] = {0, 1, 2, 3};

int Location(int u[5],int v[5],unsigned char id[5],int *X,int *Y,int *Z,int lampnum)
{
//��ʼ��PNPSolver��
PNPSolver p4psolver;
//��ʼ���������
p4psolver.SetCameraMatrix(fx, fy, u0, v0);
//���û������
p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);
//��ȡ�Ƶ�id
//for(int i=0;i<=3;i++){
//	id[i] = 
//����Ƶ�id���õ��Ƶ�����
for(int i=0;i<=12;i++){
	a_1[i][0] = a[i][0]+alpha1*100;
	a_1[i][1] = a[i][1]+alpha2*600;
	a_2[i][0] = a[i][0]-alpha1*100;
	a_2[i][1] = a[i][1]-alpha2*600;
	a_3[i][0] = a[i][0]-alpha1*120;
	a_3[i][1] = a[i][1]+alpha2*600;
	a_4[i][0] = a[i][0]+alpha1*100;
	a_4[i][1] = a[i][1]-alpha2*600;
}

//i<=lampnum-1
	for(int i=0;i<=4;i++){
	if(id[i]!=0){
	x_w = a_1[id[i]-1][0];
	y_w = a_1[id[i]-1][1];
	p4psolver.Points3D.push_back(cv::Point3f(x_w, y_w, 0));
	x_w = a_2[id[i]-1][0];
	y_w = a_2[id[i]-1][1];
	p4psolver.Points3D.push_back(cv::Point3f(x_w, y_w, 0));
	x_w = a_3[id[i]-1][0];
	y_w = a_3[id[i]-1][1];
	p4psolver.Points3D.push_back(cv::Point3f(x_w, y_w, 0));
	x_w = a_4[id[i]-1][0];
	y_w = a_4[id[i]-1][1];
	p4psolver.Points3D.push_back(cv::Point3f(x_w, y_w, 0));
	break;
}
}


//for(int i=0;i<=1;i++){
//	x_w = a_1[id[i]-1][0];
//	y_w = a_1[id[i]-1][1];
//	p4psolver.Points3D.push_back(cv::Point3f(x_w, y_w, 0));
//}
//
//
//for(int i=0;i<=1;i++){
//	x_w = a_2[id[i]-1][0];
//	y_w = a_2[id[i]-1][1];
//	p4psolver.Points3D.push_back(cv::Point3f(x_w, y_w, 0));
//}


 //�������������������
//p4psolver.Points3D.push_back(cv::Point3f(100, 5120, 0));    //P1��ά����ĵ�λ�Ǻ���
//p4psolver.Points3D.push_back(cv::Point3f(100, 2860, 0));  //P2
//p4psolver.Points3D.push_back(cv::Point3f(2000, 3990, 0));  //P3
//p4psolver.Points3D.push_back(cv::Point3f(3900, 5120, 0));   //P4
//p4psolver.Points3D.push_back(cv::Point3f(3900, 2860, 0));//P5

//p4psolver.Points3D.push_back(cv::Point3f(x_1, y_1, 0));    //P1��ά����ĵ�λ�Ǻ���
//p4psolver.Points3D.push_back(cv::Point3f(x_2, y_2, 0));  //P2
//p4psolver.Points3D.push_back(cv::Point3f(x_3, y_3, 0));  //P3
//p4psolver.Points3D.push_back(cv::Point3f(x_4, y_4, 0));   //P4
//p4psolver.Points3D.push_back(cv::Point3f(x_5, y_5, 0));//P5

cout <<"test2:�������������� = " << endl << p4psolver.Points3D << endl;
 //�����������ͼ������

//��������������

//i<=lampnum-1

for(int i=0;i<=4;i++){
	if(id[i]!=0){
	x = u[i]+100*alpha1*fx/f;
	y = v[i]+600*alpha2*fy/f;
	p4psolver.Points2D.push_back(cv::Point2f(x, y));
	x = u[i]-100*alpha1*fx/f;
	y = v[i]-600*alpha2*fy/f;
	p4psolver.Points2D.push_back(cv::Point2f(x, y));
	x = u[i]-120*alpha1*fx/f;
	y = v[i]+600*alpha2*fy/f;
	p4psolver.Points2D.push_back(cv::Point2f(x, y));
	x = u[i]+100*alpha1*fx/f;
	y = v[i]-600*alpha2*fy/f;
	p4psolver.Points2D.push_back(cv::Point2f(x, y));
	break;
	}
}


//for(int i=0;i<=1;i++){
//	x = u[i]+100*alpha1*fx/f;
//	y = v[i]+600*alpha2*fy/f;
//	p4psolver.Points2D.push_back(cv::Point2f(x, y));
//}
//
//for(int i=0;i<=1;i++){
//	x = u[i]-100*alpha1*fx/f;
//	y = v[i]-600*alpha2*fy/f;
//	p4psolver.Points2D.push_back(cv::Point2f(x, y));
//}

//p4psolver.Points2D.push_back(cv::Point2f(297.5, 165.5)); //P1
//p4psolver.Points2D.push_back(cv::Point2f(333.5, 828)); //P2
//p4psolver.Points2D.push_back(cv::Point2f(897.5, 463.5)); //P3
//p4psolver.Points2D.push_back(cv::Point2f(1465, 130.5));    //P4
////p4psolver.Points2D.push_back(cv::Point2f(4148, 673));  //P5
//p4psolver.Points2D.push_back(cv::Point2f(1492.5, 773));  //P5

cout <<"test2:ͼ������������ = " << endl << p4psolver.Points2D << endl;
//cout<< (int)id[0]<< '\t' << (int)id[1] << '\t' << (int)id[2] << '\t' <<(int)id[3]<<'\t'<<(int)id[4]<<endl;
//if (p4psolver.Solve(PNPSolver::METHOD::CV_P3P) == 0)
//    cout <<"test2:CV_P3P����:  ���λ�ˡ�" <<"Oc����=" << p4psolver.Position_OcInW <<"    �����ת=" << p4psolver.Theta_W2C << endl;
//if (p4psolver.Solve(PNPSolver::METHOD::CV_ITERATIVE) == 0)
//    cout <<"test2:CV_ITERATIVE����:    ���λ�ˡ�" <<"Oc����=" << p4psolver.Position_OcInW <<"    �����ת=" << p4psolver.Theta_W2C << endl;
if (p4psolver.Solve(PNPSolver::METHOD::CV_EPNP) == 0)
    cout <<"test2:CV_EPNP����: ���λ�ˡ�" <<"Oc����=" << p4psolver.Position_OcInW <<"    �����ת=" << p4psolver.Theta_W2C << endl;
X[0] = (int)p4psolver.Position_OcInW.x;
Y[0] = (int)p4psolver.Position_OcInW.y; 
Z[0] = (int)p4psolver.Position_OcInW.z;
//system("pause");
return 0;
}




