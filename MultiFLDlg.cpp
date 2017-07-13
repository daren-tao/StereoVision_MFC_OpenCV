
// MultiFLDlg.cpp : ʵ���ļ�;
//

#include "stdafx.h"
#include "MultiFL.h"
#include "MultiFLDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������;
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��;

	// ʵ��;
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMultiFLDlg �Ի���;

CMultiFLDlg::CMultiFLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiFLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiFLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiFLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN_CAM, &CMultiFLDlg::OnBnClickedBtnOpenCam)
	ON_BN_CLICKED(IDC_BTN_STOP_VIDEO, &CMultiFLDlg::OnBnClickedBtnStopVideo)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STEREO_CALIB, &CMultiFLDlg::OnBnClickedBtnStereoCalib)
END_MESSAGE_MAP()


// CMultiFLDlg ��Ϣ�������;

BOOL CMultiFLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵���;

	// IDM_ABOUTBOX ������ϵͳ���Χ��;
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�;
	//  ִ�д˲���;
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��;
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��;

	// TODO: �ڴ���Ӷ���ĳ�ʼ������;
	m_bStopVideo = false;

	m_pWnd2 = GetDlgItem(IDC_STATIC_RIGHT_VIDEO);
	m_pCdc2 = m_pWnd2->GetDC();
	m_hDc2  = m_pCdc2->GetSafeHdc();
	m_pWnd2->GetClientRect(&m_rect2);

	m_pWnd1 = GetDlgItem(IDC_STATIC_LEFT_VIDEO);
	m_pCdc1 = m_pWnd1->GetDC();
	m_hDc1 = m_pCdc1->GetSafeHdc();
	m_pWnd1->GetClientRect(&m_rect1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE;
}

void CMultiFLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���;
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���;
//  �⽫�ɿ���Զ����;

void CMultiFLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������;

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���;
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��;
//��ʾ;
HCURSOR CMultiFLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMultiFLDlg::OnBnClickedBtnOpenCam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������;
	m_camera1.open(0);
	m_camera2.open(1);
	if(!m_camera1.isOpened() && !m_camera2.isOpened())	
	{
		AfxMessageBox(" open cameras failed !");
		return;
	}

	SetTimer(0, 33, NULL);
}


void CMultiFLDlg::OnBnClickedBtnStopVideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������;
	m_bStopVideo = true;
	KillTimer(0);
}


void CMultiFLDlg::detectAndShowCircles(cv::Mat& frame1, cv::Mat& frame2)
{
	std::vector<cv::Vec3f> circles_left, circles_right;
	cv::Mat gray_left, gray_right;
	cv::cvtColor(frame1, gray_left, cv::COLOR_RGB2GRAY);
	cv::cvtColor(frame2, gray_right, cv::COLOR_RGB2GRAY);
	cv::HoughCircles(gray_left, circles_left, CV_HOUGH_GRADIENT, 1, gray_left.rows / 2);
	cv::HoughCircles(gray_right, circles_right, CV_HOUGH_GRADIENT, 1, gray_right.rows / 2);

	for (int i=0;i<circles_left.size();i++)
	{
		cv::Point centre(cvRound(circles_left[i][0]), cvRound(circles_left[i][1]));
		int radius = cvRound(circles_left[i][2]);
		cv::circle(frame1, centre, radius, cv::Scalar(0, 0, 255), 3);
	}
	for (int i=0;i<circles_right.size();i++)
	{
		cv::Point centre(cvRound(circles_right[i][0]), cvRound(circles_right[i][1]));
		int radius = cvRound(circles_right[i][2]);
		cv::circle(frame2, centre, radius, cv::Scalar(0, 0, 255), 3);
	}
}


void CMultiFLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ;
	CvvImage CvvImage1, CvvImage2;
	cv::Mat tmpFrame1, tmpFrame2;
	m_camera1 >> tmpFrame1;
	m_camera2 >> tmpFrame2;

	IplImage frame1 = tmpFrame1;
	IplImage frame2 = tmpFrame2;

	switch(nIDEvent)
	{
	case 0:
		CvvImage1.CopyOf(&frame1,frame1.nChannels);
		CvvImage1.DrawToHDC(m_hDc1,&m_rect1);

		CvvImage2.CopyOf(&frame2,frame2.nChannels);
		CvvImage2.DrawToHDC(m_hDc2,&m_rect2);

		break;

	case 1:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMultiFLDlg::OnBnClickedBtnStereoCalib()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������;
	cv::Mat chessBoard = cv::imread("chessboard.jpg");

	std::vector<cv::Point2f> corners = m_stereoCalibration.detectChessBoardCorners(chessBoard, cv::Size(6, 4));

	if (corners.size() == 24)
		cv::drawChessboardCorners(chessBoard, cv::Size(6, 4), corners, true);

	IplImage frame = chessBoard;
	CvvImage cvvImage;
	cvvImage.CopyOf(&frame,frame.nChannels);
	cvvImage.DrawToHDC(m_hDc1,&m_rect1);
}
