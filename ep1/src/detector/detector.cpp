#include "detector.hpp"
#include "helpers.hpp"
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

TargetDetector::TargetDetector (Mat template_file){
	original_template = template_file;
}

TargetDetector::~TargetDetector(){
	

}

void TargetDetector::init(){
	unsigned int i;
	Mat t;
	vector<double> factors = get_scale_factors();

	for(i=0;i<factors.size();i++){
		t = original_template.clone();
		t = somatoriaUm(dcReject(t));
		//resize(t,t,Size(0,0),0.4/(i+1),0.4/(i+1),INTER_LINEAR);
		resize(t,t,Size(0,0),factors[i],factors[i],INTER_LINEAR);
		cout << t.rows << "x" << t.cols << endl;

		t = this->prepare_template(t);
		templates.push_back(t);
	}

	if(DEBUG){
		namedWindow("DEBUG0",1);
		Mat templates_display;
		Mat tmp,roi;
		vector<Mat> templates_cpy;
		for(i=0; i<templates.size();i++)
			templates_cpy.push_back(templates[i].clone());

		int max_rows = 0;
		for(i=0; i<templates.size(); i++){
			if(max_rows < templates[i].rows)
				max_rows = templates[i].rows;
		}
		for(i=0; i<templates.size();i++){
			tmp = Mat::zeros(max_rows, templates_cpy[i].cols, templates_cpy[0].type());
			roi = Mat(tmp, Rect(0,0,templates_cpy[i].cols,templates_cpy[i].rows));
			roi = roi + templates_cpy[i];
			templates_cpy[i] = tmp.clone();
		}

		hconcat(templates_cpy, templates_display);
		//imwrite("templates.png", templates_display);
		imshow("DEBUG0",templates_display);
		//waitKey(0);
	}
}

void TargetDetector::process_frame(Mat original_frame){
	vector<TargetCandidate> candidates;
	TargetCandidate candidate;

	Mat frame = original_frame.clone();
	frame = this->prepare_frame(frame);

	unsigned int i = 0;
	for(i=0;i<templates.size();i++){
		if(this->match(frame,templates[i], candidate)){
			if(!candidate.empty){
				cout << "ADDING CANDIDATE " << candidate.empty << endl;
				candidates.push_back(candidate);
			}
				
		}
	}

	int best = this->find_best_candidate(candidates);
	if(best != -1){
		candidates[best].draw(original_frame);
	}
		
}


vector<double> TargetDetector::get_scale_factors(double start, double end, int parts){
	vector<double> ret;

	double inc = end/parts;
	for(int i=0;i<parts;i++){
		cout << (start + inc*i) << endl;
		ret.push_back(start + inc*i);
	}
	return ret;
}


