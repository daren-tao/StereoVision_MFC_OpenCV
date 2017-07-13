#include "StdAfx.h"
#include "StereoCalibration.h"


StereoCalibration::StereoCalibration(void)
{
}

StereoCalibration::~StereoCalibration(void)
{
}

std::vector<cv::Point2f> StereoCalibration::detectChessBoardCorners(cv::Mat& frame, cv::Size boardSize)
{
	std::vector<cv::Point2f> corners;

	bool cornersfound = cv::findChessboardCorners(frame, boardSize, corners);
	if(cornersfound)
	{	
		cv::Mat framegray;
		cv::cvtColor(frame, framegray, cv::COLOR_RGB2GRAY);
		cv::cornerSubPix(framegray, corners, cv::Size(11, 11), cv::Size(-1, -1), 
		cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.01));
	}

	return corners;
}

std::vector<std::vector<cv::Point3f>> StereoCalibration::initObjectPoints(cv::Size boardSize, float squareSize, int image_n)
{
	std::vector<std::vector<cv::Point3f>> objPoints;
	objPoints.resize(image_n);

	for (int i = 0; i < image_n; i++)
	{
		for (int j = 0; j < boardSize.height; j++)
		{
			for (int k = 0; k < boardSize.width; k++)
			{
				objPoints[i].push_back(cv::Point3f(j*squareSize, k*squareSize, 0.0f));
			}
		}
	}
	return objPoints;
}
