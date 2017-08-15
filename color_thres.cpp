#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <time.h>


using namespace cv;
using namespace std;
RNG rng(12345);
 

int main()
{	
	clock_t start=clock();
	Mat img_color=imread("pi.png",1); 
	Mat img_bw=imread("pi.png",0);

	int I,J;
	I=img_bw.cols;
	J=img_bw.rows;

	float thres =20;
	float colorthres=155;
	float greenthres=100;

	for(int i=0;i<J;i++)
	{
		for(int j=0;j<I;j++)
		{	
			int menv=(img_color.at<cv::Vec3b>(i,j)[1]+img_color.at<cv::Vec3b>(i,j)[0]+img_color.at<cv::Vec3b>(i,j)[2])/3;
			if(abs(img_color.at<cv::Vec3b>(i,j)[0]-menv)<thres&&abs(img_color.at<cv::Vec3b>(i,j)[2]-menv)<thres&&abs(img_color.at<cv::Vec3b>(i,j)[1]-menv)<thres)
			{	
				if(img_color.at<cv::Vec3b>(i,j)[1]<colorthres&&img_color.at<cv::Vec3b>(i,j)[0]<colorthres&&img_color.at<cv::Vec3b>(i,j)[2]<colorthres)	
				{	img_color.at<cv::Vec3b>(i,j)[0]=0;
					img_color.at<cv::Vec3b>(i,j)[1]=0;
					img_color.at<cv::Vec3b>(i,j)[2]=0;
				}
				
				else 
					{
						img_color.at<cv::Vec3b>(i,j)[0]=255;
						img_color.at<cv::Vec3b>(i,j)[1]=255;
						img_color.at<cv::Vec3b>(i,j)[2]=255;
					}	
			}	
			else 
				{

					img_color.at<cv::Vec3b>(i,j)[0]=255;
					img_color.at<cv::Vec3b>(i,j)[1]=255;
					img_color.at<cv::Vec3b>(i,j)[2]=255;
				}
		}
	}


	float edgethres=120;
		
		
	Mat grad_x, grad_y;    // derivative along x and y direction respectively
    Mat square_grad_x, square_grad_y;   // absoute value of derivative 
    
    /// Gradient  along X direction
	Sobel( img_bw, grad_x,CV_8U, 1, 0, 3, 1,0, BORDER_DEFAULT );
	/// Gradient  along y direction
	Sobel( img_bw, grad_y,CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT );

	square_grad_y=grad_y.mul(grad_y);
	square_grad_x=grad_x.mul(grad_x);

	Point anchor=Point(0,0);
	
	Mat img_color_to_bw;
	cvtColor(img_color,img_color_to_bw,cv::COLOR_RGB2GRAY);
	addWeighted( square_grad_x,2, square_grad_y,2, 0,img_bw);  

 	for(int i=0;i<J;i++)
		{	
		for(int j=0;j<I;j++)
			{	
				if(img_bw.at<uchar>(i,j)>edgethres)
				{
					img_bw.at<uchar>(i,j)=255;
				}
				else 
					img_bw.at<uchar>(i,j)=0;
			}
		}	

	Size str_elem_dim_55;
    str_elem_dim_55=Size(7,7);
    Mat circ_str_elem=getStructuringElement(MORPH_ELLIPSE,str_elem_dim_55,anchor);

	dilate(img_color_to_bw,img_color_to_bw,circ_str_elem);
	
  	vector<vector<Point> > contours;
  	vector<Vec4i> hierarchy;

 	findContours(img_color_to_bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE, Point(0, 0) );

 	Mat drawing = Mat::zeros( img_color_to_bw.size(), CV_8UC3 );
 	
 	for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( drawing, contours, i, color, -1, 8, hierarchy, 0, Point() );
     }
	clock_t end=clock();
	cout<<(double)(end-start)/CLOCKS_PER_SEC;
	namedWindow("color thres dil", WINDOW_NORMAL);
	resizeWindow("color thres dil", 700, 700);
	imshow("color thres dil",drawing);

    waitKey(0); 
	return 0;
}
