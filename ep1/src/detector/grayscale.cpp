#include "detector.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

GrayscaleTargetDetector::GrayscaleTargetDetector(Mat template_file) : TargetDetector(template_file) {
	if(DEBUG){
		namedWindow("THRESH_FRAME",1);
		namedWindow("THRESH_TMATCH",1);
	}
}

int GrayscaleTargetDetector::match(Mat frame, Mat& templ, TargetCandidate& candidate){
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	int result_cols, result_rows;
	Mat result;

	result_cols = frame.cols - templ.cols + 1;
	result_rows = frame.rows - templ.rows + 1;
	if(result_cols <= 0 || result_rows <= 0)
		return 0 ;
	result.create(result_cols, result_rows, CV_32FC1);

	matchTemplate( frame, templ, result, CV_TM_CCOEFF_NORMED );
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	candidate.empty = 0;
	candidate.p = Point(maxLoc.x,maxLoc.y);
	candidate.q = Point(maxLoc.x + templ.cols, maxLoc.y + templ.rows);
	candidate.src = maxLoc;
	candidate.templ = &templ;
	candidate.score = maxVal;
	if(maxVal > 0.58)
		candidate.draw(frame);
	else
		candidate.empty = 1;

	if(DEBUG){
		static unsigned int count = 0;
		static Mat result_display;
		static vector<Mat> results;

		imshow("THRESH_FRAME", frame);
		results.push_back(result);
		if(++count==templates.size()){
			count=0;

			Mat roi,tmp;
			int max_cols = 0;
			unsigned int i;
			for(i=0; i<results.size(); i++){
				if(max_cols < results[i].cols)
					max_cols = results[i].cols;
			}
			for(i=0; i<results.size();i++){
				tmp = Mat::zeros(results[i].rows, max_cols, results[0].type());
				roi = Mat(tmp, Rect(0,0,results[i].cols,results[i].rows));
				roi = roi + results[i];
				results[i] = tmp.clone();
			}
			vconcat(results, result_display);
			imshow("THRESH_TMATCH", result_display);
			results.clear();
		}
		
	}
	
	return 1;
}

Mat GrayscaleTargetDetector::threshold(Mat frame){
	cvtColor(frame,frame,CV_BGRA2GRAY);

/*	adaptiveThreshold(frame,
		frame,
		255,
		ADAPTIVE_THRESH_GAUSSIAN_C,
		THRESH_BINARY,
		3,
		3
	);*/

	return frame;
}

Mat GrayscaleTargetDetector::prepare_template(Mat frame){
	return this->threshold(frame);
}

Mat GrayscaleTargetDetector::prepare_frame(Mat frame){
	/* TODO: Rename this */
	return this->threshold(frame);;
}

int GrayscaleTargetDetector::find_best_candidate(vector<TargetCandidate> candidates){
	if (candidates.size() == 0)
		return -1;
	unsigned int i;

	int best = 0;
	for(i=1;i<candidates.size();i++){
		if(candidates[i].score > candidates[best].score )
			best = i;
	}
	return best;
}

