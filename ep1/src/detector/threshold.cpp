#include "detector.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

ThresholdTargetDetector::ThresholdTargetDetector(Mat template_file) : TargetDetector(template_file) {
	//namedWindow("DEBUG_TH")
	cout << "new ThresholdTargetDetector" << endl;
}

int ThresholdTargetDetector::match(Mat frame, Mat templ){
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	int result_cols, result_rows;
	Mat result;

	result_cols = frame.cols - templ.cols + 1;
	result_rows = frame.rows - templ.rows + 1;
	if(result_cols <= 0 || result_rows <= 0)
		return 0 ;
	result.create(result_cols, result_rows, CV_32FC1);

	matchTemplate( frame, templ, result, CV_TM_SQDIFF_NORMED );
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	cout << "minVal: " << minVal << endl;

	return 1;
}

/*void ThresholdTargetDetector::process_frame(Mat frame){
	cout << "wtf" << endl;
}*/

void ThresholdTargetDetector::prepare_frame(Mat frame){
	//Mat frame = original_frame.clone();
	cout << "prepare frame" << endl;
	cvtColor(frame,frame,CV_BGRA2GRAY);

	adaptiveThreshold(frame,
		frame,
		255,
		ADAPTIVE_THRESH_GAUSSIAN_C,
		THRESH_BINARY_INV
		,7,7);

	if(DEBUG){
		imshow("DEBUG1", frame);
	}
}