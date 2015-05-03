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

	target_detector = new ThresholdTargetDetector(templ);
	target_detector->init();

	this->log("prepare end");
	return 0;
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

Mat Application::process_frame(Mat frame){
	int offx = 32;
	int offy = offx;

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

	cout << 'p' << endl;
	target_detector->process_frame(control_panel);
	return frame;
}