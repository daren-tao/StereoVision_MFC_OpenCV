#pragma once
class StereoCalibration
{
public:
	StereoCalibration(void);
	~StereoCalibration(void);

public:
	std::vector<cv::Point2f>	detectChessBoardCorners(cv::Mat& frame, cv::Size boadSize);
};

