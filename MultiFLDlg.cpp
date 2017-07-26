
// MultiFLDlg.cpp : 实现文件;
//

#include "stdafx.h"
#include "MultiFL.h"
#include "MultiFLDlg.h"
#include "afxdialogex.h"

#include <sstream>
#include "T3DReconstruction.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据;
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持;

	// 实现;
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


// CMultiFLDlg 对话框;

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
	ON_BN_CLICKED(IDC_BTN_RECONSTRUCTION, &CMultiFLDlg::OnBnClickedBtnReconstruction)
END_MESSAGE_MAP()


// CMultiFLDlg 消息处理程序;

BOOL CMultiFLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中;

	// IDM_ABOUTBOX 必须在系统命令范围内;
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动;
	//  执行此操作;
	SetIcon(m_hIcon, TRUE);			// 设置大图标;
	SetIcon(m_hIcon, FALSE);		// 设置小图标;

	// TODO: 在此添加额外的初始化代码;
	m_bStopVideo = false;

	m_pWnd2 = GetDlgItem(IDC_STATIC_RIGHT_VIDEO);
	m_pCdc2 = m_pWnd2->GetDC();
	m_hDc2  = m_pCdc2->GetSafeHdc();
	m_pWnd2->GetClientRect(&m_rect2);

	m_pWnd1 = GetDlgItem(IDC_STATIC_LEFT_VIDEO);
	m_pCdc1 = m_pWnd1->GetDC();
	m_hDc1 = m_pCdc1->GetSafeHdc();
	m_pWnd1->GetClientRect(&m_rect1);

	m_pWnd3 = GetDlgItem(IDC_STATIC_DISPARITY);
	m_pCdc3 = m_pWnd3->GetDC();
	m_hDc3 = m_pCdc3->GetSafeHdc();
	m_pWnd3->GetClientRect(&m_rect3);

	m_iSaved = 0;
	m_iChessBoardFrame = 0;

	m_bStereoCalibed = true;

#if 1
	cv::FileStorage fp("stereo_calib.xml", cv::FileStorage::READ);
	if(fp.isOpened())
	{
		fp["QMatrix"] >> m_mQ;
		fp["remap1X"] >> m_mRemap1X;
		fp["remap1Y"] >> m_mRemap1Y;
		fp["remap2X"] >> m_mRemap2X;
		fp["remap2Y"] >> m_mRemap2Y;
	}
#endif

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE;
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

// 如果向对话框添加最小化按钮，则需要下面的代码;
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序;
//  这将由框架自动完成;

void CMultiFLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文;

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中;
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标;
//显示;
HCURSOR CMultiFLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMultiFLDlg::OnBnClickedBtnOpenCam()
{
	// TODO: 在此添加控件通知处理程序代码;
	m_camera1.open(1);
	m_camera2.open(0);
	if(!m_camera1.isOpened() && !m_camera2.isOpened())	
	{
		AfxMessageBox(" open cameras failed !");
		return;
	}

	SetTimer(0, 33, NULL);
}


void CMultiFLDlg::OnBnClickedBtnStopVideo()
{
	// TODO: 在此添加控件通知处理程序代码;
	m_bStopVideo = true;
	KillTimer(0);
}


