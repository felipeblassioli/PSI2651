#include "opencv2/core/core.hpp"
#include <vector>

class TargetDetector {
	public:
		TargetDetector(cv::Mat);
		virtual ~TargetDetector();

		void init();
		void process_frame(cv::Mat);
		virtual int match(cv::Mat, cv::Mat){return 0;}
	protected:
		cv::Mat original_template;
		std::vector<cv::Mat> templates;

		virtual void prepare_frame(cv::Mat){}
		std::vector<double> get_scale_factors(double start=0.1, double end=0.3, int parts=5);
};

class ThresholdTargetDetector : public TargetDetector {
	public:
		ThresholdTargetDetector(cv::Mat);
		virtual int match(cv::Mat, cv::Mat);
	protected:
		virtual void prepare_frame(cv::Mat);
};