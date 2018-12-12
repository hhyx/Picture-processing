#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
using namespace cv;
using namespace std;

int main () {
	Mat apple = imread("redapple.jpg");

	int col, row;
    col=apple.cols;
	row=apple.rows;

	int r[col*row];
	int g[col*row];
	int b[col*row];

	for (int i = 0; i < row; i++) {
		const uchar*data = apple.ptr<uchar>(i);
		for (int j = 0; j < col; j++) {
			r[i*col+j] = data[3*j];
			g[i*col+j] = data[3*j+1];
			b[i*col+j] = data[3*j+2];
		}
	}
	sort(r, r+col*row);
	sort(g, g+col*row);
	sort(b, b+col*row);

	int r_min = r[0];
	int r_max = r[col*row-1];
	int g_min = g[0];
	int g_max = g[col*row-1];
	int b_min = b[0];
	int b_max = b[col*row-1];

	int res[257][12];
	int tem[257][12];
	memset(res, 0, 257*12*4);
	memset(tem, 0, 257*12*4);

	res[0][0] = r_min;
	res[0][1] = r_max;
	res[0][2] = g_min;
	res[0][3] = g_max;
	res[0][4] = b_min;
	res[0][5] = b_max;
	res[0][6] = 0;
	res[0][7] = col*row-1;
	res[0][8] = 0;
	res[0][9] = col*row-1;
	res[0][10] = 0;
	res[0][11] = col*row-1;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < (int)pow(2.0, i); j++) {
			int red = res[j][1]-res[j][0];
			int green = res[j][3]-res[j][2];
			int blue = res[j][5]-res[j][4];
			int max = (red>=green) ? red : green;
			max = (max>=blue) ? max : blue;

			memcpy(tem[2*j], res[j], 12*4);
			memcpy(tem[2*j+1], res[j], 12*4);

			if (max == red) {
				int new_red = r[res[j][6] + (res[j][7]-res[j][6])/2];
				tem[2*j][1] = new_red;
				tem[2*j][7] = res[j][6] + (res[j][7]-res[j][6])/2;
				tem[2*j+1][0] = new_red+1;
				tem[2*j+1][6] = res[j][6] + (res[j][7]-res[j][6])/2+1;
			} else if (max == green) {
				int new_green = g[res[j][8] + (res[j][9]-res[j][8])/2];
				tem[2*j][3] = new_green;
				tem[2*j][9] = res[j][8] + (res[j][9]-res[j][8])/2;
				tem[2*j+1][2] = new_green+1;
				tem[2*j+1][8] = res[j][8] + (res[j][9]-res[j][8])/2+1;
			} else {
				int new_blue = b[res[j][10] + (res[j][11]-res[j][10])/2];
				tem[2*j][5] = new_blue;
				tem[2*j][11] = res[j][10] + (res[j][11]-res[j][10])/2;
				tem[2*j+1][4] = new_blue+1;
				tem[2*j+1][10] = res[j][10] + (res[j][11]-res[j][10])/2+1;
			}

		}
		for (int k = 0; k < (int)pow(2.0, i+1); k++)
			memcpy(res[k], tem[k], 12*4);
	
	}

	Mat test(row, col, CV_8UC3);
	for (int i = 0; i < row; i++) {
		const uchar*inData = apple.ptr<uchar>(i);
		uchar*outDate = test.ptr<uchar>(i);

		for (int j = 0; j < col; j++) {

			for (int k = 0; k < 256; k++) {

				if (inData[3*j] >= res[k][0] && inData[3*j] <= res[k][1] 
					&& inData[3*j+1] >= res[k][2] && inData[3*j+1] <= res[k][3] 
					&& inData[3*j+2] >= res[k][4] && inData[3*j+2] <= res[k][5] ) {
								
					outDate[3*j] = (res[k][0] + res[k][1]) / 2;
					outDate[3*j+1] = (res[k][2] + res[k][3]) / 2;
					outDate[3*j+2] = (res[k][4] + res[k][5]) / 2;	

					break;
				}
			}
		}
	}

    namedWindow("test");
    imshow("test", test);
    namedWindow("apple");
    imshow("apple", apple);
    waitKey(0);
	
	return 0;
}