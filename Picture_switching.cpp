#include <opencv2/opencv.hpp>  
#include <iostream>
#include <string>
#include <math.h>
using namespace cv;
using namespace std;

int main()
{
    Mat lena = imread("lena.jpg");
    Mat nuobeier = imread("诺贝尔.jpg");

    int col, row;
    col=lena.cols;
	row=lena.rows;

	Mat test(row, col, CV_8UC1);

	float r;
	float r_max = sqrt((float)(row/2)*(row/2) + (col/2)*(col/2));
	float t = 0;
	float t_max = 10;

	for (t = 0; t <= t_max; t++) {
		String str;
		stringstream ss;
		ss << t; 
		str = ss.str();
		for (int i = 0; i < row; i++) {
			
			const uchar*inData1 = lena.ptr<uchar>(i);
			const uchar*inData2 = nuobeier.ptr<uchar>(i);
			uchar*outDate = test.ptr<uchar>(i);

			for (int j = 0; j < col; j++) {
				r = sqrt((float)(i-row/2)*(i-row/2) + (j-col/2)*(j-col/2));
				if (r < r_max*t/t_max) {
					outDate[j] = inData1[3*j];
				} else {
					outDate[j] = inData2[3*j];
				}
			}
		}
	    namedWindow("test");
	    imshow("test", test);

	    imwrite( str+".jpg", test);
	    waitKey(0);
	}

    waitKey(0);
    return 0;
}