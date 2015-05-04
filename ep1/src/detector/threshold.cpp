#include "detector.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

ThresholdTargetDetector::ThresholdTargetDetector(Mat template_file) : TargetDetector(template_file) {
	//namedWindow("DEBUG_TH")
	cout << "new ThresholdTargetDetector" << endl;
	if(DEBUG){
		namedWindow("THRESH_FRAME",1);
		namedWindow("THRESH_TMATCH",1);
	}
}

int ThresholdTargetDetector::match(Mat frame, Mat& templ, TargetCandidate& candidate){
	cout << "ThresholdTargetDetector::match" << "templ: " << templ.rows << "x" << templ.cols << endl;
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	int result_cols, result_rows;
	Mat result;

	result_cols = frame.cols - templ.cols + 1;
	result_rows = frame.rows - templ.rows + 1;
	if(result_cols <= 0 || result_rows <= 0)
		return 0 ;
	result.create(result_cols, result_rows, CV_32FC1);

	//CV_TM_CCOEFF
	//CV_TM_SQDIFF
	matchTemplate( frame, templ, result, CV_TM_CCOEFF_NORMED );
	//normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	cout << "minVal: " << minVal << "maxVal:" << maxVal << endl;
	candidate.empty = 0;
	candidate.src = maxLoc;
	candidate.templ = &templ;
	candidate.score = maxVal;
	if(maxVal > 0.58)
		candidate.draw(frame);
	else
		candidate.empty = 1;

/*	candidate.empty = 0;
	candidate.src = minLoc;
	candidate.templ = &templ;
	candidate.score = minVal;

	if(minVal < 0.06)
		candidate.draw(frame);
	candidate.draw(result);*/

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

/*void ThresholdTargetDetector::process_frame(Mat frame){
	cout << "wtf" << endl;
}*/

Mat ThresholdTargetDetector::threshold(Mat frame){
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

Mat ThresholdTargetDetector::prepare_template(Mat frame){
	return this->threshold(frame);
}

Mat ThresholdTargetDetector::prepare_frame(Mat frame){
	//Mat frame = original_frame.clone();
	cout << "prepare frame begin" << endl;
	frame = this->threshold(frame);

/*	if(DEBUG){
		imshow("DEBUG1", frame);
	}
	waitKey(0);*/
	cout << "prepare frame end" << endl;
	return frame;
}

int ThresholdTargetDetector::find_best_candidate(vector<TargetCandidate> candidates){
	if (candidates.size() == 0)
		return -1;
	unsigned int i;

	cout << "find_best_candidate" << endl;
	cout << "total candidates: " << candidates.size() << endl;
	int best = 0;
	for(i=1;i<candidates.size();i++){
		cout << "score is " << endl;
		//cout << candidates[i] << endl;
		cout << candidates[i].score << endl;
		if(candidates[i].score > candidates[best].score )
			best = i;
	}
	cout << "best is " << best << " with " << candidates[best].score << " from " << candidates.size() << endl;
	return best;
}

