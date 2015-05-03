#include "app.hpp"

#include <iostream>
#include <stdio.h>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

void Application::log(string msg){
	cout << msg << endl;
}

Application::Application(string window_name, string template_file){
	this->window_name = window_name;
	this->template_file = template_file;
	target_detector = NULL;
	capture = NULL;
	this->log("Constructor");
	cout << "template_file: " << template_file << endl;
	cout << "window_name: " << window_name << endl;
}

Application::~Application(){
	this->log("Destructor");
	if(capture != NULL)
		delete capture;
	if(target_detector != NULL)
		delete target_detector;
}

int Application::main_loop(){
	int n = 0;
	char filename[200];
	Mat frame;

	this->log("main_loop begin");
	if(this->prepare())
		return 1;

	while(1){
		*capture >> frame;
		if(frame.empty())
			break;
		frame = this->process_frame(frame);
		imshow(window_name, frame);
		char key = (char)waitKey(5);
		switch(key){
			case 'q':
			case 'Q':
			case 27:
				return 0;
			case 'p': //Save an image
				sprintf(filename,"filename%.3d.jpg",n++);
				imwrite(filename,frame);
				cout << "Saved " << filename << endl;
				break;
			default:
				break;
		}
	}
	this->log("main_loop end");
	return 0;
}



int Application::prepare(){
	this->log("prepare begin");
	this->log(window_name);


	namedWindow(window_name);
	
	namedWindow("DEBUG1",1);

	/* Video Input */
	capture = new VideoCapture(0);
	if (!capture->isOpened()) {
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 1;
	}

	/* Load files */
	Mat templ = imread(template_file,CV_LOAD_IMAGE_COLOR);
	cout << "templ: " << template_file << templ.cols << "x" << templ.rows << endl;
	if(templ.data == NULL){
		cerr << "Failed to load template file!\n" << endl;
	}

	target_detector = new TargetDetector(templ);
	target_detector->init();

	this->log("prepare end");
	return 0;
}

Mat Application::process_frame(Mat frame){
	int offx = 32;
	int offy = offx;

/*	Mat frame;
	cvtColor(original_frame,frame,CV_BGRA2GRAY);*/

/*	adaptiveThreshold(frame,
		frame,
		255,
		ADAPTIVE_THRESH_GAUSSIAN_C,
		THRESH_BINARY_INV
		,7,7);
*/
	Mat control_panel(frame, Rect(offx,offy,frame.cols - 2*offx, frame.rows*0.50 - 2*offy));

	int thickness = 2;
	int lineType = 8;
	Scalar color = Scalar( 255, 0, 0 );

	rectangle( control_panel,
	           Point( 0, 0 ),
	           Point( control_panel.cols, control_panel.rows),
	           color,
	           thickness,
	           lineType );

	//target_detector->process_frame(control_panel);
	return frame;
}

void Application::do_match(Mat frame, Mat templ){
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	Mat result;
	Mat tmp;

	unsigned int i;

	//char wn[255];

	Mat result_display;
	vector<Mat> results;

	for(i=0;i<templates.size();i++){
		tmp = templates[i];

		int result_cols =  frame.cols - tmp.cols + 1;
		int result_rows = frame.rows - tmp.rows + 1;

		if(result_cols <= 0 || result_rows <= 0)
			continue;
		cout << result_cols << "x" << result_rows << endl;
		result.create( result_cols, result_rows, CV_32FC1 );

		matchTemplate( frame, tmp, result, CV_TM_CCOEFF );
		normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		matchLoc = maxLoc;
		cout << "minVal: " << minVal << " maxVal: " << maxVal << " Loc: " << matchLoc.x << "x" << matchLoc.y << endl;
		
		rectangle( frame, matchLoc, Point( matchLoc.x + tmp.cols , matchLoc.y + tmp.rows ), Scalar(0,0,255));
		rectangle( result, matchLoc, Point( matchLoc.x + tmp.cols , matchLoc.y + tmp.rows ), Scalar(0,0,255));
		
		results.push_back(result.clone());
/*		sprintf(wn,"DEBUG%d",i);
		imshow(wn, result);*/
	}

	Mat roi;

	int max_cols = 0;
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
	imshow("DEBUG1",result_display);

}