#include "CImg.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
using namespace cimg_library;
using namespace std;
#define N 8
const double PI = acos(-1);

// 亮度
int brightness[N][N]={
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99}
};

// 色度
int chroma[N][N]={
	{17,18,24,47,99,99,99,99},
	{18,21,26,66,99,99,99,99},
	{24,26,56,99,99,99,99,99},
	{47,66,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99},
	{99,99,99,99,99,99,99,99}
};

// Z
int Z[N][N] = {
	0, 1, 5, 6, 14, 15, 27, 28,
	2, 4, 7, 13, 16, 26, 29, 42,
	3, 8, 12, 17, 25, 30, 41, 43, 
	9, 11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54, 
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63
};

// 亮度霍夫曼编码表 DC
string DC_Brightness_Hoffman [12] = {
	"00",
	"010",
	"011",	
	"100",
	"101",
	"110",
	"1110",
	"11110",
	"111110",
	"1111110",
	"11111110",
	"111111110"
};

// 色度霍夫曼编码表 DC
string DC_Chroma_Hoffman [12] = {
	"00",
	"01",
	"10",
	"110",
	"1110",
	"11110",
	"111110",
	"1111110",
	"11111110",
	"111111110",
	"1111111110",
	"11111111110"
};

// 亮度霍夫曼编码表 AC
string AC_Brightness_Hoffman [161] = {
	"1010", "00", "01", "100", "1011" ,"11010" ,"1111000" ,"11111000" ,"1111110110" ,"1111111110000010" ,"1111111110000011",
	"1100" ,"11011" ,"1111001" ,"111110110" ,"11111110110" ,"1111111110000100" ,"1111111110000101" ,"1111111110000110" ,"1111111110000111" ,"1111111110001000",
	"11100", "11111001", "1111110111", "111111110100", "1111111110001001", "1111111110001010", "1111111110001011", "1111111110001100", "1111111110001101", "1111111110001110",
	"111010", "111110111", "111111110101", "1111111110001111", "1111111110010000", "1111111110010001", "1111111110010010", "1111111110010011", "1111111110010100", "1111111110010101",
	"111011", "1111111000", "1111111110010110", "1111111110010111", "1111111110011000", "1111111110011001", "1111111110011010", "1111111110011011", "1111111110011100", "1111111110011101",
	"1111010", "11111110111", "1111111110011110", "1111111110011111", "1111111110100000", "1111111110100001", "1111111110100010", "1111111110100011", "1111111110100100", "1111111110100101",
	"1111011", "111111110110", "1111111110100110", "1111111110100111", "1111111110101000", "1111111110101001", "1111111110101010", "1111111110101011", "1111111110101100", "1111111110101101",
	"11111010", "111111110111", "1111111110101110", "1111111110101111", "1111111110110000", "1111111110110001", "1111111110110010", "1111111110110011", "1111111110110100", "1111111110110101",
	"111111000", "111111111000000", "1111111110110110", "1111111110110111", "1111111110111000", "1111111110111001", "1111111110111010", "1111111110111011", "1111111110111100", "1111111110111101",
	"111111001", "1111111110111110", "1111111110111111", "1111111111000000", "1111111111000001", "1111111111000010", "1111111111000011", "1111111111000100", "1111111111000101", "1111111111000110",
	"111111010", "1111111111000111", "1111111111001000", "1111111111001001", "1111111111001010", "1111111111001011", "1111111111001100", "1111111111001101", "1111111111001110", "1111111111001111",
	"1111111001", "1111111111010000", "1111111111010001", "1111111111010010", "1111111111010011", "1111111111010100", "1111111111010101", "1111111111010110", "1111111111010111", "1111111111011000",
	"1111111010", "1111111111011001", "1111111111011010", "1111111111011011", "1111111111011100", "1111111111011101", "1111111111011110", "1111111111011111", "1111111111100000", "1111111111100001",
	"11111111000", "1111111111100010", "1111111111100011", "1111111111100100", "1111111111100101", "1111111111100110", "1111111111100111", "1111111111101000", "1111111111101001", "1111111111101010",
	"1111111111101011", "1111111111101100", "1111111111101101", "1111111111101110", "1111111111101111", "1111111111110000", "1111111111110001", "1111111111110010", "1111111111110011", "1111111111110100",
	"1111111111110101", "1111111111110110", "1111111111110111", "1111111111111000", "1111111111111001", "1111111111111010", "1111111111111011", "1111111111111100", "1111111111111101", "1111111111111110"
};

