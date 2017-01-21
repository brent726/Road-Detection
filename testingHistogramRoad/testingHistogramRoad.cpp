
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
void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
    
}


void removeSmallBlobs(Mat& im, Mat& dstImg , double size)
{

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
	//waitKey(10);
	

	// iterate through each contour.
    for( int i = 0; i< contours.size(); i++ )
    {
		
		//bounding_rect=boundingRect(contours[i]);
		//rectangle(dstImg, bounding_rect, Scalar(255,255,255), 2 );
		//rectangle(im, bounding_rect, Scalar(255,255,255), 1 );
		//  Find the area of contour
         drawContours(dstImg,contours,i, Scalar(200), 1, 8, hierarchy);
		// Approximate contours to polygons + get bounding rects and circles for the new dstImage
		 //destroyWindow("contourImage"); //destroy the window with the name, "MyWindow"
	}
	imshow("contour lines",dstImg);
	
	printf("\n");
	

	int dilate_size = 1;  
    Mat dilateElement = getStructuringElement(cv::MORPH_RECT,Size(2 * dilate_size + 1, 2* dilate_size + 1),Point(dilate_size, dilate_size) );
	dilate(dstImg,dstImg,dilateElement); 
	imshow("dilate lines",dstImg);

	//waitKey(1000);
	findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	for( int i = 0; i< contours.size(); i++ )
    {
		drawContours( dstImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
		imshow("contourRoadImage",dstImg);	
	}
	
	//int erode_size =2;  
   // Mat erodeElement = getStructuringElement(cv::MORPH_RECT,Size(2 * erode_size + 1, 2* erode_size + 1),Point(erode_size, erode_size) );
	
	//Mat erodeRectContourImg;
	
	//Apply erosion or dilation on the image
   // erode(dstImg,erodeRectContourImg,erodeElement); 
	//imshow("Result Road Erode Contour",erodeRectContourImg);
	
	//dstImg=Scalar(0, 0, 0);
	findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	for( int i = 0; i< contours.size(); i++ )
    {
		area=contourArea( contours[i]); 
		printf("area:%lf\n",area);
		if ((area>=0&&area<=size)||area<0)
		{
			//drawContours( erodeRectContourImg, contours,i, Scalar(0, 0, 0), CV_FILLED);	
			drawContours( dstImg, contours,i, Scalar(0, 0, 0), CV_FILLED);	
			printf("removed rectangular area:%lf\n",area);
			
		}
		else
		{	
			//drawContours( erodeRectContourImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
			drawContours( dstImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
			//drawContours( dstImg, contours,i, Scalar(0, 0, 0), CV_FILLED);	
			//printf("rectangular area:%lf\n",area);
		}
	}
	imshow("Result After small blob eliminations",dstImg);

	findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	for(int i=0;i<contours.size();i++)
	{
		bounding_rect=boundingRect(contours[i]);
		rectangle(dstImg, bounding_rect, Scalar(255,255,255), 1 );
		//drawContours( erodeRectContourImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
	}
	
	findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	for(int i=0;i<contours.size();i++)
	{
		//bounding_rect=boundingRect(contours[i]);
		//rectangle(erodeRectContourImg, bounding_rect, Scalar(255,255,255), 1 );
		drawContours( dstImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
	}
	imshow("Result Rectangles",dstImg);
	//findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );


	for(int i=0;i<contours.size();i++)
	{
		//bounding_rect=boundingRect(contours[i]);
		//rectangle(erodeRectContourImg, bounding_rect, Scalar(255,255,255), 1 );
		//drawContours(dstImg,contours,i, Scalar(255), 1, 8, hierarchy);
		//drawContours( dstImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
		
		
	}
	//imshow("removal blob",dstImg);
	//findContours( dstImg, contours, hierarchy,RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	for(int i=0;i<contours.size();i++)
	{
		//drawContours( dstImg, contours,i, Scalar(255, 255, 255), CV_FILLED);	
		//drawContours(dstImg,contours,i, Scalar(255), 1, 8, hierarchy);
	}

	/*vector<Point> pts;
	int x=0;
	for(int i= 0; i < contours.size(); i++)
	{
		for(int j= 0; j < contours[i].size();j++) // run until j < contours[i].size();
		{	
			//cout << contours[i][j] << endl; //do whatever
			//pts[x]=contours[i][j];
			//x++;
		}
	}


	
	//printf("extLeft: %d",extLeft);
	//imshow("points blob",dstImg);

	
	//set the callback function for any mouse event
     setMouseCallback("points blob", CallBackFunc, NULL);*/
	 
}


int main(int argc, char** argv){
	// read colored BGR image

	Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
	int x,y,L,a,b;
	int xSize, ySize;
	Mat originalRoadImage;
	//imshow("Road Image",roadImage);

	VideoCapture capture("Take 2(60m).mp4");
	


	double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	std::cout << "Frame Size = " << dWidth << "x" << dHeight << std::endl;

	//-Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	//VideoWriter oVideoWriter ("LaneDetection.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, true); //initialize the VideoWriter object 
	namedWindow( "Video", 1);
    while (1)
     {
           	capture >> roadImage;
			Mat originalRoadImage=roadImage.clone();
			if(roadImage.empty())
			{
				break;
			}
			
			GaussianBlur( roadImage, roadImage, Size( 3, 3 ), 0, 0 );
	
			cvtColor(roadImage, labRoadImage, COLOR_BGR2Lab);		

			Mat binaryImage( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
	
			//imshow("Road Image Blur",roadImage);

			//imshow("CIELAB ROAD Image", labRoadImage);
			printf("Road Image Width= %d\n",roadImage.size().width);
			printf("Road Image Height= %d\n",roadImage.size().height);
			for(int x=0;x<roadImage.size().width;x++)
			{
				for(int y=0;y<roadImage.size().height;y++)
				{
					if((labRoadImage.at<Vec3b>(y,x)[1]>126)&&(labRoadImage.at<Vec3b>(y,x)[1]<133))  // these threshold values must be tuned or determined automatically!
					{
				
						if((labRoadImage.at<Vec3b>(y,x)[2]>126)&&(labRoadImage.at<Vec3b>(y,x)[2]<133)) //these threshold values must be tuned or determined automatically!
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
			Canny(morphImg,  cannyImg, 50,50*3, 3 );
			//imshow("Result Road Canny",cannyImg);

			Mat dstContourImg( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
			removeSmallBlobs(cannyImg, dstContourImg ,1000);
	
			/*******coloring the image dilatedCannyImg*******/

			Mat roadImageSegmented=originalRoadImage.clone();
			for(int x=0;x<roadImage.size().width;x++)
			{
				for(int y=0;y<roadImage.size().height;y++)
				{
					if(dstContourImg.at<uchar>(y, x) !=255)
					{
						//roadImageSegmented.at<Vec3b>(y,x)[2]=0;
						roadImageSegmented.at<Vec3b>(y,x)[1]=0;
						roadImageSegmented.at<Vec3b>(y,x)[0]=0;
					}
				}
			}
			
		imshow("Result Road Contour",roadImageSegmented);

			imshow("Video", roadImage);
			 waitKey(0); // waits to display frame
	}

	waitKey(0);
	return 0;
}

