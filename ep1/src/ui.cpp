#include "ui.hpp"

using namespace std;
using namespace cv;

Button::Button(string lbl, int w, int h, Scalar c){
	label = lbl;
	color = c;
	width = w;
	height = h;
}

void Button::draw(Mat img, Point p){
	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
	double fontScale = 2;
	int thickness = 3;

	int baseline=0;
	Size textSize = getTextSize(label, fontFace,
	                fontScale, thickness, &baseline);
	baseline += thickness;

	// center the text
	/*Point textOrg((img.cols - textSize.width)/2,
	  (img.rows + textSize.height)/2);*/
	Point textOrg(p.x,(p.y - textSize.height)/2);

	// draw the box
	rectangle(
		img, textOrg + Point(0, baseline),
		textOrg + Point(textSize.width, -textSize.height),
		color
	);

	// then put the text itself
	putText(img, label, textOrg, fontFace, fontScale,
	Scalar::all(255), thickness, 8);
}


ControlPanel::ControlPanel(int offx, int offy){
	this->offx = offx;
	this->offy = offy;

	buttons.push_back(Button("+", width*0.5, height*0.5));
}

void ControlPanel::init(Mat frame){
	width = (frame.cols - 2*offx) - offx;
	height = (frame.rows*0.50 - 2*offy) - offy;
}

void ControlPanel::draw(Mat control_panel){
	int thickness = 2;
	int lineType = 8;
	Scalar color = Scalar( 255, 0, 0 );

	rectangle( control_panel,
	           Point( 0, 0 ),
	           Point( control_panel.cols, control_panel.rows),
	           color,
	           thickness,
	           lineType );

	unsigned int i = 0;
	int gap_x = 16;
	for(i=0;i<buttons.size();i++)
		buttons[i].draw(control_panel, Point(i*buttons[i].width+gap_x,control_panel.rows));
}


Rect ControlPanel::roi(Mat){
	return Rect(offx,offy, width + offx, height + offy);
}