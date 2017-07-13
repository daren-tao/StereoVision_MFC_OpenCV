#pragma once
class StereoCalibration
{
public:
	StereoCalibration(void);
	~StereoCalibration(void);

public:
	std::vector<cv::Point2f>	detectChessBoardCorners(cv::Mat& frame, cv::Size boardSize);

	std::vector<std::vector<cv::Point3f>> initObjectPoints(cv::Size boardSize, float squareSize, int image_n);
};