// 色度霍夫曼编码表 AC
string AC_Chroma_Hoffman [161] = {
	"00", "01", "100", "1010", "11000", "11001", "111000", "1111000", "111110100", "1111110110", "111111110100",
	"1011", "111001", "11110110", "111110101", "11111110110", "111111110101", "1111111110001000", "1111111110001001", "1111111110001010", "1111111110001011",
	"11010", "11110111", "1111110111", "111111110110", "111111111000010", "1111111110001100", "1111111110001101", "1111111110001110", "1111111110001111", "1111111110010000",
	"11011", "11111000", "1111111000", "111111110111", "1111111110010001", "1111111110010010", "1111111110010011", "1111111110010100", "1111111110010101", "1111111110010110",
	"111010", "111110110", "1111111110010111", "1111111110011000", "1111111110011001", "1111111110011010", "1111111110011011", "1111111110011100", "1111111110011101", "1111111110011110",
	"111011", "1111111001", "1111111110011111", "1111111110100000", "1111111110100001", "1111111110100010", "1111111110100011", "1111111110100100", "1111111110100101", "1111111110100110",
	"1111001", "11111110111", "1111111110100111", "1111111110101000", "1111111110101001", "1111111110101010", "1111111110101011", "1111111110101100", "1111111110101101", "1111111110101110",
	"1111010", "11111111000", "1111111110101111", "1111111110110000", "1111111110110001", "1111111110110010", "1111111110110011", "1111111110110100", "1111111110110101", "1111111110110110",
	"11111001", "1111111110110111", "1111111110111000", "1111111110111001", "1111111110111010", "1111111110111011", "1111111110111100", "1111111110111101", "1111111110111110", "1111111110111111",
	"111110111", "1111111111000000", "1111111111000001", "1111111111000010", "1111111111000011", "1111111111000100", "1111111111000101", "1111111111000110", "1111111111000111", "1111111111001000",
	"111111000", "1111111111001001", "1111111111001010", "1111111111001011", "1111111111001100", "1111111111001101", "1111111111001110", "1111111111001111", "1111111111010000", "1111111111010001",
	"111111001", "1111111111010010", "1111111111010011", "1111111111010100", "1111111111010101", "1111111111010110", "1111111111010111", "1111111111011000", "1111111111011001", "1111111111011010",
	"111111010", "1111111111011011", "1111111111011100", "1111111111011101", "1111111111011110", "1111111111011111", "1111111111100000", "1111111111100001", "1111111111100010", "1111111111100011",
	"11111111001", "1111111111100100", "1111111111100101", "1111111111100110", "1111111111100111", "1111111111101000", "1111111111101001", "1111111111101010", "1111111111101011", "1111111111101100",
	"11111111100000", "1111111111101101", "1111111111101110", "1111111111101111", "1111111111110000", "1111111111110001", "1111111111110010", "1111111111110011", "1111111111110100", "1111111111110101",
	"111111111000011", "1111111111110110", "1111111111110111", "1111111111111000", "1111111111111001", "1111111111111010", "1111111111111011", "1111111111111100", "1111111111111101", "1111111111111110"
}; //  注意 F/0 ZRL 删掉了

// VLI编码
string VLI(int num) {
	bool flag = true;
	num>0 ? flag  = true : flag = false;
	string res;
	num = abs(num);
	int i = 0;
	while (num > 0) {
		res = res + to_string(num % 2);
		num /= 2;
	}
	string s(res.rbegin(),res.rend());
	if (flag)
		return s;
	else {
		for (int i = 0; i < s.length(); i++) {
			if (s[i] == '1')	s[i] = '0';
			else 	s[i] = '1';
		}
		return s;
	}
}

