//objectTrackingTutorial.cpp

//Written by  Kyle Hounslow 2015

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include "stdafx.h"
#include "highgui.h"
#include "cv.h"
#include "conio.h"

using namespace cv;
using namespace std;

// Global variables
Mat inputImage[5], outputImage, channel[3], labImage, roadImage,labRoadImage;
std::string colorSpace;
bool showSingleChannel;


void showLAB();

void showChannels(Mat[], std::string[], double[][3], int);

bool calibrationMode;//used for showing debugging windows, trackbars etc.

bool mouseIsDragging;//used for showing a rectangle on screen as user clicks and drags mouse
bool mouseMove;
bool rectangleSelected;
cv::Point initialClickPoint, currentMousePoint; //keep track of initial point clicked and current position of mouse
cv::Rect rectangleROI; //this is the ROI that the user has selected

void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param);

//to remove small blobs (noise)
void threshSegments(Mat &src, double threshSize) {
    // FindContours:
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat srcBuffer, output;
    src.copyTo(srcBuffer);
    findContours(srcBuffer, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_TC89_KCOS);

    vector<vector<Point> > allSegments;

    // For each segment:
    for (size_t i = 0; i < contours.size(); ++i) {
        cv::drawContours(srcBuffer, contours, i, Scalar(200, 0, 0), 1, 8, hierarchy, 0, Point());
        cv::Rect brect = cv::boundingRect(contours[i]);
        cv::rectangle(srcBuffer, brect, Scalar(255, 0, 0));

        int result;
        vector<Point> segment;
        for (unsigned int row = brect.y; row < brect.y + brect.height; ++row) {
            for (unsigned int col = brect.x; col < brect.x + brect.width; ++col) {
                result = pointPolygonTest(contours[i], Point(col, row), false);
                if (result == 1 || result == 0) {
                    segment.push_back(Point(col, row));
                }
            }
        }
        allSegments.push_back(segment);
    }

    output = Mat::zeros(src.size(), CV_8U);
    int totalSize = output.rows*output.cols;
    for (int segmentCount = 0; segmentCount < allSegments.size(); ++segmentCount) {
        vector<Point> segment = allSegments[segmentCount];
        if(segment.size() > totalSize*threshSize){
            for (int idx = 0; idx < segment.size(); ++idx) {
                output.at<uchar>(segment[idx].y, segment[idx].x) = 255;
            }
        }
    }
	imshow("Output", output);
   // return output;
}



int main(int argc, char** argv){
	// read colored BGR image
	//inputImage = imread("100mSnapshot1.png");
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
	int x,y,L,a,b;
	int xSize, ySize;
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
			/*********accessing the values**************/

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
			/*********accessing the values**************/
			fprintf(f,"********************\n");
			printf("********************\n");
	}
	
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

	threshSegments(binaryImage, 400);
	//imshow("Small Blobs Remove Binary Image", binaryImage);

	//if(colorSpace == "lab") 	showLAB();
	// prevent from closing
	waitKey(0);
	return 0;
}
void clickAndDrag_Rectangle(int event, int x, int y, int flags, void* param){
	//only if calibration mode is true will we use the mouse to change HSV values
	
	if (calibrationMode == true){
		//get handle to video feed passed in as "param" and cast as Mat pointer
		Mat* videoFeed = (Mat*)param;
		
		if (event == CV_EVENT_LBUTTONDOWN && mouseIsDragging == false)
		{
			//keep track of initial point clicked
			initialClickPoint = cv::Point(x, y);
			
			printf("x:%d, y: %d\n",x,y);
			//Vec3d pix_bgr = labImage.ptr<Vec3f>(y)[x]; 
			//Vec3d pix_bgr = inputImage.ptr<Vec3f>(y)[x]; 
			std::vector<cv::Mat> Lab_vec;
			cv::split(labImage,Lab_vec);
			
			int L = labImage.at<Vec3b>(y,x)[0];  //beware the order : openCV treats images as matrixes, thus the y param come first
			int a = labImage.at<Vec3b>(y,x)[1];
			int b = labImage.at<Vec3b>(y,x)[2];  
			printf("L=%d,a=%d,b=%d\n",L,a,b);
			//user has begun dragging the mouse
			mouseIsDragging = true;
		}
		/* user is dragging the mouse */
		if (event == CV_EVENT_MOUSEMOVE && mouseIsDragging == true)
		{
			//keep track of current mouse point
			currentMousePoint = cv::Point(x, y);
			//user has moved the mouse while clicking and dragging
			mouseMove = true;
		}
		/* user has released left button */
		if (event == CV_EVENT_LBUTTONUP && mouseIsDragging == true)
		{
			//set rectangle ROI to the rectangle that the user has selected
			rectangleROI = Rect(initialClickPoint, currentMousePoint);

			//reset boolean variables
			mouseIsDragging = false;
			mouseMove = false;
			rectangleSelected = true;
		}

		if (event == CV_EVENT_RBUTTONDOWN){
			//user has clicked right mouse button
		}
		if (event == CV_EVENT_MBUTTONDOWN){
			//user has clicked middle mouse button
			//enter code here if needed.
		}
	}

}
void showLAB(){
	//cvtColor(inputImage, inputImage, COLOR_BGR2Lab);
	//imshow("LAB Image",inputImage);
}

