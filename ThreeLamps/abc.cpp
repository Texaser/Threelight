// abc.cpp : 定义控制台应用程序的入口点。
//
//

#include "stdafx.h"

#include "PNPSolver.h"

#include "abc.h"
/*************相机物理参数***********/
//内参数
#define f 2750
double fx = 61*f/200;//3.3;//6800.7;
double fy = 362*f/1200;/*11600*/;//4;//6798.1;
double u0 = 768;/*0*/;//3065.8;
double v0 = 512;/*0*/;//1667.6;
//镜头畸变参数
double k1 = 0;//-0.189314;
double k2 = 0;//0.444657;
double p1 = 0;//-0.00116176;
double p2 = 0;//0.00164877;
double k3 = 0;//-2.57547;
//每盏灯对应的世界坐标
double x_1, x_2, x_3, x_4, y_1, y_2, y_3, y_4;
double x_w, y_w;
//第二个视频 9号灯在7号灯的位置 8510->3990
double a[13][2] = { {100, 600}, {100, 2860}, {100, 5720}, {100, 7380},{2000, -530}, {2000, 1730}, {2000, 3990}, {2000, 6250}, {2000, 8510}, 
{3900, 600}, {3900, 2860}, {3900, 5120}, {3900, 7380}};
//在每一盏灯的中心周围找两个随机点,通过调整alpha1和alpha2的值
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
//灯的像素坐标
double x_pixel, y_pixel;
double x, y;

//灯的id数组
//int id[4] = {0, 1, 2, 3};

int Location(int u[5],int v[5],unsigned char id[5],int *X,int *Y,int *Z,int lampnum)
{
//初始化PNPSolver类
PNPSolver p4psolver;
//初始化相机参数
p4psolver.SetCameraMatrix(fx, fy, u0, v0);
//设置畸变参数
p4psolver.SetDistortionCoefficients(k1, k2, p1, p2, k3);
//获取灯的id
//for(int i=0;i<=3;i++){
//	id[i] = 
//读入灯的id，得到灯的坐标
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


 //设置特征点的世界坐标
//p4psolver.Points3D.push_back(cv::Point3f(100, 5120, 0));    //P1三维坐标的单位是毫米
//p4psolver.Points3D.push_back(cv::Point3f(100, 2860, 0));  //P2
//p4psolver.Points3D.push_back(cv::Point3f(2000, 3990, 0));  //P3
//p4psolver.Points3D.push_back(cv::Point3f(3900, 5120, 0));   //P4
//p4psolver.Points3D.push_back(cv::Point3f(3900, 2860, 0));//P5

//p4psolver.Points3D.push_back(cv::Point3f(x_1, y_1, 0));    //P1三维坐标的单位是毫米
//p4psolver.Points3D.push_back(cv::Point3f(x_2, y_2, 0));  //P2
//p4psolver.Points3D.push_back(cv::Point3f(x_3, y_3, 0));  //P3
//p4psolver.Points3D.push_back(cv::Point3f(x_4, y_4, 0));   //P4
//p4psolver.Points3D.push_back(cv::Point3f(x_5, y_5, 0));//P5

cout <<"test2:特征点世界坐标 = " << endl << p4psolver.Points3D << endl;
 //设置特征点的图像坐标

//读入点的像素坐标

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

cout <<"test2:图中特征点坐标 = " << endl << p4psolver.Points2D << endl;
//cout<< (int)id[0]<< '\t' << (int)id[1] << '\t' << (int)id[2] << '\t' <<(int)id[3]<<'\t'<<(int)id[4]<<endl;
//if (p4psolver.Solve(PNPSolver::METHOD::CV_P3P) == 0)
//    cout <<"test2:CV_P3P方法:  相机位姿→" <<"Oc坐标=" << p4psolver.Position_OcInW <<"    相机旋转=" << p4psolver.Theta_W2C << endl;
//if (p4psolver.Solve(PNPSolver::METHOD::CV_ITERATIVE) == 0)
//    cout <<"test2:CV_ITERATIVE方法:    相机位姿→" <<"Oc坐标=" << p4psolver.Position_OcInW <<"    相机旋转=" << p4psolver.Theta_W2C << endl;
if (p4psolver.Solve(PNPSolver::METHOD::CV_EPNP) == 0)
    cout <<"test2:CV_EPNP方法: 相机位姿→" <<"Oc坐标=" << p4psolver.Position_OcInW <<"    相机旋转=" << p4psolver.Theta_W2C << endl;
X[0] = (int)p4psolver.Position_OcInW.x;
Y[0] = (int)p4psolver.Position_OcInW.y; 
Z[0] = (int)p4psolver.Position_OcInW.z;
//system("pause");
return 0;
}




