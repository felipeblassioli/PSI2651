#ifndef _INCL_UI
#define _INCL_UI

#include "opencv2/core/core.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "detector.hpp"

using namespace std;
using namespace cv;

class Button {
	public:
		string label;
		Scalar color;
		int width;
		int height;
		bool is_clicked;
		unsigned long int count;

		Button(string lbl, int w, int h, Scalar c = Scalar(255,0,0));

		void draw(Mat img, Point offset);
		bool intersect(TargetCandidate);
		void click();
	private:
		Point r1;
		Point r2;
		bool pt_inside_rect(Point);
};

class ControlPanel {
	public:
		int offx;
		int offy;
		int width;
		int height;

		ControlPanel(int offx = 32, int offy = 32);

		void init(Mat frame);

		void draw(Mat control_panel);

		Rect roi();

		bool intersect(TargetCandidate);
	private:
		vector<Button> buttons;
};

#endif

