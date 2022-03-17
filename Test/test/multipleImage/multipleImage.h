#ifndef MULTIPLEIMAGE_H
#define MULTIPLEIMAGE_H

#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#define CHANGE  0
#define NO_CHANGE 1

void multipleImage(vector<Mat> imgVector, Mat& dst, int imgCols, int mode = NO_CHANGE, int pixel=300);  //参数为列
void multipleImage(vector<Mat> imgVector, Mat& dst, int imgCols, double ratio);  //参数为列
void ManyImages(vector<Mat> Images, Mat& dst, int imgRows);  //行


#endif // MULTIPLEIMAGE_H
