#pragma once

#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;


class StereoCalibration
{
public:
	StereoCalibration(void);
	~StereoCalibration(void);

	/***
	 *	棋盘角点数据 结构体;
	 */
	struct CornerDatas
	{
		int		nPoints;				// 棋盘角点总数;
		int		nImages;				// 棋盘图像数;
		int		nPointsPerImage;		// 每幅棋盘的角点数;
		cv::Size	imageSize;			// 图像分辨率;
		cv::Size	boardSize;			// 棋盘尺寸;
		vector<vector<cv::Point3f> >	objectPoints;	// 棋盘角点世界坐标序列;
		vector<vector<cv::Point2f> >	imagePoints1;	// 左视图的棋盘角点像素坐标序列;
		vector<vector<cv::Point2f> >	imagePoints2;	// 右视图的棋盘角点像素坐标序列;
	};

	/***
	 *	单目标定的输出参数;
	 */
	struct CameraParams
	{
		cv::Size		imageSize;				// 图像分辨率;
		cv::Mat			cameraMatrix;			// 摄像机矩阵;
		cv::Mat			distortionCoefficients;	// 摄像机畸变参数;
		vector<cv::Mat> rotations;				// 棋盘图片的旋转矩阵;
		vector<cv::Mat> translations;			// 棋盘图片的平移向量;
		int				flags;					// 单目标定所用的标志位;
	};


	struct StereoParams
	{
		cv::Size		imageSize;		// 图像分辨率;
		CameraParams	cameraParams1;	// 左摄像机标定参数;
		CameraParams	cameraParams2;	// 右摄像机标定参数;
		cv::Mat			rotation;		// 旋转矩阵;
		cv::Mat			translation;	// 平移向量;
		cv::Mat			essential;		// 本质矩阵;
		cv::Mat			foundational;	// 基础矩阵;
		int				flags;			// 双目标定所用的标志位;
        double          alpha;          // 双目校正效果的缩放系数，取值 0~1 或 -1;
	};

	struct RemapMatrixs
	{
		cv::Mat		mX1;	// 左视图 X 方向像素映射矩阵;
		cv::Mat		mY1;	// 左视图 Y 方向像素映射矩阵;
		cv::Mat		mX2;	// 右视图 X 方向像素映射矩阵;
		cv::Mat		mY2;	// 右视图 Y 方向像素映射矩阵;
		cv::Mat		Q;		// 用于计算三维点云的 Q 矩阵;
		cv::Rect	roi1;	// 左视图有效区域的矩形;
		cv::Rect	roi2;	// 右视图有效区域的矩形;
	};

	enum RECTIFYMETHOD { RECTIFY_BOUGUET, RECTIFY_HARTLEY };

	int initCornerData(int nImages, cv::Size imageSize, cv::Size boardSize, float squareWidth, CornerDatas& cornerDatas);

	int resizeCornerData(int nImages, CornerDatas& cornerDatas);

	int loadCornerData(const char* filename, CornerDatas& cornerDatas);
	
	int saveCornerData(const char* filename, const CornerDatas& cornerDatas);

	int detectCorners(cv::Mat& img1, cv::Mat& img2, CornerDatas& cornerDatas, int imageCount);

	int loadCameraParams(const char* filename, CameraParams& cameraParams);

	int saveCameraParams(const CameraParams& cameraParams, const char* filename = "cameraParams.yml");

	int calibrateStereoCamera(CornerDatas& cornerDatas, StereoParams& stereoParams, bool cameraUncalibrated = false);

	int getStereoCalibrateError(CornerDatas& cornerDatas, StereoParams& sterepParams, double& err);

	int rectifyStereoCamera(CornerDatas& cornerDatas, StereoParams& stereoParams, RemapMatrixs& remapMatrixs, RECTIFYMETHOD method);

	int saveCalibrationDatas(const char* filename, RECTIFYMETHOD method, CornerDatas& cornerDatas, StereoParams& stereoParams, RemapMatrixs& remapMatrixs);

	int remapImage(cv::Mat& img1, cv::Mat& img2, cv::Mat& img1r, cv::Mat& img2r, RemapMatrixs& remapMatrixs);
};

