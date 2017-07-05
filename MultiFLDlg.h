
// MultiFLDlg.h : 头文件;
//

#pragma once


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

	cv::VideoCapture m_camera_right;
	CRect	m_rect_right;
	CDC*	m_pCdc_right;
	HDC		m_hDc_right;
	CWnd*	m_pWnd_right;

	cv::VideoCapture m_camera_left;
	CRect	m_rect_left;
	CDC*	m_pCdc_left;
	HDC		m_hDc_left;
	CWnd*	m_pWnd_left;


public:
	void detectAndShowCircles(cv::Mat& frame0, cv::Mat& frame1);


public:
	afx_msg void OnBnClickedBtnOpenCam();
	afx_msg void OnBnClickedBtnStopVideo();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
