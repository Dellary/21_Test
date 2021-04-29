#include "stdafx.h"

#include "cv.h"

#include "highgui.h"

#include "iostream"

using namespace std;



bool key = false;



int _tmain(int argc, _TCHAR* argv[])

{

	IplImage* src = cvLoadImage("c:/img1.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	if (!src)

	{

		cout << "src load error..." << endl;

		system("pause");

		exit(-1);

	}



	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);

	IplImage* color_dst = cvCreateImage(cvGetSize(src), 8, 3);



	//cvCanny( src, dst, 50, 200, 3 );

	cvThreshold(src, dst, 50, 255, CV_THRESH_BINARY_INV);

	cvCvtColor(dst, color_dst, CV_GRAY2BGR);



	CvMemStorage* storage = cvCreateMemStorage(0);

	CvSeq* lines = 0;

	int i;



	if (key)

	{

		lines = cvHoughLines2(dst, storage, CV_HOUGH_STANDARD, 1, CV_PI / 180, 100, 0, 0);



		for (i = 0; i < MIN(lines->total, 100); i++)

		{

			float* line = (float*)cvGetSeqElem(lines, i);

			float rho = line[0];

			float theta = line[1];

			CvPoint pt1, pt2;

			double a = cos(theta), b = sin(theta);

			double x0 = a * rho, y0 = b * rho;

			pt1.x = cvRound(x0 + 1000 * (-b));

			pt1.y = cvRound(y0 + 1000 * (a));

			pt2.x = cvRound(x0 - 1000 * (-b));

			pt2.y = cvRound(y0 - 1000 * (a));

			cvLine(color_dst, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0);

		}

	}

	else

	{

		lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);

		for (i = 0; i < lines->total; i++)

		{

			CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);

			cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, CV_AA, 0);

		}

	}





	cvNamedWindow("Source", 1);

	cvShowImage("Source", src);



	cvNamedWindow("Hough", 1);

	cvShowImage("Hough", color_dst);

	cvSaveImage("c:/result.bmp", color_dst);



	cvWaitKey(0);

	cvReleaseImage(&src);

	cvReleaseImage(&dst);

	cvReleaseImage(&color_dst);

	cvClearSeq(lines);

	cvReleaseMemStorage(&storage);





	system("pause");

	return 0;

}