// VLI逆编码
int NVLI(string str) {
	bool flag = true;
	if (str[0] == '0') {
		flag = false;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == '1')	str[i] = '0';
			else 	str[i] = '1';
		}
	}
	int num = 0, pow = 1;
	for (int i = str.length()-1; i >= 0; i--) {
		num += pow * (str[i]-'0');
		pow *= 2;
	}
	if (flag)
		return num;
	else 
		return 0-num;
}

// 查找hoffman表对应位置
int find(string hoffman[], string str, int size) {
	for (int i = 0; i < size; i++)
		if (hoffman[i] == str)
			return i;
}

// 色度二次采样
void Chroma_two_sampling(double array[][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			array[i][j] = array[i][j] - 128;
		}
	}
}

// DCT
void DCT(double sampling[][8], double F [][8]) {
	for(int u = 0; u < N; u++){
		for(int v = 0; v < N; v++){
			double temp = 0.0;
			for(int i = 0; i < N; i++){
				for(int j = 0; j < N; j++){
					temp = temp + sampling[i][j]*cos((2*i+1)*u*PI*1.0/16)*cos((2*j+1)*v*PI*1.0/16);
				}
			}
			F[u][v] = 1.0/4*(u==0?1.0/sqrt(2):1)*(v==0?1.0/sqrt(2):1)*temp;
		}
	}
}

// 亮度量化
void Brightness_quantization(double F [][8], double F_[][8]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F_[i][j] = round(F[i][j] / brightness[i][j]);
		}
	}
}

// 色度量化
void Chroma_quantization(double F [][8], double F_[][8]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F_[i][j] = round(F[i][j] / chroma[i][j]);
		}
	}
}

// 亮度量化还原
void Brightness_reduction(double F [][8]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = F[i][j] * brightness[i][j];
		}
	}
}

// 色度量化还原
void Chroma_reduction(double F [][8]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F[i][j] = F[i][j] * chroma[i][j];
		}
	}
}

// NDCT
void NDCT(double F [][8], double F_ [][8]) {
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			double sum = 0.0;
			for(int u = 0; u < N; u++){
				for(int v = 0; v < N; v++){
					sum = sum + (u==0?1.0/sqrt(2.0):1.0)*(v==0?1.0/sqrt(2.0):1.0)*F[u][v]*cos((2*i+1)*u*PI*1.0/16)*cos((2*j+1)*v*PI*1.0/16);
				}
			}
			F_[i][j] = 1.0/4*sum + 128;
		}
	}
}

// DC系数
int DC_coefficient(int temp){
	if(temp == 0){
		return 0;
	}else{
		for(int i = 1; i <= 11; i++)
			if(abs(temp)<pow(2,i))
				return i;
	}
}

// AC系数
int AC_coefficient(int temp){
	if(temp == 0){
		return 0;
	}else{
		for(int i=1;i<=10;i++)
			if(abs(temp)<pow(2,i))
				return i;
	}
}

//DPCM + 游长编码
void DCAC(double F_[][8], int last_DC, int change[][N], int DC_format[2], int AC_format[][3]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			change[ Z[i][j]/N ][ Z[i][j]%N ] = F_[i][j];
		}
	}
	DC_format[1] = change[0][0] - last_DC;
	DC_format[0] = DC_coefficient(DC_format[1]);
	last_DC = change[0][0];

	int count = 0, AC_num = 0;
	for (int i = 1; i < 64; i++) {
		if (change[i/N][i%N] == 0) {
			count ++;
		} else {
			AC_format[AC_num][0] = count;
			count = 0;
			AC_format[AC_num][2] = change[i/N][i%N];
			AC_format[AC_num][1] = AC_coefficient(change[i/N][i%N]);
			AC_num ++;
		}
	}
	AC_format[AC_num][0] = 100;
	AC_format[AC_num][1] = 100;
	AC_format[AC_num][2] = 100;
}

