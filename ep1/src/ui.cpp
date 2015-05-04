#include "ui.hpp"

using namespace std;
using namespace cv;

Button::Button(string lbl, int w, int h, Scalar c){
	label = lbl;
	color = c;
	width = w;
	height = h;
}

void Button::draw(Mat img, Point offset){
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
	//Point textOrg(p.x,(p.y - height)/2);

	Point p = Point(offset.x , (img.rows-height)/2);
	rectangle(
		img, 
		p,
		p + Point(width,height),
		color
	);


	// then put the text itself
	putText(img, label, Point((p.x + (width - textSize.width)/2),( p.y + (textSize.height + height)/2)), fontFace, fontScale,
	Scalar::all(255), thickness, 8);
}


ControlPanel::ControlPanel(int offx, int offy){
	this->offx = offx;
	this->offy = offy;
}

void ControlPanel::init(Mat frame){
	width = (frame.cols - 2*offx);
	height = (frame.rows*0.50 - 2*offy);

	buttons.push_back(Button("+", height*0.5, height*0.80));
	buttons.push_back(Button("-", height*0.5, height*0.80));
	buttons.push_back(Button("Q", height*0.5, height*0.80));
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
	int gap_x = (width - (buttons.size() * buttons[0].width)) / buttons.size();
	for(i=0;i<buttons.size();i++)
		buttons[i].draw(control_panel, Point(gap_x/2 + (buttons[i].width+gap_x)*i,0));
}


Rect ControlPanel::roi(){
	return Rect(offx, offy, width, height);
}