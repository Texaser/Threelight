void Otsu2Threshold(Mat& src, Mat& dst)
{
	Mat img = src;
	//dst.create(src.size(), src.type());
	CV_Assert(1 == img.channels());

	int height = img.rows;
	int width  = img.cols;
	int Threshold1 = 0;//��ֵ
	int Threshold2 = 0;//��ֵ
	double gSum_object;//��һ��,ǰ���Ҷ���ֵ
	double gSum_middle;
	double gSum_blackground;//�ڶ��࣬�����Ҷ���ֵ

	double N_object = 0;//ǰ��������
	double N_middle = 0;
	double N_blackground = 0;//����������

	double u0 = 0;//ǰ������ƽ���Ҷ�
	double u1 = 0;
	double u2 = 0;//��������ƽ���Ҷ�
	double w0 = 0;//ǰ�����ص���ռ����ͼ��ı���Ϊ��0
	double w1 = 0;
	double w2 = 0;//�������ص���ռ����ͼ��ı���Ϊ��1
	double u = 0;//��ƽ���Ҷ�
	double tempg = -1;//��ʱ��䷽��
	double g = -1;//��䷽��
	double Histogram[256]={0};//�Ҷ�ֱ��ͼ
	double N_allpixel = width * height;//��������
	for(int k = 0; k < height; k++)
	{
		//����ֱ��ͼ
		uchar* data = img.ptr<uchar>(k);
		for(int n = 0; n < width; n++)
		{
			Histogram[data[n]]++;
		} 
	}

	// 	Mat Hist = Mat::zeros(256, 256, CV_8UC1);
	// 	//double *tmpHistogram = Histogram;
	// 	for(int k = 0; k < 256; k++)
	// 	{
	// 		//����ֱ��ͼ
	// 		uchar* data = Hist.ptr<uchar>(k);
	// 		Histogram[k] = Histogram[k]/2773*255;
	// 		for(int n = 0; n < (int)Histogram[k]; n++)
	// 		{
	// 			data[n] = 255;
	// 		} 
	// 	}
	// 	imshow("hist", Hist);



	//������ֵ
	int tmpBlack = 0;
	int tmpMiddle = 0;
	for (int i = 0; i < 256; i++)
	{
		N_object += Histogram[i];
		N_blackground = 0;
		for (int k = i+10; k < 256; k++)
		{
			N_blackground += Histogram[k];
		}
		//if(0 == N_object) continue;//û��ǰ������ѭ��
		for (int j = i+10; j < 256; j++)
		{
			gSum_object = 0;
			gSum_middle = 0;
			gSum_blackground = 0;

			N_middle = N_allpixel - N_object - N_blackground;
			w0 = N_object / N_allpixel;
			w2 = N_blackground / N_allpixel;
			w1 = 1 - w0 - w2;

			//��u0
			for (int k = 0; k <= i; k++)
			{
				gSum_object += k*Histogram[k];
			}
			u0 = gSum_object / N_object;

			//��u1
			for (int k = i+1; k <= j; k++)
			{
				gSum_middle += k*Histogram[k] ;
			}
			u1 = gSum_middle / N_middle;

			//��u2
			for(int k = j+1; k < 256; k++)
			{
				gSum_blackground += k*Histogram[k];
			}
			u2 = gSum_blackground / N_blackground;

			u = w0*u0 + w1*u1 + w2*u2;
			g = w0*(u - u0)*(u - u0) + w1*(u - u1)*(u - u1) + w2*(u - u2)*(u - u2); 

			if (tempg < g)//ͨ����䷽�����޸���ֵ
			{
				tempg = g;
				Threshold1 = (int)i;
				Threshold2 = (int)j;
				tmpBlack = N_object;
				tmpMiddle = N_middle;
			}

			N_blackground -= Histogram[j];

		}
	}

	int MiddlePixel = 255;
	if (tmpBlack > tmpMiddle)
	{
		MiddlePixel = 0;
	}


	for (int y=0; y < height; y++)
	{ 
		uchar* dstdata = dst.ptr<uchar>(y);
		uchar* srcdata = src.ptr<uchar>(y);
		for (int x=0; x < width; x++)
		{ 

			if ((int)srcdata[x] > Threshold2) 
				dstdata[x] = 255; 
			if ((int)srcdata[x] > Threshold1 && (int)srcdata[x] <= Threshold2) 
				dstdata[x] = MiddlePixel;
			if ((int)srcdata[x] <= Threshold1) 
				dstdata[x] = 0;
		} 
	}


	//��ɫ������������40%����Ϊ����Ϊ�ף��滻Ϊ�ڱ���
	int BlackgroundNum = 0;
	int AllNum = dst.rows * dst.cols;
	for (int i = 0; i < dst.rows; i++)
	{
		uchar* data = dst.ptr<uchar>(i);
		for (int j = 0; j < dst.cols; j++)
		{
			if (data[j] == 0)
			{
				BlackgroundNum++;
			}
		}
	}

	float BlackgroundRatio = 0;
	BlackgroundRatio = (float)BlackgroundNum / (float)AllNum;
	if (BlackgroundRatio < 0.4)
	{
		dst = 255 - dst;
	}
}