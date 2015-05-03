#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

/* Defaults */
#define DEFAULT_TEMPLATE_FILE "default_template.png"


class Application {

public:
	Application(string,string);
	~Application();
	int main_loop();
private:
	VideoCapture *capture;
	string template_file;
	Mat templ;
	Mat result;
	string window_name;

	void log(string);
	int prepare();
	Mat process_frame(Mat);
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

Mat dcReject(Mat a){
	Scalar m = mean(a);
	Mat res = a - m;
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


Mat Application::process_frame(Mat frame){
	int offx = 32;
	int offy = offx;

	/*Mat frame;
	cvtColor(original_frame,frame,CV_BGRA2GRAY);
*/
/*	adaptiveThreshold(frame,
		frame,
		255,
		ADAPTIVE_THRESH_GAUSSIAN_C,
		THRESH_BINARY_INV
		,7,7);*/
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
	cvtColor(templ,tmp,CV_BGRA2GRAY);
	resize(templ,tmp,Size(0,0),0.20,0.20,INTER_LINEAR);
	cout << control_panel.total() << endl;
	cout << control_panel.rows << "x" << control_panel.cols << endl;
	cout << tmp.rows << "x" << tmp.cols << endl;
	int result_cols =  control_panel.cols - tmp.cols + 1;
	int result_rows = control_panel.rows - tmp.rows + 1;
	result.create( result_cols, result_rows, CV_32FC1 );

	//tmp=2*somatoriaUm(dcReject(tmp));

	cout << tmp.channels() << endl;
	matchTemplate( control_panel, tmp, result, CV_TM_CCOEFF_NORMED );
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
	cout << "minVal: " << minVal << " maxVal: " << maxVal << endl;

	matchLoc = minLoc;
	circle(result, matchLoc, 5, Scalar(0,0,255), -1);
	circle(frame, matchLoc, 5, Scalar(0,0,255), -1);
	rectangle( frame, matchLoc, Point( matchLoc.x + tmp.cols , matchLoc.y + tmp.rows ), Scalar(0,0,255));
	rectangle( result, matchLoc, Point( matchLoc.x + tmp.cols , matchLoc.y + tmp.rows ), Scalar(0,0,255));
	
	imshow("DEBUG", result);
	return frame;
}


int main(int argc, char **argv){
	Application app;
	string template_file (DEFAULT_TEMPLATE_FILE);

	namedWindow("DEBUG",1);
	return app.main_loop();
}