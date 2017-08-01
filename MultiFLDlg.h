
// MultiFLDlg.h : 头文件;
//

#pragma once

#include <sstream>

#include "StereoCalibration.h"

#define CAP_HIGH	240
#define CAP_WIDE	320


// CMultiFLDlg 对话框;
class CMultiFLDlg : public CDialogEx
{
// 构造;
public:
	CMultiFLDlg(CWnd* pParent = NULL);	// 标准构造函数;

// 对话框数据;
	enum { IDD = IDD_MULTIFL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持;


// 实现;
protected:
	HICON m_hIcon;

	// 生成的消息映射函数;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	bool m_bStopVideo;

	cv::VideoCapture m_camera1;
	CRect	m_rect1;
	CDC*	m_pCdc1;
	HDC		m_hDc1;
	CWnd*	m_pWnd1;
	
	cv::VideoCapture m_camera2;
	CRect	m_rect2;
	CDC*	m_pCdc2;
	HDC		m_hDc2;
	CWnd*	m_pWnd2;

	CRect	m_rect3;
	CDC*	m_pCdc3;
	HDC		m_hDc3;
	CWnd*	m_pWnd3;

	cv::Mat m_frame1;	//	left;
	cv::Mat m_frame2;	//	right;

	unsigned int m_iSaved;
	unsigned int m_iChessBoardFrame;
	bool m_bStereoCalibed;

	StereoCalibration m_stereoCalibration;

	std::vector<std::vector<cv::Point2f>> m_vCorners1;
	std::vector<std::vector<cv::Point2f>> m_vCorners2;
	cv::Mat m_mRemap1X, m_mRemap1Y, m_mRemap2X, m_mRemap2Y, m_mQ;

	cv::FileStorage m_fsCalib;

public:
	void detectAndShowCircles(cv::Mat& frame1, cv::Mat& frame2);

public:
	afx_msg void OnBnClickedBtnOpenCam();
	afx_msg void OnBnClickedBtnStopVideo();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnStereoCalib();
	afx_msg void OnBnClickedBtnReconstruction();
};
