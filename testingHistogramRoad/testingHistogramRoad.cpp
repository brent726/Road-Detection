
#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "conio.h"
#include <vector>   

using namespace cv;
using namespace std;

// Global variables
Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
std::string colorSpace;
bool showSingleChannel;


void showimgcontours(Mat &threshedimg,Mat &original)
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	int largest_area = 0;
	int largest_contour_index = 0;
	imshow("before contour threshedimg",threshedimg);
	
	findContours(threshedimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//this will find largest contour
	imshow("threshedimg",threshedimg);
	for (int i = 0; i< contours.size(); i++) // iterate through each contour. 
	{
		printf("cotours size: %d",contours.size());
		double a = contourArea(contours[i], false);  //  Find the area of contour
		if (a>largest_area)
		{
			largest_area = a;
			largest_contour_index = i;                //Store the index of largest contour
		}
		printf("area=%d\n",a);

	}
	//search for largest contour has end

	if (contours.size() > 0)
	{
		//drawContours(original, contours,largest_contour_index, CV_RGB(0, 255, 0), 2, 8, hierarchy);
		//if want to show all contours use below one
		
		drawContours(original,contours,-1, CV_RGB(0, 255, 0), 2, 8, hierarchy);
	}
}



//void RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode)   where, Src as the source image, Dst object image, AreaLimit for the area of the communication field, CheckMode the mode selection, where 0 is the removal of small areas, 1 hole filling. NeihborMode is a neighborhood type, which can be either 4-neighborhood or 8-neighborhood. The following is the implementation of the code.
//to remove small blobs (noise)
//CheckMode: 0????????1???????; NeihborMode?0??4???1??8??;  




int main(int argc, char** argv){
	// read colored BGR image
	//inputImage = imread("100mSnapshot1.png");
	int x,y,L,a,b;
	int xSize, ySize;
	roadImage=imread("100mSnapshot1Crop.png");
	/*
	inputImage[0] = imread("100mTemp1.png");
	inputImage[1] = imread("100mTemp2.png");
	inputImage[2] = imread("100mTemp3.png");
	inputImage[3] = imread("100mTemp4.png");
	inputImage[4] = imread("100mTemp5.png");
	inputImage[5] = imread("100mTemp5.png");
	//inputImage[0] = imread("outsideRoadTemp1.png");
	roadImage=imread("100mSnapshot1Crop.png");
	colorSpace = "lab";
	calibrationMode = true;
	
	//imshow("Input", inputImage);
	
	
	//cv::setMouseCallback("labImage", clickAndDrag_Rectangle, &labImage);
	//cv::setMouseCallback("labImage", clickAndDrag_Rectangle, &labImage);
	
	FILE * f;
	f=fopen("labValues.txt","w");

	int i;
	for(i=0;i<1;i++)
	{
			cvtColor(inputImage[i], labImage, COLOR_BGR2Lab);
			//imshow("inputImage",inputImage[i]);
			//imshow("labImage", labImage);
			xSize=inputImage[i].size().width; 
			ySize=inputImage[i].size().height; 

			fprintf(f,"Image: %d values\n",i);
			printf("Image: %d values\n",i);
			*/
			/*********accessing the values**************/
	/*

			for(x=0;x<xSize;x++)
			{
				for(y=0;y<ySize;y++)
				{
					L = labImage.at<Vec3b>(y,x)[0];  //beware the order : openCV treats images as matrixes, thus the y param come first
					a = labImage.at<Vec3b>(y,x)[1];
					b = labImage.at<Vec3b>(y,x)[2];  
					//fprintf(f,"x:%d, y: %d L=%d,a=%d,b=%d\n",x,y,L,a,b);
					fprintf(f,"%d,%d\n",a,b);
					printf("x:%d, y: %d L=%d,a=%d,b=%d\n",x,y,L,a,b);
				}
				
			}
			*/
			/*********accessing the values**************/
	/*
			fprintf(f,"********************\n");
			printf("********************\n");
	}*/
	
	cvtColor(roadImage, labRoadImage, COLOR_BGR2Lab);		

	Mat binaryImage( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(1));
	imshow("Road Image",roadImage);
	imshow("CIELAB ROAD Image", labRoadImage);
	printf("Road Image Width= %d\n",roadImage.size().width);
	printf("Road Image Height= %d\n",roadImage.size().height);
	for(int x=0;x<roadImage.size().width;x++)
	{
		for(int y=0;y<roadImage.size().height;y++)
		{
			//printf("Road Image x= %d ",x);
			//printf("Road Image y= %d\n",y);
			if((labRoadImage.at<Vec3b>(y,x)[1]>126)&&(labRoadImage.at<Vec3b>(y,x)[1]<131))  // these threshold values must be tuned or determined automatically!
			{
				
				if((labRoadImage.at<Vec3b>(y,x)[2]>127)&&(labRoadImage.at<Vec3b>(y,x)[2]<135)) //these threshold values must be tuned or determined automatically!
				{
					//changing the pixel intensity to white
					binaryImage.at<uchar>(y, x) = 255;
				}
			}
		}
	}
	imshow("Binary Image", binaryImage);
	Mat sample=imread("contours.png");
	Mat graySample;
	cvtColor(sample, graySample, COLOR_BGR2GRAY);	
	threshold(graySample, graySample, 50, 255, THRESH_BINARY);
	imshow("Binary Image", graySample);
	showimgcontours(graySample,sample);
	
	
	//removeSmallBlobs(sampleBinary, 200);
	imshow("Small Blobs Remove Binary Image", sample);

	//if(colorSpace == "lab") 	showLAB();
	// prevent from closing
	waitKey(0);
	return 0;
}

