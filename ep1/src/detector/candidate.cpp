#include "detector.hpp"
#include <iostream>

using namespace std;
using namespace cv;
TargetCandidate::TargetCandidate(){
	this->src = Point(0,0);
	this->templ = NULL;
	this->score = -1;
	this->empty = 1;
}

TargetCandidate::TargetCandidate(Point src, Mat templ, double score){
	this->src = src;
	this->templ = &templ;
	this->score = score;
	this->empty = 0;

	this->p = Point(src.x,src.y);
	this->q = Point(src.x + templ.cols, src.y + templ.rows);
}

void TargetCandidate::draw(Mat dst){
	cout << "draw" << endl;
	cout << src << endl;
	cout << "templ is " << templ->rows << "x" << templ->cols << endl;
	rectangle(dst, src, Point( src.x + templ->cols , src.y + templ->rows ), Scalar(0,0,255));
	cout << "draw" << endl;
}