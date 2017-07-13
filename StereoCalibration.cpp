#include "StdAfx.h"
#include "StereoCalibration.h"


StereoCalibration::StereoCalibration(void)
{
}


StereoCalibration::~StereoCalibration(void)
{
}

std::vector<cv::Point2f> StereoCalibration::detectChessBoardCorners(cv::Mat& frame, cv::Size boadSize)
{
	std::vector<cv::Point2f> corners;

	bool cornersfound = cv::findChessboardCorners(frame, boadSize, corners);
	if(cornersfound)
		cv::cornerSubPix(frame, corners, cv::Size(11, 11), cv::Size(-1, -1), 
		cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.01));

	return corners;
}
