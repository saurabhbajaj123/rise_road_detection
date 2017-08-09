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
	cvtColor(img, img_bw,COLOR_RGB2GRAY);  // conversion to greayscale
/*

note: we may have to either pre process the image or not work with the custom bgr to greyscale or both 
	  as they might not be able to detect edge very well 
*/

	Mat grad_x, grad_y;    // derivative along x and y direction respectively
    Mat square_grad_x, square_grad_y;   // absoute value of derivative 

    /// Gradient  along X direction
	Sobel( img_bw, grad_x,CV_16S, 1, 0, 3, 1,0, BORDER_DEFAULT );
	/// Gradient  along y direction
	Sobel( img_bw, grad_y,CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );

	square_grad_y=grad_y.mul(grad_y);
	square_grad_x=grad_x.mul(grad_x);

	Mat img_edge;

    addWeighted( square_grad_x,1, square_grad_y, 0.5, 0,img_edge);    // more weight is provided in the horizontal direction for more importance to lane detection

    Mat img_edge_dilated;
    Point anchor=Point(-1,-1);
    Size str_elem_dim_11;
    str_elem_dim_11=Size(11,11);

    Mat rect_str_elem=getStructuringElement(MORPH_RECT,str_elem_dim_11,anchor);

    dilate(img_edge,img_edge_dilated,rect_str_elem);
/*
    Mat img_eroded;
    Mat circ_str_elem=getStructuringElement(MORPH_ELLIPSE,str_elem_dim_11,anchor);
    erode( img_edge_dilated,img_eroded,circ_str_elem);
*/	
    Mat img_flood_fill;
    Point origin=Point(0,0);  // is this have to be origin or (-1,-1) ?
    
    floodFill(img_flood_fill,origin,Scalar(255),0,Scalar(),Scalar(),4);   /// got an issue with arguments SCALAR 

    Mat img_openloops_removed;

    morphologyEx(img_flood_fill,img_openloops_removed,MORPH_OPEN,rect_str_elem);  // <------ confirm it 

    Mat img_small_closedloops_removed;
	morphologyEx(img_openloops_removed,img_small_closedloops_removed,MORPH_OPENCLOSED,rect_str_elem);  // comfirm it 

	Mat img_erode_post_fill1;img_erode_post_erode;

	erode(img_small_closedloops_removed,img_erode_post_fill1,circ_str_elem);
	erode(img_erode_post_fill1,img_erode_post_erode,circ_str_elem);

	Mat img_final_filled;
	floodFill(img_final_filled,origin,Scalar(255),0,Scalar(),Scalar(),4);   /// got an issue with arguments SCALAR 

////////////////////////////////////////////////////////////////////////////

	int connected_components_count;

	Mat img_label;
	connected_components_count=onnectedComponents(img_final_filled,img_label,8,int ltype=cv_32s);

	Mat label;
	label = // add matlab labelmatrix function equivalent of opencv 

	Mat final_color_segmented;
	applyColorMap(label,final_color_segmented, COLORMAP_JET);
 
	return 0;
}
