
// MultiFLDlg.h : ͷ�ļ�;
//

#pragma once


// CMultiFLDlg �Ի���;
class CMultiFLDlg : public CDialogEx
{
// ����;
public:
	CMultiFLDlg(CWnd* pParent = NULL);	// ��׼���캯��;

// �Ի�������;
	enum { IDD = IDD_MULTIFL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��;


// ʵ��;
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��;
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
