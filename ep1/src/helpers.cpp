#include "helpers.hpp"

using namespace cv;

Mat dcReject(Mat a){
	Scalar m = mean(a);
	Mat res = a - m;
	return res;
}

Mat somatoriaUm(Mat a){
	Scalar soma = sum(abs(a));

	Mat img = a.clone();

	for(int i=0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++) {
			img.at<Vec3b>(i,j)[0] /= soma[0];
			img.at<Vec3b>(i,j)[1] /= soma[1];
			img.at<Vec3b>(i,j)[2] /= soma[2];
		}
			
	return a;
}