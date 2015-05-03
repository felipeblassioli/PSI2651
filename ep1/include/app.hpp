#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include <string>

#define DEFAULT_TEMPLATE_FILE "default_template.png"

class Application {

public:
	Application(std::string window_name="PSI2651 - EP1",std::string template_file=DEFAULT_TEMPLATE_FILE);
	~Application();
	int main_loop();
private:
	cv::Mat templ;
	cv::Mat result;
	std::vector<cv::Mat> templates;
	cv::VideoCapture *capture;
	
	std::string window_name;
	std::string template_file;

	void log(std::string);
	int prepare();
	cv::Mat process_frame(cv::Mat);
	void do_match(cv::Mat,cv::Mat);

	std::vector<double> get_scale_factors(double,double,int);
};
