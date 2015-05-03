#include "helpers.hpp"

using namespace cv;

Mat dcReject(Mat a){
	Scalar m = mean(a);
	Mat res = a - m;
/*	cout << a << endl;
	cout << "vs" << endl;
	cout << res << endl;*/
	return res;
}

Mat somatoriaUm(Mat a){
	//Scalar absSum = sum(abs(a));
	Scalar soma = sum(abs(a));

	Mat img = a.clone();
/*	for(int i=0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++) 
			soma += abs(img.at<Vec3b>(i,j));*/

	for(int i=0; i<img.rows; i++)
		for(int j=0; j<img.cols; j++) {
			img.at<Vec3b>(i,j)[0] /= soma[0];
			img.at<Vec3b>(i,j)[1] /= soma[1];
			img.at<Vec3b>(i,j)[2] /= soma[2];
		}
			
	return a;
}