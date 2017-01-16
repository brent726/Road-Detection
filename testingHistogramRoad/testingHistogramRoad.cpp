
#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "conio.h"
#include <vector>   
#include <windows.h>
#include <iostream>
using namespace cv;

using namespace std;

// Global variables
//Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;


void removeSmallBlobs(Mat& im, Mat& dstImg , double size)
{
   int largest_contour_index=0;
   double largest_area=0;
	double area;
    Rect bounding_rect;
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
	Mat dst;
	/// Approximate contours to polygons + get bounding rects and circles
    findContours( im, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );

	for( int i = 0; i< contours.size(); i++ )
    {
		cv::drawContours(im, contours, i, Scalar(255, 255, 255), 1);
	}
	imshow("binary white filled Image",im);
	waitKey(10);
	
	// iterate through each contour.
    for( int i = 0; i< contours.size(); i++ )
    {
		area=contourArea( contours[i]); 
		bounding_rect=boundingRect(contours[i]);
		rectangle(dstImg, bounding_rect, Scalar(255,255,255), 2 );
		rectangle(im, bounding_rect, Scalar(255,255,0), 1 );
		//  Find the area of contour
         drawContours(dstImg,contours,i, Scalar(200), 1, 8, hierarchy);
		// Approximate contours to polygons + get bounding rects and circles for the new dstImage
		 //destroyWindow("contourImage"); //destroy the window with the name, "MyWindow"
	}
	imshow("contour lines",dstImg);
	findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	printf("\n");
	
	//waitKey(1000);
	for( int i = 0; i< contours.size(); i++ )
    {
		drawContours( dstImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
		//imshow("contourRoadImage",dstImg);	
	}
	int erode_size = 1;  
    Mat erodeElement = getStructuringElement(cv::MORPH_RECT,Size(2 * erode_size + 1, 2* erode_size + 1),Point(erode_size, erode_size) );
	
	Mat erodeRectContourImg;
	
	//Apply erosion or dilation on the image
    erode(dstImg,erodeRectContourImg,erodeElement); 
	imshow("Result Road Erode Contour",dstImg);
	dstImg=Scalar(0, 0, 0);

	for( int i = 0; i< contours.size(); i++ )
    {
		area=contourArea( contours[i]); 
		if ((area>=0&&area<=size)||area<0)
		{
			drawContours( erodeRectContourImg, contours,i, Scalar(0, 0, 0), CV_FILLED);	
			printf("removed rectangular area:%lf\n",area);
			
		}
		else
		{
			drawContours( erodeRectContourImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
			drawContours(dstImg,contours,i, Scalar(255,255,255), 1, 8, hierarchy);
			//drawContours( dstImg, contours,i, Scalar(0, 0, 0), CV_FILLED);	
			printf("rectangular area:%lf\n",area);
		}
	}
	


	imshow("removal blob",dstImg);

}


int main(int argc, char** argv){
	// read colored BGR image
	Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
	int x,y,L,a,b;
	int xSize, ySize;
	roadImage=imread("100mSnapshot1.png");
	Mat originalRoadImage=roadImage.clone();
	imshow("Road Image",roadImage);
	vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
	GaussianBlur( roadImage, roadImage, Size( 11, 11 ), 0, 0 );
	
	cvtColor(roadImage, labRoadImage, COLOR_BGR2Lab);		

	Mat binaryImage( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
	
	imshow("Road Image Blur",roadImage);

	imshow("CIELAB ROAD Image", labRoadImage);
	printf("Road Image Width= %d\n",roadImage.size().width);
	printf("Road Image Height= %d\n",roadImage.size().height);
	for(int x=0;x<roadImage.size().width;x++)
	{
		for(int y=0;y<roadImage.size().height;y++)
		{
			if((labRoadImage.at<Vec3b>(y,x)[1]>127)&&(labRoadImage.at<Vec3b>(y,x)[1]<133))  // these threshold values must be tuned or determined automatically!
			{
				
				if((labRoadImage.at<Vec3b>(y,x)[2]>127)&&(labRoadImage.at<Vec3b>(y,x)[2]<133)) //these threshold values must be tuned or determined automatically!
				{
					//changing the pixel intensity to white
					binaryImage.at<uchar>(y, x) = 255;
				}
			}
		}
	}

	 // Create a structuring element (SE)
    int morph_size = 6;
	Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	Mat morphImg;
	for (int i=0;i<2;i++)
    {   
		morphologyEx( binaryImage, morphImg, 2, element, Point(-1,-1), i );   
    }   
	//imshow("Morph Binary Image", morphImg);

	
	Mat cannyImg;
	Canny(morphImg,  cannyImg, 80, 80*3, 3 );
	imshow("Result Road Canny",cannyImg);

	Mat dstContourImg( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
	removeSmallBlobs(cannyImg, dstContourImg ,10000);
	/*******coloring the image dilatedCannyImg*******/

	Mat roadImageSegmented=originalRoadImage.clone();
	for(int x=0;x<roadImage.size().width;x++)
	{
		for(int y=0;y<roadImage.size().height;y++)
		{
			if(dstContourImg.at<uchar>(y, x) !=255)
			{
				roadImageSegmented.at<Vec3b>(y,x)[2]=0;
				roadImageSegmented.at<Vec3b>(y,x)[1]=0;
				//roadImageSegmented.at<Vec3b>(y,x)[0]=0;
			}
		}
	}
	imshow("Result Road Contour",roadImageSegmented);
	waitKey(0);
	return 0;
}

