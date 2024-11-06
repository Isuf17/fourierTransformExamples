#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void showImg(cv::Mat& img,const std::string& name){
	cv::namedWindow(name.c_str());
	cv::imshow(name.c_str(),img);
}

void expand_img_to_optimal(cv::Mat& padded,cv::Mat& img) {
	int row = cv::getOptimalDFTSize(img.rows);
	int col = cv::getOptimalDFTSize(img.cols);
	cv::copyMakeBorder(img,padded,0,row - img.rows,0,col - img.cols,cv::BORDER_CONSTANT,cv::Scalar::all(0));
}

void crop_and_rearrange(cv::Mat& magI)
{
	magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    int cx = magI.cols/2;
    int cy = magI.rows/2;
		cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));
		cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    cv::Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

}

void lowpassFilter( cv::Mat& dft_filter,int distance){
	cv::Mat tmp = cv::Mat(dft_filter.rows,dft_filter.cols,CV_32F);
	cv::Point center = cv::Point(dft_filter.rows/2,dft_filter.cols/2);
	double radius;
	for(int i = 0; i < dft_filter.rows; ++i) {
		for(int j = 0; j < dft_filter.cols; ++j) {
			radius = (double)(sqrt(pow((i-center.x),2.0)+pow((j-center.y),2.0)));
			if(radius < distance)
				tmp.at<float>(i,j) = 1.0; 
			else
				tmp.at<float>(i,j) = 0.0;
		}
	}
	cv::Mat toMerge[] = {tmp,tmp};
	cv::merge(toMerge,2,dft_filter); //since we are dealing with Two Channel image which is greyscale
}

void highpassFilter( cv::Mat& dft_filter,int distance){
	cv::Mat tmp = cv::Mat(dft_filter.rows,dft_filter.cols,CV_32F);
	cv::Point center = cv::Point(dft_filter.rows/2,dft_filter.cols/2);
	double radius;
	for(int i = 0; i < dft_filter.rows;++i) {
		for(int j = 0; j < dft_filter.cols;++j) {
			radius = (double)(sqrt(pow((i-center.x),2.0)+pow((j-center.y),2.0)));
			if(radius > distance)
				tmp.at<float>(i,j) = 1.0; 
			else
				tmp.at<float>(i,j) = 0.0;
		}
	}
	cv::Mat toMerge[] = {tmp,tmp};
	cv::merge(toMerge,2,dft_filter); //since we are dealing with Two Channel image which is greyscale
}

cv::Mat fourier_transform(cv::Mat& img) {
	cv::Mat padded;
	expand_img_to_optimal(padded,img);

	// Since the result of Fourier Transformation is in complex form we make two planes to hold  real and imaginary value
	cv::Mat planes[] = {cv::Mat_<float>(padded),cv::Mat::zeros(padded.size(),CV_32F)};
	cv::Mat complexI;
	cv::merge(planes,2,complexI);

	cv::dft(complexI,complexI,cv::DFT_COMPLEX_OUTPUT); // Fourier Transform

	return complexI;
}

void showMagnitudeSpectrum(cv::Mat& complexI,const std::string& name){
	cv::Mat p[2];

	cv::split(complexI,p);
	cv::magnitude(p[0],p[1],p[0]);
	cv::Mat magI = p[0];

	magI += cv::Scalar::all(1);
	cv::log(magI,magI);

	crop_and_rearrange(magI);

	cv::normalize(magI, magI, 0, 1, cv::NORM_MINMAX);
	showImg(magI,name.c_str());

}

void showPhaseSpectrum(cv::Mat& complexI, const std::string& name) {
    cv::Mat p[2];
    cv::split(complexI, p); 

    cv::Mat phase;
    cv::phase(p[0], p[1], phase);

    crop_and_rearrange(phase);

    cv::normalize(phase, phase, 0, 1, cv::NORM_MINMAX);
    showImg(phase, name.c_str());
}


int main(int argc,char** argv){

	cv::Mat input_img,fourier_img;
	input_img = cv::imread("../image.png",cv::IMREAD_GRAYSCALE);
	if(input_img.empty()) {
		fprintf(stderr,"Could not Open image\n\n");
		return -1;
	}

	showImg(input_img,"Input Image");

	cv::Mat complexI = fourier_transform(input_img);
	showMagnitudeSpectrum(complexI,"Input Image Magnitude Spectrum");
    showPhaseSpectrum(complexI,"Input Image Phase Spectrum");

	cv::Mat lowfilter = complexI.clone();
    cv::Mat highfilter = complexI.clone();
	lowpassFilter(lowfilter,30); //Our Low Pass Filter of Radius 30
    highpassFilter(highfilter,15); //Our High Pass Filter of Radius 5

	crop_and_rearrange(complexI);
    cv::Mat lowFilterImg;
    cv::Mat highFilterImg;

	cv::mulSpectrums(complexI,lowfilter,lowFilterImg,0); //Multiplying original image with filter image to get final image
	crop_and_rearrange(lowFilterImg);


	cv::mulSpectrums(complexI,highfilter,highFilterImg,0); //Multiplying original image with filter image to get final image
	crop_and_rearrange(highFilterImg);

	showMagnitudeSpectrum(lowFilterImg,"Low Pass Filter Image Magnitude Spectrum");

	showMagnitudeSpectrum(highFilterImg,"High Pass Filter Image Magnitude Spectrum");


	cv::Mat planes0[2],imgOutput0;
    cv::Mat planes1[2],imgOutput1;

	cv::idft(lowFilterImg,lowFilterImg); //Reversing dft process to get our final image
    cv::idft(highFilterImg,highFilterImg); //Reversing dft process to get our final image


	cv::split(lowFilterImg,planes0);
	cv::normalize(planes0[0], imgOutput0, 0, 1, cv::NORM_MINMAX);


	cv::split(highFilterImg,planes1);
	cv::normalize(planes1[0], imgOutput1, 0, 1, cv::NORM_MINMAX);


	showImg(imgOutput0,"Low Pass Filter");
    showImg(imgOutput1,"High Pass Filter");
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