// 熵编码 亮度
void Brightness_entropy(string huffman_DC[2], string huffman_AC[][2], int DC_format[2], int AC_format[][3]) {
	huffman_DC[0] = DC_Brightness_Hoffman[DC_format[0]];
	huffman_DC[1] = VLI(DC_format[1]);

	for (int i = 0; i < N*N; i++) {
		if (AC_format[i][0] == 100) {
			huffman_AC[i][0] = AC_Brightness_Hoffman[0];
			 break;
		} else {
			huffman_AC[i][0] = AC_Brightness_Hoffman[AC_format[i][0]*10+AC_format[i][1]];
			huffman_AC[i][1] = VLI(AC_format[i][2]);
		}
	}
}

// 熵编码 色度
void Chroma_entropy(string huffman_DC[2], string huffman_AC[][2], int DC_format[2], int AC_format[][3]) {
	huffman_DC[0] = DC_Chroma_Hoffman[DC_format[0]];
	huffman_DC[1] = VLI(DC_format[1]);

	for (int i = 0; i < N*N; i++) {
		if (AC_format[i][0] == 100) {
			huffman_AC[i][0] = AC_Brightness_Hoffman[0];
			 break;
		} else {
			huffman_AC[i][0] = AC_Chroma_Hoffman[AC_format[i][0]*10+AC_format[i][1]];
			huffman_AC[i][1] = VLI(AC_format[i][2]);
		}
	}
}

// 熵编码解码 亮度
void Brightness_Nentropy(string huffman_DC[2], string huffman_AC[][2], int DC_format[2], int AC_format[][3]) {
	DC_format[0] = find(DC_Brightness_Hoffman, huffman_DC[0], sizeof(DC_Brightness_Hoffman)/sizeof(DC_Brightness_Hoffman[0]));
	DC_format[1] = NVLI(huffman_DC[1]);
	for (int i = 0; i < N*N; i++) {
		if (huffman_AC[i][0] == "1010") {
			AC_format[i][0] = 100;
			AC_format[i][1] = 100;
			AC_format[i][2] = 100;
			break;
		} else {
			int count = find(AC_Brightness_Hoffman, huffman_AC[i][0], sizeof(AC_Brightness_Hoffman)/sizeof(AC_Brightness_Hoffman[0]));
			AC_format[i][1] = count % 10;
			AC_format[i][0] = count / 10;
			AC_format[i][2] = NVLI(huffman_AC[i][1]);
		}
	}
}

// 熵编码解码 色度
void Chroma_Nentropy(string huffman_DC[2], string huffman_AC[][2], int DC_format[2], int AC_format[][3]) {
	DC_format[0] = find(DC_Chroma_Hoffman, huffman_DC[0], sizeof(DC_Chroma_Hoffman)/sizeof(DC_Chroma_Hoffman[0]));
	DC_format[1] = NVLI(huffman_DC[1]);
	for (int i = 0; i < N*N; i++) {
		if (huffman_AC[i][0] == "1010") {
			AC_format[i][0] = 100;
			AC_format[i][1] = 100;
			AC_format[i][2] = 100;
			break;
		} else {
			int count = find(AC_Chroma_Hoffman, huffman_AC[i][0], sizeof(AC_Chroma_Hoffman)/sizeof(AC_Chroma_Hoffman[0]));
			AC_format[i][1] = count % 10;
			AC_format[i][0] = count / 10;
			AC_format[i][2] = NVLI(huffman_AC[i][1]);
		}
	}
}

// 解DPCM + 游长编码
void NDCAC(double F_[][8], int last_DC, int change[][N], int DC_format[2], int AC_format[][3]) {
	int AC_num = 0;
	change[0][0] = DC_format[1] + last_DC;
	for (int i = 1; i < N*N; i++) {
		if (AC_format[AC_num][0] > 0) {
			AC_format[AC_num][0] --;
			change[i/N][i%N] = 0;
		} else {
			change[i/N][i%N] = AC_format[AC_num][2];
			AC_num ++;
		}
	} 
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			F_[i][j] = change[ Z[i][j]/N ][ Z[i][j]%N ];
		}
	}
}


