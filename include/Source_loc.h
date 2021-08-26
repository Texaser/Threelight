#pragma once
#include"stdio.h"
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include"BWLabel.h"
using namespace cv;
void imclose();
//void Source_loc(int gray[1024][1536]);
void Source_loc(Mat gray_image);
typedef struct
{
	int light_num;
	int light_loc[10][2];
	int light_range[10][4];  //иообвСср
}pic_divide;
