#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace cv;
using namespace std;

 /*sxlp;'
 '
string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}
*/


int main()
{
	Mat img_bw=imread("tumtum.jpg", 0); // reading the rgb image in Mat data struct

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
    
    Size str_elem_dim_55;
    str_elem_dim_55=Size(5,5);

    Size str_elem_dim_11;
    str_elem_dim_11=Size(11,11);
    

    Mat rect_str_elem=getStructuringElement(MORPH_CROSS,str_elem_dim_55,anchor);

    dilate(img_edge,img_edge_dilated,rect_str_elem);
    //Mat mask;      
    img_edge_dilated.convertTo(img_edge_dilated, CV_32S);
    Mat img_flood_fill=Mat::zeros( img_edge_dilated.rows + 2, img_edge_dilated.cols + 2, CV_8UC1 );
    Point origin=Point(2,24);  // is this have to be origin or (-1,-1) ?
    Rect rect;
    floodFill(img_edge_dilated,img_flood_fill,origin,Scalar(255),&rect,Scalar(20),Scalar(20),4); 

    Mat img_openloops_removed; 
    morphologyEx(img_flood_fill,img_openloops_removed,MORPH_OPEN,rect_str_elem);  // <------ confirm it 
    
    Mat img_compliment;
    bitwise_not(img_openloops_removed,img_compliment);

    Mat img_eroded0,img_eroded1;
    Mat circ_str_elem=getStructuringElement(MORPH_ELLIPSE,str_elem_dim_55,anchor);
    erode(img_compliment,img_eroded0,circ_str_elem);
    erode(img_eroded0,img_eroded1,circ_str_elem);

    Mat img_flood_fill2=Mat::zeros( img_edge_dilated.rows + 2, img_edge_dilated.cols + 2, CV_8UC1 );
    img_eroded1.convertTo(img_eroded1,CV32S);
    floodFill(img_eroded1,img_flood_fill2,origin,Scalar(255),&rect,Scalar(20),Scalar(20),4); 
    



    

      /// got an issue with arguments SCALAR 
    //floodFill(img_edge_dilated,,Scalar(255,255,255),rect,Scalar(0),Scalar(0),4);   /// got an issue with arguments SCALAR 
    //floodFill(img_edge_dilated,origin,Scalar(255));
    

    imshow("asdf",img_edge_dilated);
	waitKey(0);

    

    
    /*Mat mask;   
    img_edge_dilated.convertTo(img_edge_dilated, CV_32S);*/
    //cvtColor(img_edge_dilated, mask, CV_BGR2GRAY);

	/*string ty =  type2str( img_edge_dilated.type() );
    cout<<ty<<endl;
*/



/*
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345); 

    findContours( img_edge_dilated, contours, hierarchy, CV_RETR_FLOODFILL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    Mat drawing = Mat::zeros( img_edge_dilated.size(), CV_32SC1 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
    } 

    imshow( "Result window", drawing );
    waitKey(0); 
*/



    
    //= Mat::zeros( img_edge_dilated.rows + 2, img_edge_dilated.cols + 2, CV_8UC1 );

    imshow("asdf",img_openloops_removed);
	waitKey(0);
/*
    Mat img_small_closedloops_removed;
	morphologyEx(img_openloops_removed,img_small_closedloops_removed,MORPH_CLOSE,rect_str_elem);  // comfirm it 

	Mat img_erode_post_fill1,img_erode_post_erode;

	erode(img_small_closedloops_removed,img_erode_post_fill1,circ_str_elem);
	erode(img_erode_post_fill1,img_erode_post_erode,circ_str_elem);



	Mat img_final_filled=Mat::zeros( img_erode_post_erode.rows + 2, img_erode_post_erode.cols + 2, CV_8UC1 );
    //Point origin=Point(2,24);  // is this have to be origin or (-1,-1) ?
    //ect rect;

	
	floodFill(img_erode_post_erode,img_final_filled,origin,Scalar(255),&rect,Scalar(20),Scalar(20),4);   /// got an issue with arguments SCALAR 
	//floodFill(img_erode_post_erode,origin,Scalar(255,0,0),rect,Scalar(),Scalar(),4);   /// got an issue with arguments SCALAR 
	//floodFill(img_erode_post_erode,origin,Scalar(255));
	//imshow("asdf",img_final_filled);
	//waitKey(0);
*/	
////////////////////////////////////////////////////////////////////////////
/*
	int connected_components_count;

	Mat img_label;
	connected_components_count=onnectedComponents(img_final_filled,img_label,8,int ltype=cv_32s);

	Mat label;
	label = // add matlab labelmatrix function equivalent of opencv 

	Mat final_color_segmented;
	applyColorMap(label,final_color_segmented, COLORMAP_JET);*/
 
	return 0;
}
