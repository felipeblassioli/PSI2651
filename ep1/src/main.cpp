#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

/* Defaults */
#define DEFAULT_TEMPLATE_FILE "default_template.png"

Mat dcReject(Mat a){
	Scalar m = mean(a);
	Mat res = a - m;
/*	cout << a << endl;
	cout << "vs" << endl;
	cout << res << endl;*/
	return res;
}

Mat somatoriaUm(Mat a){
	//Scalar absSum = sum(abs(a));
	Scalar soma = sum(abs(a));

	Mat img = a.clone();
/*	for(int i=0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++) 
			soma += abs(img.at<Vec3b>(i,j));*/

	for(int i=0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++) {
			img.at<Vec3b>(i,j)[0] /= soma[0];
			img.at<Vec3b>(i,j)[1] /= soma[1];
			img.at<Vec3b>(i,j)[2] /= soma[2];
		}
			
	return a;
}

class Application {

public:
	Application(string,string);
	~Application();
	int main_loop();
private:
	vector<Mat> templates;
	VideoCapture *capture;
	string template_file;
	Mat templ;
	Mat result;
	string window_name;

	void log(string);
	int prepare();
	Mat process_frame(Mat);
	void do_match(Mat,Mat);
};

void Application::log(string msg){
	cout << msg << endl;
}

Application::Application(string window_name="PSI2651 - EP1", string template_file=DEFAULT_TEMPLATE_FILE){
	this->window_name = window_name;
	this->template_file = template_file;
	capture = NULL;
	this->log("Constructor");
}

Application::~Application(){
	this->log("Destructor");
	if(capture != NULL)
		delete capture;
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

	/* Video Input */
	capture = new VideoCapture(0);
	if (!capture->isOpened()) {
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 1;
	}

	/* Load files */
	templ = imread(template_file,CV_LOAD_IMAGE_COLOR);
	cout << templ.cols << endl;
	cout << templ.rows << endl;
	if(templ.data == NULL){
		cerr << "Failed to load template file!\n" << endl;
	}

	unsigned int i;
	Mat t;
	for(i=1;i<=5;i++){
		t = templ.clone();
		t = somatoriaUm(dcReject(t));
		resize(t,t,Size(0,0),0.20/i,0.20/i,INTER_LINEAR);
		templates.push_back(t);
	}

	Mat templates_display;
	Mat tmp,roi;
	vector<Mat> templates_cpy;
	for(i=0; i<templates.size();i++)
		templates_cpy.push_back(templates[i].clone());
	for(i=1; i<templates.size();i++){
		tmp = Mat::zeros(templates_cpy[0].rows, templates_cpy[i].cols, templates_cpy[0].type());
		roi = Mat(tmp, Rect(0,0,templates_cpy[i].cols,templates_cpy[i].rows));
		roi = roi + templates_cpy[i];
		templates_cpy[i] = tmp.clone();
	}

	hconcat(templates_cpy, templates_display);
	//imwrite("templates.png", templates_display);
	imshow("DEBUG",templates_display);
	waitKey(0);

	this->log("prepare end");
	return 0;
}

/*Mat dcReject(Mat a) // Elimina nivel DC (subtrai media)
{ Mat d=a.clone();
  double soma=0.0;
  for (MatIterator_<Vec3b> di=d.begin(); di!=d.end(); di++) soma += (*di);
  double media=soma/d.total();
  for (MatIterator_<Vec3b> di=d.begin(); di!=d.end(); di++) (*di) -= media;
  return d;
}


Mat somatoriaUm(Mat a) // Faz somatoria absoluta da imagem dar um
{ Mat d=a.clone();
  double soma=0.0;
  for (MatIterator_<Vec3b> di=d.begin(); di!=d.end(); di++) soma += abs(*di);
  if (soma<epsilon) erro("Erro somatoriaUm: Divisao por zero"); 
  for (MatIterator_<Vec3b> di=d.begin(); di!=d.end(); di++) (*di) /= soma;
  return d;
}*/

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

	do_match(control_panel, templ);
	return frame;
}

void Application::do_match(Mat frame, Mat templ){
	Mat result;
	Mat tmp;

	int i;

	for(i=1; i<=4; i++){
		//cvtColor(templ,tmp,CV_BGRA2GRAY);
		resize(templ,tmp,Size(0,0),0.20/i,0.20/i,INTER_LINEAR);
		cout << frame.total() << endl;
		cout << frame.rows << "x" << frame.cols << endl;
		cout << tmp.rows << "x" << tmp.cols << endl;

		int result_cols =  frame.cols - tmp.cols + 1;
		int result_rows = frame.rows - tmp.rows + 1;
		result.create( result_cols, result_rows, CV_32FC1 );

/*		x=2*somatoriaUm(dcReject(tmp));
		imshow("DEBUG2",x);*/

		cout << tmp.channels() << endl;
		matchTemplate( frame, tmp, result, CV_TM_CCOEFF );
		normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

		double minVal; double maxVal; Point minLoc; Point maxLoc;
		Point matchLoc;

		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		//cout << result << endl;

		matchLoc = maxLoc;
		cout << "minVal: " << minVal << " maxVal: " << maxVal << " Loc: " << matchLoc.x << "x" << matchLoc.y << endl;
		circle(result, matchLoc, 5, Scalar(0,0,255), -1);
		circle(frame, matchLoc, 5, Scalar(0,0,255), -1);

		if(maxVal == 1)
		rectangle( frame, matchLoc, Point( matchLoc.x + tmp.cols , matchLoc.y + tmp.rows ), Scalar(0,0,255));
		rectangle( result, matchLoc, Point( matchLoc.x + tmp.cols , matchLoc.y + tmp.rows ), Scalar(0,0,255));
		

		imshow("DEBUG", result);
	}


}


int main(int argc, char **argv){
	Application app;
	string template_file (DEFAULT_TEMPLATE_FILE);

	namedWindow("DEBUG",1);
	return app.main_loop();
}