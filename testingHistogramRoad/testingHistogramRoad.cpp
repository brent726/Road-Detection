
#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "conio.h"
#include <vector>   

using namespace cv;

using namespace std;

// Global variables
//Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
std::string colorSpace;
bool showSingleChannel;

void removeSmallBlobs(Mat& im, Mat& origImage , double size)
{
   int largest_contour_index=0;
   double largest_area=0;
	double a;

    Rect bounding_rect;
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;

	/// Approximate contours to polygons + get bounding rects and circles
	
    //findContours( im, contours, hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,Point(0,0) );
	findContours( im, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
	RNG rng(12345);
	//Mat drawing=Mat::zeros( im.size(), CV_8UC3 );
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	// iterate through each contour.
    for( int i = 0; i< contours.size(); i++ )
    {
        //  Find the area of contour
         a=contourArea( contours[i],false); 
		printf("area:%lf\n",a);
		//float epsilon = 0.1*arcLength(contours[i],true);
		approxPolyDP( Mat(contours[i]), contours_poly[i], 0.001, true );
        boundRect[i] = boundingRect( contours[i]);
        //minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
        if(a>largest_area){
            //largest_area=a;cout<<i<<" area  "<<a<<endl;
            // Store the index of largest contour
           // largest_contour_index=i;               
            // Find the bounding rectangle for biggest contour
            //bounding_rect=boundingRect(contours[i]);
		}
        //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        //drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
	}
	if (contours.size() > 0)
	{
		//drawContours(original, contours,largest_contour_index, CV_RGB(0, 255, 0), 2, 8, hierarchy);
		//if want to show all contours use below one
		drawContours(origImage,contours,-1, CV_RGB(0, 255, 0), 2, 8, hierarchy);
	}
	imshow("contour Image",origImage);
    //Scalar color( 255,255,255);  // color of the contour in the
    //Scalar color( 255,255,255);
	//Draw the contour and rectangle
    //drawContours( roadImage, contours,largest_contour_index, color, CV_FILLED,8,hierarchy);
	//imshow("contoured Image",roadImage);
	//imshow("contoured Image",origImage);
}


int main(int argc, char** argv){
	// read colored BGR image
	//inputImage = imread("100mSnapshot1.png");
	Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
	int x,y,L,a,b;
	int xSize, ySize;
	//roadImage=imread("100mSnapshot1Crop.png");
	roadImage=imread("100mSnapshot1.png");
	
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
	//imshow("Binary Image", binaryImage);

	//Mat sample=imread("contours.png");
	////Mat graySample;
	//cvtColor(sample, graySample, COLOR_BGR2GRAY);	
	//threshold(graySample, graySample, 100, 255, THRESH_BINARY);
	//imshow("Binary Image", graySample);
	

	

	 // Create a structuring element (SE)
    int morph_size = 4;
	Mat element = getStructuringElement( MORPH_RECT, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	Mat morphImg;
	for (int i=0;i<2;i++)
    {   
		morphologyEx( binaryImage, morphImg, 2, element, Point(-1,-1), i );   
		

		//imshow("result", morphImg);
		waitKey(1000);
    }   
	imshow("Morph Binary Image", morphImg);

	/*******coloring the image yellow no dilation********/
	for(int x=0;x<roadImage.size().width;x++)
	{
		for(int y=0;y<roadImage.size().height;y++)
		{
			if(morphImg.at<uchar>(y, x) ==255)
			{
				roadImage.at<Vec3b>(y,x)[2]=255;
				roadImage.at<Vec3b>(y,x)[1]=255;
			}
		}
	}

	Mat cannyImg;
	Canny(morphImg,  cannyImg, 20, 20*2, 3 );
	imshow("Result Road Canny",cannyImg);
	//imshow("Result Road Without Dilation",roadImage);
	/*******coloring the image yellow no dilation********/
	
	//int dilation_size = 2;  
    //Mat dilationElement = getStructuringElement(cv::MORPH_CROSS,Size(2 * dilation_size + 1, 2 * dilation_size + 1),Point(dilation_size, dilation_size) );
	
	//Mat dilatedImg;
	// Apply erosion or dilation on the image
     //dilate(morphImg,dilatedImg,element); 
	 //imshow("Dilated Image",dilatedImg);


	 /*******coloring the image yellow with dilation********/
	for(int x=0;x<roadImage.size().width;x++)
	{
		for(int y=0;y<roadImage.size().height;y++)
		{
			//if(dilatedImg.at<uchar>(y, x) ==255)
			//{
				//roadImage.at<Vec3b>(y,x)[2]=255;
				//roadImage.at<Vec3b>(y,x)[1]=255;
			//}
		}
	}
	//imshow("Result Road With Dilation",roadImage);
	/*******coloring the image yellow with dilation********/


	removeSmallBlobs(cannyImg, roadImage , 400);

	//imshow("Result Road Contour",roadImage);
	Mat cImage=imread("contours.png");
	Mat grayCImage;
	cvtColor(cImage, grayCImage, COLOR_BGR2GRAY);		
	//removeSmallBlobs(grayCImage, cImage   , 400);
	//imshow("Result Contour",cImage);
	//if(colorSpace == "lab") 	showLAB();
	// prevent from closing
	waitKey(0);
	return 0;
}

