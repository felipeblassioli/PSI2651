#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <string>

#include "detector.hpp"
#include "ui.hpp"

#define DEFAULT_TEMPLATE_FILE "default_template.png"

class Application {

public:
	Application(std::string window_name="PSI2651 - EP1",std::string template_file=DEFAULT_TEMPLATE_FILE);
	~Application();
	int main_loop();
private:
	ControlPanel control_panel;
	TargetDetector *target_detector;

	cv::Mat templ;
	cv::VideoCapture *capture;
	
	std::string window_name;
	std::string template_file;


	int prepare();
	cv::Mat process_frame(cv::Mat);
	void log(std::string);
	void init_ui_components(cv::Mat);
};