int main() {
	CImg<int> animal2("动物照片.bmp");//"动物照片.bmp"    "动物卡通图片.bmp"

	int col, row, col_, row_;
    col = col_ = animal2._height;
	row = row_ = animal2._width;

	int col_add = N - (col % N);
	int row_add = N - (row % N);
	if (col_add != N) col_ += col_add;
	if (row_add != N) row_ += row_add;
	CImg<float> test3(row_, col_, 1, 3);

	int r[N][N];
	int g[N][N];
	int b[N][N];

	double Y[N][N];
	double Cb[N][N];
	double Cr[N][N];

	double Y_F[N][N];
	double Y_F_[N][N];
	double Cb_F[N][N];
	double Cb_F_[N][N];
	double Cr_F[N][N];
	double Cr_F_[N][N];

	int last_DC_Y = 0, last_DC_Cb = 0, last_DC_Cr = 0;

	int change[N][N];
	int DC_format[2];
	int AC_format[N*N][3];

	string *huffman_DC_Y = new string [(row_/8) * (col_/8) * 2];
	string **huffman_AC_Y = new string *[row_*col_*N*2];
	for (int i = 0; i < row_*col_*N*2; i++) 
		huffman_AC_Y[i] = new string [2];

	string *huffman_DC_Cb = new string [(row_/8) * (col_/8) * 2];
	string **huffman_AC_Cb = new string *[row_*col_*N*2];
	for (int i = 0; i < row_*col_*N*2; i++) 
		huffman_AC_Cb[i] = new string [2];

	string *huffman_DC_Cr = new string [(row_/8) * (col_/8) * 2];
	string **huffman_AC_Cr = new string *[row_*col_*N*2];
	for (int i = 0; i < row_*col_*N*2; i++) 
		huffman_AC_Cr[i] = new string [2];



	for (int i = 0; i < row_; i+=N) {
		for (int j = 0; j < col_; j+=N) {

			memset(r, 0, sizeof(r));
			memset(g, 0, sizeof(g));
			memset(b, 0, sizeof(b));
			memset(Y, 0, sizeof(Y));
			memset(Cb, 0, sizeof(Cb));
			memset(Cr, 0, sizeof(Cr));
			memset(Y_F, 0, sizeof(Y_F));
			memset(Y_F_, 0, sizeof(Y_F_));
			memset(Cb_F, 0, sizeof(Cb_F));
			memset(Cb_F_, 0, sizeof(Cb_F_));
			memset(Cr_F, 0, sizeof(Cr_F));
			memset(Cr_F_, 0, sizeof(Cr_F_));

			for (int ii = 0; ii < N; ii++) {
				for (int jj = 0; jj < N; jj++) {
					//颜色读取
					if (j+jj < col && i+ii < row) { 
						r[ii][jj] = animal2(i + ii, j + jj, 0);
						g[ii][jj] = animal2(i + ii, j + jj, 1);
						b[ii][jj] = animal2(i + ii, j + jj, 2);
					} else {
						r[ii][jj] = 255;
						g[ii][jj] = 255;
						b[ii][jj] = 255;
					}

					//颜色转换
					Y[ii][jj] = 0.299*r[ii][jj] + 0.587*g[ii][jj] + 0.114*b[ii][jj];
					Cb[ii][jj] = -0.1687*r[ii][jj] - 0.3313*g[ii][jj] + 0.5*b[ii][jj] + 128;
					Cr[ii][jj] = 0.5*r[ii][jj] - 0.418*g[ii][jj]-0.0813*b[ii][jj] + 128;	

				test3(i + ii,j +jj,0) = Y[ii][jj] + 1.402*(Cr[ii][jj] - 128);
					test3(i + ii, j + jj, 1) = Y[ii][jj] - 0.34414*(Cb[ii][jj] - 128) - 0.71414*(Cr[ii][jj] - 128);
					test3(i + ii, j + jj, 2) = Y[ii][jj] + 1.772*(Cb[ii][jj] - 128);

				}
			}

			string huffman_DC[2];
			string huffman_AC[N*N][2];

			Chroma_two_sampling(Y);
			DCT(Y, Y_F);
			Brightness_quantization(Y_F, Y_F_);
			
			memset(DC_format, 0, sizeof(DC_format));
			memset(AC_format, 0, sizeof(AC_format));
			DCAC(Y_F_, last_DC_Y, change, DC_format, AC_format);
			Brightness_entropy(huffman_DC, huffman_AC, DC_format, AC_format);

			huffman_DC_Y[i*col_/32 + j/4] = huffman_DC[0];
			huffman_DC_Y[i*col_/32 + j/4+1] = huffman_DC[1];

			for (int ii = 0; ii < N*2; ii ++) {
				huffman_AC_Y[i*col_ + j*2 +ii][0] = huffman_AC[ii][0];
				huffman_AC_Y[i*col_ + j*2 +ii][1] = huffman_AC[ii][1];
			}

			last_DC_Y = Y_F_[0][0];



			Chroma_two_sampling(Cb);
			DCT(Cb, Cb_F);
			Chroma_quantization(Cb_F, Cb_F_);

			memset(DC_format, 0, sizeof(DC_format));
			memset(AC_format, 0, sizeof(AC_format));
			DCAC(Cb_F_, last_DC_Cb, change, DC_format, AC_format);
			Chroma_entropy(huffman_DC, huffman_AC, DC_format, AC_format);

			huffman_DC_Cb[i*col_/32 + j/4] = huffman_DC[0];
			huffman_DC_Cb[i*col_/32 + j/4+1] = huffman_DC[1];

			for (int ii = 0; ii < N*2; ii ++) {
				huffman_AC_Cb[i*col_ + j*2 +ii][0] = huffman_AC[ii][0];
				huffman_AC_Cb[i*col_ + j*2 +ii][1] = huffman_AC[ii][1];
			}

			last_DC_Cb = Cb_F_[0][0];
	


			Chroma_two_sampling(Cr);
			DCT(Cr, Cr_F);
			Chroma_quantization(Cr_F, Cr_F_);

			memset(DC_format, 0, sizeof(DC_format));
			memset(AC_format, 0, sizeof(AC_format));
			DCAC(Cr_F_, last_DC_Cr, change, DC_format, AC_format);
			Chroma_entropy(huffman_DC, huffman_AC, DC_format, AC_format);

			huffman_DC_Cr[i*col_/32 + j/4] = huffman_DC[0];
			huffman_DC_Cr[i*col_/32 + j/4+1] = huffman_DC[1];

			for (int ii = 0; ii < N*2; ii ++) {
				huffman_AC_Cr[i*col_ + j*2 +ii][0] = huffman_AC[ii][0];
				huffman_AC_Cr[i*col_ + j*2 +ii][1] = huffman_AC[ii][1];
			}

			last_DC_Cr = Cr_F_[0][0];
		}
	}

	last_DC_Y = 0; last_DC_Cb = 0; last_DC_Cr = 0;
	

	for (int i = 0; i < row_; i+=N) {
		for (int j = 0; j < col_; j+=N) {

			memset(Y, 0, sizeof(Y));
			memset(Cb, 0, sizeof(Cb));
			memset(Cr, 0, sizeof(Cr));
			memset(Y_F, 0, sizeof(Y_F));
			memset(Y_F_, 0, sizeof(Y_F_));
			memset(Cb_F, 0, sizeof(Cb_F));
			memset(Cb_F_, 0, sizeof(Cb_F_));
			memset(Cr_F, 0, sizeof(Cr_F));
			memset(Cr_F_, 0, sizeof(Cr_F_));

			string huffman_AC[N*N][2];
			string huffman_DC[2];

			huffman_DC[0] = huffman_DC_Y[i*col_/32 + j/4];
			huffman_DC[1] = huffman_DC_Y[i*col_/32 + j/4+1];

			for (int ii = 0; ii < N*2; ii ++) {
				huffman_AC[ii][0] = huffman_AC_Y[i*col_ + j*2 +ii][0];
				huffman_AC[ii][1] = huffman_AC_Y[i*col_ + j*2 +ii][1];
			}
			Brightness_Nentropy(huffman_DC, huffman_AC, DC_format, AC_format);
			NDCAC(Y_F_, last_DC_Y, change, DC_format, AC_format);
			last_DC_Y = Y_F_[0][0];

			Brightness_reduction(Y_F_);
			NDCT(Y_F_, Y);


			huffman_DC[0] = huffman_DC_Cb[i*col_/32 + j/4];
			huffman_DC[1] = huffman_DC_Cb[i*col_/32 + j/4+1];

			for (int ii = 0; ii < N*2; ii ++) {
				huffman_AC[ii][0] = huffman_AC_Cb[i*col_ + j*2 +ii][0];
				huffman_AC[ii][1] = huffman_AC_Cb[i*col_ + j*2 +ii][1];
			}
			Chroma_Nentropy(huffman_DC, huffman_AC, DC_format, AC_format);
			NDCAC(Cb_F_, last_DC_Cb, change, DC_format, AC_format);
			last_DC_Cb = Cb_F_[0][0];

			Chroma_reduction(Cb_F_);
			NDCT(Cb_F_, Cb);


			huffman_DC[0] = huffman_DC_Cr[i*col_/32 + j/4];
			huffman_DC[1] = huffman_DC_Cr[i*col_/32 + j/4+1];

			for (int ii = 0; ii < N*2; ii ++) {
				huffman_AC[ii][0] = huffman_AC_Cr[i*col_ + j*2 +ii][0];
				huffman_AC[ii][1] = huffman_AC_Cr[i*col_ + j*2 +ii][1];
			}
			Chroma_Nentropy(huffman_DC, huffman_AC, DC_format, AC_format);
			NDCAC(Cr_F_, last_DC_Cr, change, DC_format, AC_format);
			last_DC_Cr = Cr_F_[0][0];

			Chroma_reduction(Cr_F_);
			NDCT(Cr_F_, Cr);

			for (int ii = 0; ii < N; ii++) {
				for (int jj = 0; jj < N; jj++) {
					test3(i + ii,j +jj,0) = Y[ii][jj] + 1.402*(Cr[ii][jj] - 128);
					test3(i + ii, j + jj, 1) = Y[ii][jj] - 0.34414*(Cb[ii][jj] - 128) - 0.71414*(Cr[ii][jj] - 128);
					test3(i + ii, j + jj, 2) = Y[ii][jj] + 1.772*(Cb[ii][jj] - 128);

					if (test3(i + ii, j + jj, 0) > 255)test3(i + ii, j + jj, 0) = 255;
					else if (test3(i + ii, j + jj, 0) < 0) test3(i + ii, j + jj, 0) = 0;
					if (test3(i + ii, j + jj, 1) > 255) test3(i + ii, j + jj, 1) = 255;
					else if (test3(i + ii, j + jj, 1) < 0) test3(i + ii, j + jj, 1) = 0;
					if (test3(i + ii, j + jj, 2) > 255) test3(i + ii, j + jj, 2) = 255;
					else if (test3(i + ii, j + jj, 2) < 0) test3(i + ii, j + jj, 2) = 0;
				}
			}
		}
	}

	delete []huffman_DC_Y;
    delete []huffman_DC_Cb;
    delete []huffman_DC_Cr;

    for(int i = 0; i < row_*col_*N*2; i++) {
		delete []huffman_AC_Y[i];
	}
	delete []huffman_AC_Y;
	for(int i = 0; i < row_*col_*N*2; i++) {
		delete []huffman_AC_Cb[i];
	}
	delete []huffman_AC_Cb;
	for(int i = 0; i < row_*col_*N*2; i++) {
		delete []huffman_AC_Cr[i];
	}
	delete []huffman_AC_Cr;

    test3.display();
    

	return 0;
}