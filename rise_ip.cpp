#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace cv;
using namespace std;




int main()
{
	
	//char dir[100]={"/home/sedrica/Desktop/images.jpg"};

	Mat img;
	img=imread("images.jpg", CV_LOAD_IMAGE_COLOR); // reading the rgb image in Mat data struct

	Mat img_bw;
	img_bw=convert_to_bw(img,img_bw);  // conversion to greayscale
/*

note: we may have to either pre process the image or not work with the custom bgr to greyscale or both 
	  as they might not be able to detect edge very well 
*/




	Mat grad_x, grad_y;    // derivative along x and y direction respectively
    Mat square_grad_x, square_grad_y;   // absoute value of derivative 

    /// Gradient  along X direction
	Sobel( img_bw, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	/// Gradient Y
	Sobel( image_bw, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );

	square_grad_y=grad_y.mul(grad_y);
	square_grad_x=grad_x.mul(grad_x);

	Mat img_edge;

    edge=addWeighted( square_grad_x,1, square_grad_y, 0.5, 0, grad );    // more weight is provided in the horizontal direction for more importance to lane detection

    Mat img_edge_dilated;
    Point anchor=Point(-1,-1);

	dilate(edge,img_edge_dilated,/* add some structuring element */,anchor,int iterations=1, int borderType=BORDER_CONSTANT, const Scalar& borderValue=morphologyDefaultBorderValue() );
	

	cv::Mat sel = cv::getStructuringElement(MORPH_RECT, cv::Size(9,9)); 
	erode( src, erosion_dst, element );

return 0;
}
