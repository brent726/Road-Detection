
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


void removeSmallBlobs(Mat& im, Mat& dstImg,Mat roadImage )
{
	int x,y;
	double area;
    Rect bounding_rect;
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
	Mat dst;
	Mat labRoadImage;
	cvtColor(roadImage, labRoadImage, COLOR_BGR2Lab);	
	for( x=0;x<roadImage.size().width;x++)
			{
				for(y=0;y<roadImage.size().height;y++)
				{
					if((labRoadImage.at<Vec3b>(y,x)[1]>125)&&(labRoadImage.at<Vec3b>(y,x)[1]<135))  // these threshold values must be tuned or determined automatically!
					{				
						if((labRoadImage.at<Vec3b>(y,x)[2]>125)&&(labRoadImage.at<Vec3b>(y,x)[2]<135)) //these threshold values must be tuned or determined automatically!
						{
							//changing the pixel intensity to white
							im.at<uchar>(y, x) = 255;
						}
					}
				}
			}
			imshow("Image", im);
			int counter=0;
			int arrayPercent[1280];
			float percentage;
			x=0;
			FILE *f;
			f=fopen("frequency.txt","w");
			for(int y=0;y<im.size().height;y++)
			{
				for(x=0, counter=0;x<im.size().width;x++)
				{
					if(im.at<uchar>(y,x)==255)  // these threshold values must be tuned or determined automatically!
					{				
						counter++;
					}
				}
				percentage=(float)counter/im.size().width;
				printf("y:%d, percentage: %f\n",y,percentage);
				fprintf(f," %d, %f\n",y,percentage);
				if(percentage>=0.65)
				{
					arrayPercent[y]=255;
				}
				else
				{
					arrayPercent[y]=0;
				}

			}
			for(int y=0;y<im.size().height;y++)
			{
				for(x=0;x<im.size().width;x++)
				{
					im.at<uchar>(y,x)=arrayPercent[y];  // these threshold values must be tuned or determined automatically!
				}
			}

	imshow("bin",im);

	int dilate_size =2;  
    Mat dilateElement = getStructuringElement(cv::MORPH_RECT,Size(2 * dilate_size + 1, 2* dilate_size + 1),Point(dilate_size, dilate_size) );
		
	for(int i=0;i<10;i++)
	{
		//Apply erosion or dilation on the image
		dilate(im,im,dilateElement); 	
	}
	//imshow("Result Road Dilate",im);

	double largest_area=0;
	double a;
	int largest_contour_index;
	findContours( im, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	for( int i = 0; i< contours.size(); i++ )
    {
		a=contourArea( contours[i],false);  //  Find the area of contour
       if(a>largest_area)
	   {
		   largest_area=a;
		   largest_contour_index=i;                //Store the index of largest contour
		   bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
	   }
	}
	drawContours(dstImg,contours,largest_contour_index, Scalar(255), 1, 8, hierarchy);
	//drawContours( dstImg,contours,largest_contour_index, Scalar(255, 255, 255), CV_FILLED);	
	imshow("largest area",dstImg);
	
	
	findContours( dstImg, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	vector<Point> vert(8);
	 y=0;
	 x=0;
		
	 int arrayX[4], arrayY[4];
	for(int i= 0; i < contours.size(); i++)
	{
		for(int j= 0; j < contours[i].size();j++) // run until j < contours[i].size();
		{	
			cout << contours[i][j] << endl; //do whatever
			if(x<4)
			{
				arrayX[x]=contours[i][j].x;
				arrayY[x]=contours[i][j].y;
				x++;
			}
		}
	}	
	
	for(int i=0;i<4;i++)
	{
		printf("%d, %d\n",arrayX[i],arrayY[i]);
	}
	for(int i= 0; i < roadImage.size().width; i++)
	{
		for(int j= 0; j <roadImage.size().height;j++) // run until j < contours[i].size();
		{	
		}
	}	

	setMouseCallback("largest area", CallBackFunc, NULL);
}


int main(int argc, char** argv){
	// read colored BGR image

	Mat inputImage[5], outputImage, channel[3], labImage, roadImage;
	int x,y,L,a,b;
	int xSize, ySize;
	Mat originalRoadImage;
	
	roadImage=imread("1.jpg");
	//VideoCapture capture("Take 2(60m).mp4");
	//imshow("Road Image",roadImage);


	//double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	//double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	//std::cout << "Frame Size = " << dWidth << "x" << dHeight << std::endl;

	//-Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	//VideoWriter oVideoWriter ("LaneDetection.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, true); //initialize the VideoWriter object 
	//namedWindow( "Video", 1);
    //while (1)
     //{
           	//capture >> roadImage;
			originalRoadImage=roadImage.clone();
			//if(roadImage.empty())
			//{
				//break;
			//}
			Mat gradImage;
			Mat roadBlurImage;
			//GaussianBlur( roadImage, roadBlurImage, Size(3,3), 0, 0, BORDER_DEFAULT );
			/// Total Gradient (approximate)
			//addWeighted( roadImage, 1, roadBlurImage, 1, 0, gradImage);

			//imshow( "Road Image Sobel", gradImage );
			//roadImage=gradImage.clone();
				

			Mat binaryImage( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
			printf("Road Image Width= %d\n",roadImage.size().width);
			printf("Road Image Height= %d\n",roadImage.size().height);
			

			Mat dstContourImg( roadImage.size().height,roadImage.size().width, CV_8UC1, Scalar(0));
			removeSmallBlobs(binaryImage, dstContourImg, roadImage);
	
			/*******coloring the image dilatedCannyImg*******/

			Mat roadImageSegmented=originalRoadImage.clone();
			for(int x=0;x<roadImage.size().width;x++)
			{
				for(int y=0;y<roadImage.size().height;y++)
				{
					if(dstContourImg.at<uchar>(y, x)==255)
					{
						roadImageSegmented.at<Vec3b>(y,x)[1]=255;
						roadImageSegmented.at<Vec3b>(y,x)[0]=255;
						roadImageSegmented.at<Vec3b>(y,x)[2]=255;
					}
				}
			}
			
			//imshow("Result Road Contour",roadImageSegmented);
			 waitKey(0); // waits to display frame
	return 0;
}

