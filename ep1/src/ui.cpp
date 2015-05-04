#include "ui.hpp"
#include <cstdlib>

using namespace std;
using namespace cv;

Button::Button(string lbl, int w, int h, Scalar c){
	label = lbl;
	color = c;
	width = w;
	height = h;
	is_clicked = false;
	count = 0;
}

void Button::draw(Mat img, Point offset){
	int fontFace = FONT_HERSHEY_SIMPLEX ;
	double fontScale = 2;
	int thickness = 3;

	int baseline=0;
	Size textSize = getTextSize(label, fontFace,
	                fontScale, thickness, &baseline);
	baseline += thickness;

	Point p = Point(offset.x , (img.rows-height)/2);
	r1 = Point(p.x,p.y);
	r2 = Point(p.x + width, p.y + height);

	if(is_clicked){
		rectangle( img,r1,r2,Scalar(255,255,255),-1);
	}

	rectangle(img, r1, r2,color);

	putText(img, label, Point((p.x + (width - textSize.width)/2),( p.y + (textSize.height + height)/2)), fontFace, fontScale,
	color, thickness, 8);
}

bool Button::pt_inside_rect(Point p){
	cout << "Button.pt_inside_rect: " << p.x << "," << p.y;
	cout << " RECT ";
	cout << this->r1.x << "," << this->r1.y << " -- ";
	cout << this->r2.x << "," << this->r2.y << endl;
	
	return (p.x >= this->r1.x && p.x <= this->r2.x) && (p.y >= this->r1.y&& p.y <= this->r2.y);
}

bool Button::intersect(TargetCandidate target){
	return (pt_inside_rect(target.p) || pt_inside_rect(target.q));
}

void Button::click(){
	is_clicked = true;

	if(this->label == "Q"){
		exit(0);
	}
	cout <<"Button.CLICK" << endl;
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

bool ControlPanel::intersect(TargetCandidate target){
	unsigned int i;
	for(i=0;i<buttons.size();i++){
		if(buttons[i].intersect(target)){
			if(++buttons[i].count >= 10){
				buttons[i].click();
			}
			return true;
		}else{
			buttons[i].count = 0;
			buttons[i].is_clicked = false;
		}
			
	}
	return false;
}