void CMultiFLDlg::detectAndShowCircles(cv::Mat& frame1, cv::Mat& frame2)
{
	std::vector<cv::Vec3f> circles_left, circles_right;
	cv::Mat gray_left, gray_right;
	cv::cvtColor(frame1, gray_left, cv::COLOR_RGB2GRAY);
	cv::cvtColor(frame2, gray_right, cv::COLOR_RGB2GRAY);
	cv::HoughCircles(gray_left, circles_left, CV_HOUGH_GRADIENT, 1, gray_left.rows / 20);
	cv::HoughCircles(gray_right, circles_right, CV_HOUGH_GRADIENT, 1, gray_right.rows / 20);

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
	// TODO: 在此添加消息处理程序代码和/或调用默认值;
	CvvImage CvvImage1, CvvImage2;
	cv::Mat tmpFrame1, tmpFrame2;
	m_camera1 >> tmpFrame1;
	m_camera2 >> tmpFrame2;

	IplImage frame1, frame2;
	std::vector<cv::Point2f> corner1, corner2;
	std::stringstream filename1, filename2;
	switch(nIDEvent)
	{
	case 0:
		KillTimer(0);
		SetTimer(0, 33, NULL);

		if(m_bStereoCalibed)
		{
			cv::remap(tmpFrame1, tmpFrame1, m_mRemap1X, m_mRemap1Y, CV_INTER_LINEAR);
			cv::remap(tmpFrame2, tmpFrame2, m_mRemap2X, m_mRemap2Y, CV_INTER_LINEAR);

			for(int i = 50; i < tmpFrame1.rows; i += 50)
			{
				cv::line(tmpFrame1, cv::Point(0, i), cv::Point(tmpFrame1.cols, i), cv::Scalar(0, 0, 255));
				cv::line(tmpFrame2, cv::Point(0, i), cv::Point(tmpFrame2.cols, i), cv::Scalar(0, 0, 255));
			}
		}
		frame1 = tmpFrame1;		frame2 = tmpFrame2;
		CvvImage1.CopyOf(&frame1,frame1.nChannels);		CvvImage1.DrawToHDC(m_hDc1,&m_rect1);
		CvvImage2.CopyOf(&frame2,frame2.nChannels);		CvvImage2.DrawToHDC(m_hDc2,&m_rect2);

		break;

	case 1:
		if(m_iChessBoardFrame < 20)
		{
			KillTimer(0);

			corner1 = detectCorners(tmpFrame1, cv::Size(6, 4));
			if(corner1.size() > 0)
				corner2 = detectCorners(tmpFrame2, cv::Size(6, 4));
			if(corner1.size() > 0)
				cv::drawChessboardCorners(tmpFrame1, cv::Size(6, 4), corner1, true);
			if(corner2.size() > 0)
				cv::drawChessboardCorners(tmpFrame2, cv::Size(6, 4), corner2, true);

			cv::bitwise_not(tmpFrame1, tmpFrame1);
			cv::bitwise_not(tmpFrame2, tmpFrame2);

			if (corner1.size() == corner2.size())
			{
				m_vCorners1.push_back(corner1);
				m_vCorners2.push_back(corner2);
			}

			frame1 = tmpFrame1;
			frame2 = tmpFrame2;

			CvvImage1.CopyOf(&frame1,frame1.nChannels);
			CvvImage1.DrawToHDC(m_hDc1,&m_rect1);

			CvvImage2.CopyOf(&frame2,frame2.nChannels);
			CvvImage2.DrawToHDC(m_hDc2,&m_rect2);

			SetTimer(0, 500, NULL);

			m_iChessBoardFrame++;
		}
		else
		{
			KillTimer(0);
			KillTimer(1);
			MessageBox("start calibrating ...", "notice", MB_OK);
			calibrateStereoVision(m_vCorners1, m_vCorners2, cv::Size(6, 4), 35.0f, cv::Size(640, 480),
				m_mRemap1X, m_mRemap1Y, m_mRemap2X, m_mRemap2Y, m_mQ);

			if(m_fsCalib.open("stereo_calib.xml", cv::FileStorage::WRITE))
			{
				m_fsCalib << "remap1X" << m_mRemap1X;
				m_fsCalib << "remap1Y" << m_mRemap1Y;
				m_fsCalib << "remap2X" << m_mRemap2X;
				m_fsCalib << "remap2Y" << m_mRemap2Y;
				m_fsCalib << "QMatrix" << m_mQ;
			}
			m_fsCalib.release();

			m_bStereoCalibed = true;
			MessageBox("stereo calibration finished.", "notice", MB_OK);
			SetTimer(0, 33, NULL);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMultiFLDlg::OnBnClickedBtnStereoCalib()
{
	// TODO: 在此添加控件通知处理程序代码;
	SetTimer(1, 3000, NULL);
}

void getProcessInput(cv::Mat& frame1, cv::Mat& frame2, int index)
{
	std::stringstream filename1, filename2;
	if(index >= 0 && index < 10)
	{
		filename1 << "image/2010_03_04_drive_0041/I1_00000" << index << ".png";
		filename2 << "image/2010_03_04_drive_0041/I2_00000" << index << ".png";
	}
	if(index >= 10 && index < 100)
	{
		filename1 << "image/2010_03_04_drive_0041/I1_0000" << index << ".png";
		filename2 << "image/2010_03_04_drive_0041/I2_0000" << index << ".png";
	}
	if(index >= 100 && index < 1000)
	{
		filename1 << "image/2010_03_04_drive_0041/I1_000" << index << ".png";
		filename2 << "image/2010_03_04_drive_0041/I2_000" << index << ".png";
	}
	frame1 = cv::imread(filename1.str());
	frame2 = cv::imread(filename2.str());
}

void CMultiFLDlg::OnBnClickedBtnReconstruction()
{
	// TODO: 在此添加控件通知处理程序代码;
	for( int i = 0; i < 448; i++)
	{
		cv::Mat frame1, frame2;
		getProcessInput(frame1, frame2, i);

		uchar* disparity = new uchar [frame1.rows * frame1.cols];
		process(frame1, frame2, disparity);
		cv::Mat disp(frame1.rows, frame1.cols, CV_8UC1, disparity);

		CvvImage CvvImage1, CvvImage3;
		IplImage iplframe1, iplframe3;
		iplframe1 = frame1;
		iplframe3 = disp;
		CvvImage1.CopyOf(&iplframe1,iplframe1.nChannels);
		CvvImage1.DrawToHDC(m_hDc1,&m_rect1);

		CvvImage3.CopyOf(&iplframe3,iplframe3.nChannels);
		CvvImage3.DrawToHDC(m_hDc3,&m_rect3);
		delete disparity;
	}
}
