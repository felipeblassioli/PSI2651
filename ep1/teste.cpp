#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

Mat templ;
Mat result;
Mat process(Mat frame){
	int offx = 32;
	int offy = offx;

	Mat control_panel(frame, Rect(offx,offy,frame.cols - 2*offx, frame.rows*0.50 - 2*offy));
	//control_panel = Scalar(0,255,0);

	int thickness = 2;
	int lineType = 8;
	Scalar color = Scalar( 255, 0, 0 );

	rectangle( control_panel,
	           Point( 0, 0 ),
	           Point( control_panel.cols, control_panel.rows),
	           color,
	           thickness,
	           lineType );

	Mat tmp;
	resize(templ,tmp,Size(0,0),0.20,0.20,INTER_LINEAR);
	int result_cols =  control_panel.cols - tmp.cols + 1;
	int result_rows = control_panel.rows - tmp.rows + 1;
	result.create( result_cols, result_rows, CV_32FC1 );


	matchTemplate( control_panel, tmp, result, CV_TM_CCOEFF_NORMED );
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	rectangle( frame, minLoc, Point( minLoc.x + tmp.cols , minLoc.y + tmp.rows ), Scalar(0,0,255));
	imshow("DEBUG", result);
	cout << "frame" << frame.rows << "x" << frame.cols << endl;
	cout << "control_panel" << control_panel.rows << "x" << control_panel.cols << endl;
	cout << "result" << result.rows << "x" << result.cols << endl;
	return frame;
}
int main(){
	Mat img = imread("teste.jpg", CV_LOAD_IMAGE_COLOR);
	templ = imread("default_template.png", CV_LOAD_IMAGE_COLOR);

	namedWindow("Teste",1);
	namedWindow( "DEBUG", WINDOW_AUTOSIZE );
	img = process(img);

	imshow("Teste", img);
	waitKey(0);
	return 1;
}