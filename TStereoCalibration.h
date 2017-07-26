#ifndef __T_STEREO_CALIBRATION_H__
#define __T_STEREO_CALIBRATION_H__

#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


static std::vector<cv::Point2f> detectCorners(cv::Mat& frame, cv::Size boardSize);
static std::vector<cv::Point2f> detectCorners(std::string filename, cv::Size boardSize);
static bool calibrateStereoVision(
	std::vector<std::vector<cv::Point2f>>& vcorners1, 
	std::vector<std::vector<cv::Point2f>>& vcorners2,
	cv::Size boardSize, float squareSize, cv::Size imageSize,
	cv::Mat& remap1X, cv::Mat& remap1Y, cv::Mat& remap2X, cv::Mat& remap2Y, cv::Mat& Q
	);

static std::vector<cv::Point2f> detectCorners(cv::Mat& frame, cv::Size boardSize)
{
	std::vector<cv::Point2f> corners;
	bool found = findChessboardCorners(frame, boardSize, corners, 
		CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

	if(found)
	{
		cv::Mat frameGray;
		cv::cvtColor(frame, frameGray, cv::COLOR_RGBA2GRAY);
		cv::cornerSubPix(frameGray, corners, cv::Size(11,11), cv::Size(-1,-1),
			cv::TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, 0.01));
	}
	return corners;
}

static std::vector<cv::Point2f> detectCorners(std::string filename, cv::Size boardSize)
{
	cv::Mat frame = cv::imread(filename);
	std::vector<cv::Point2f> corners = detectCorners(frame, boardSize);

	return corners;
}

static bool calibrateStereoVision(
	std::vector<std::vector<cv::Point2f>>& vcorners1, 
	std::vector<std::vector<cv::Point2f>>& vcorners2,
	cv::Size boardSize, float squareSize, cv::Size imageSize,
	cv::Mat& remap1X, cv::Mat& remap1Y, cv::Mat& remap2X, cv::Mat& remap2Y, cv::Mat& Q
	)
{
	int frame_n = 0;
	if(vcorners1.size() == vcorners2.size())
		frame_n = vcorners1.size();
	else
		return false;

	std::vector<std::vector<cv::Point3f>> objPoints;
	objPoints.resize(frame_n);
	for (int i = 0; i < frame_n; i++)
	{
		for (int j = 0; j < boardSize.height; j++)
			for (int k = 0; k < boardSize.width; k++)
				objPoints[i].push_back(cv::Point3f(j * squareSize, k * squareSize, 0.0f));
	}

	cv::Mat cameraMatrix[2], distCoeffs[2];
	cameraMatrix[0] = cv::Mat::eye(3, 3, CV_64F);
	cameraMatrix[1] = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat R, T, E, F;

	std::vector<cv::Mat> rotations2, rotations1;
	std::vector<cv::Mat> translations2, translations1;
	cv::calibrateCamera(objPoints, vcorners1, imageSize, cameraMatrix[0], distCoeffs[0],
		rotations1, translations1, 142);
	cv::calibrateCamera(objPoints, vcorners2, imageSize, cameraMatrix[1], distCoeffs[1],
		rotations2, translations2, 142);

	double rms = cv::stereoCalibrate(objPoints, vcorners1, vcorners2,
		cameraMatrix[0], distCoeffs[0],
		cameraMatrix[1], distCoeffs[1],
		imageSize, R, T, E, F,
		cv::TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5),
	//	CV_CALIB_FIX_ASPECT_RATIO +
	//	CV_CALIB_ZERO_TANGENT_DIST +
	//	CV_CALIB_SAME_FOCAL_LENGTH +
	//	CV_CALIB_RATIONAL_MODEL +
		519 +
		cv::CALIB_FIX_K3 + cv::CALIB_FIX_K4 + cv::CALIB_FIX_K5);

	cv::Mat R1, R2, P1, P2;
	cv::Rect validRoi[2];
	cv::stereoRectify(cameraMatrix[0], distCoeffs[0],
		cameraMatrix[1], distCoeffs[1],
		imageSize, R, T, R1, R2, P1, P2, Q,
		cv::CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

	//Precompute maps for cv::remap()
	cv::initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, imageSize, CV_16SC2, remap1X, remap1Y);
	cv::initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, imageSize, CV_16SC2, remap2X, remap2Y);
}

#endif