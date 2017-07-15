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
	 *	���̽ǵ����� �ṹ��;
	 */
	struct CornerDatas
	{
		int		nPoints;				// ���̽ǵ�����;
		int		nImages;				// ����ͼ����;
		int		nPointsPerImage;		// ÿ�����̵Ľǵ���;
		cv::Size	imageSize;			// ͼ��ֱ���;
		cv::Size	boardSize;			// ���̳ߴ�;
		vector<vector<cv::Point3f> >	objectPoints;	// ���̽ǵ�������������;
		vector<vector<cv::Point2f> >	imagePoints1;	// ����ͼ�����̽ǵ�������������;
		vector<vector<cv::Point2f> >	imagePoints2;	// ����ͼ�����̽ǵ�������������;
	};

	/***
	 *	��Ŀ�궨���������;
	 */
	struct CameraParams
	{
		cv::Size		imageSize;				// ͼ��ֱ���;
		cv::Mat			cameraMatrix;			// ���������;
		cv::Mat			distortionCoefficients;	// ������������;
		vector<cv::Mat> rotations;				// ����ͼƬ����ת����;
		vector<cv::Mat> translations;			// ����ͼƬ��ƽ������;
		int				flags;					// ��Ŀ�궨���õı�־λ;
	};


	struct StereoParams
	{
		cv::Size		imageSize;		// ͼ��ֱ���;
		CameraParams	cameraParams1;	// ��������궨����;
		CameraParams	cameraParams2;	// ��������궨����;
		cv::Mat			rotation;		// ��ת����;
		cv::Mat			translation;	// ƽ������;
		cv::Mat			essential;		// ���ʾ���;
		cv::Mat			foundational;	// ��������;
		int				flags;			// ˫Ŀ�궨���õı�־λ;
        double          alpha;          // ˫ĿУ��Ч��������ϵ����ȡֵ 0~1 �� -1;
	};

	struct RemapMatrixs
	{
		cv::Mat		mX1;	// ����ͼ X ��������ӳ�����;
		cv::Mat		mY1;	// ����ͼ Y ��������ӳ�����;
		cv::Mat		mX2;	// ����ͼ X ��������ӳ�����;
		cv::Mat		mY2;	// ����ͼ Y ��������ӳ�����;
		cv::Mat		Q;		// ���ڼ�����ά���Ƶ� Q ����;
		cv::Rect	roi1;	// ����ͼ��Ч����ľ���;
		cv::Rect	roi2;	// ����ͼ��Ч����ľ���;
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

