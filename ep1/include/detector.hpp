#ifndef _INCL_DETECT
#define _INCL_DETECT
#include "opencv2/core/core.hpp"
#include <vector>

#ifndef DEBUG
#define DEBUG 0
#endif
class TargetCandidate {
	public:
		cv::Point src,p,q;
		cv::Mat *templ;
		int empty;

		TargetCandidate();
		TargetCandidate(cv::Point, cv::Mat, double);
		void draw(cv::Mat);
		double score;

		bool operator == (const TargetCandidate& d) const {
			if(d.empty || this->empty)
				return false;
			return this->templ->rows == d.templ->rows && this->templ->cols == d.templ->cols;
		}
};

class TargetDetector {
	public:
		TargetDetector(cv::Mat);
		virtual ~TargetDetector();

		void init();
		void process_frame(cv::Mat, TargetCandidate&);
		virtual int match(cv::Mat, cv::Mat&, TargetCandidate& ){return 0;}
		virtual int find_best_candidate(std::vector<TargetCandidate> candidates){return -1;};
	protected:
		cv::Mat original_template;
		std::vector<cv::Mat> templates;

		virtual cv::Mat prepare_template(cv::Mat f){return f;}
		virtual cv::Mat prepare_frame(cv::Mat f){return f;}
		std::vector<double> get_scale_factors(double start=0.1, double end=0.4, int parts=12);
};

class GrayscaleTargetDetector : public TargetDetector {
	public:
		GrayscaleTargetDetector(cv::Mat);
		virtual int match(cv::Mat, cv::Mat&, TargetCandidate& );
		virtual int find_best_candidate(std::vector<TargetCandidate>);
	protected:
		virtual cv::Mat prepare_template(cv::Mat);
		virtual cv::Mat prepare_frame(cv::Mat);
	private:
		cv::Mat threshold(cv::Mat);
};
#endif