
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

void removeSmallBlobs(Mat& im, Mat& roadImage , double size)
{
   int largest_contour_index=0;
   int largest_area=0;
    Rect bounding_rect;
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    findContours( im, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    // iterate through each contour.
    for( int i = 0; i< contours.size(); i++ )
    {
        //  Find the area of contour
        double a=contourArea( contours[i],false); 
		printf("area:%lf\n",a);
        if(a>largest_area){
            largest_area=a;cout<<i<<" area  "<<a<<endl;
            // Store the index of largest contour
            largest_contour_index=i;               
            // Find the bounding rectangle for biggest contour
            bounding_rect=boundingRect(contours[i]);
		}
	}
    //Scalar color( 255,255,255);  // color of the contour in the
    Scalar color( 255,255,255);
	//Draw the contour and rectangle
    drawContours( roadImage, contours,largest_contour_index, color, CV_FILLED,8,hierarchy);
	imshow("contoured Image",roadImage);
}


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
			if((labRoadImage.at<Vec3b>(y,x)[1]>127)&&(labRoadImage.at<Vec3b>(y,x)[1]<132))  // these threshold values must be tuned or determined automatically!
			{
				
				if((labRoadImage.at<Vec3b>(y,x)[2]>127)&&(labRoadImage.at<Vec3b>(y,x)[2]<132)) //these threshold values must be tuned or determined automatically!
				{
					//changing the pixel intensity to white
					binaryImage.at<uchar>(y, x) = 255;
				}
			}
		}
	}
	imshow("Binary Image", binaryImage);
	

	 // Create a structuring element (SE)
    int morph_size = 3;
	Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	Mat morphImg;
	for (int i=0;i<2;i++)
    {   
		morphologyEx( binaryImage, morphImg, 2, element, Point(-1,-1), i );   
		

		//imshow("result", morphImg);
		waitKey(1000);
    }   
	imshow("Morph Binary Image", morphImg);

	removeSmallBlobs(morphImg, roadImage , 400);
	Mat cImage=imread("contours.png");
	//imshow("contours",cImage);
	cvtColor(cImage, cImage, COLOR_BGR2GRAY);		
	Mat dst;
	//removeSmallBlobs(cImage,  cImage , 400);
	//if(colorSpace == "lab") 	showLAB();
	// prevent from closing
	waitKey(0);
	return 0;
}

