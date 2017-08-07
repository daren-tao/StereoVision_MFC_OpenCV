
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

void getProcessInput(cv::Mat& frame1, cv::Mat& frame2, int index);


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
	ON_BN_CLICKED(IDC_BTN_3D_SAVE, &CMultiFLDlg::OnBnClickedBtn3dSave)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE_SIZE, &CMultiFLDlg::OnCbnSelchangeComboImageSize)
	ON_BN_CLICKED(IDC_RADIO_LOAD_CALIB_PARAS, &CMultiFLDlg::OnBnClickedRadioLoadCalibParas)
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

	m_bSaveCur3D = false;
	m_bStereoCalibed = false;

	((CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE))->AddString("1920 * 1080");
	((CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE))->AddString("1280 * 1024");
	((CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE))->AddString("1024 * 768");
	((CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE))->AddString("640 * 480");
	((CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE))->AddString("320 * 240");
	
	((CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE))->SetCurSel(0);
	m_imageWide = 1920;
	m_imageHigh = 1080;

	m_bLoadCalibParas = false;
	((CButton*)GetDlgItem(IDC_RADIO_LOAD_CALIB_PARAS))->SetCheck(FALSE);

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
#ifdef CAM_DS
	if(!m_camera1.OpenCamera(0, false, m_imageWide, m_imageHigh))
	{
		AfxMessageBox(_T("打开左摄像头失败."));		return;
	}
	if(!m_camera2.OpenCamera(1, false, m_imageWide, m_imageHigh))
	{
		AfxMessageBox(_T("打开右摄像头失败."));		return;
	}
#else
	m_camera1.open(1);
	m_camera2.open(0);
	if(!m_camera1.isOpened() && !m_camera2.isOpened())
	{
		AfxMessageBox(" open cameras failed !");
		return;
	}
#endif
	GetDlgItem(IDC_BTN_OPEN_CAM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_STOP_VIDEO)->EnableWindow(TRUE);

#if 1
	std::stringstream calibfile;
	calibfile << "calib_paras_" << m_imageWide << "_" << m_imageHigh << ".xml";
	cv::FileStorage fp(calibfile.str(), cv::FileStorage::READ);
	if(fp.isOpened())
	{
		fp["QMatrix"] >> m_mQ;
		fp["remapX1"] >> m_mRemapX1;
		fp["remapY1"] >> m_mRemapY1;
		fp["remapX2"] >> m_mRemapX2;
		fp["remapY2"] >> m_mRemapY2;
	}
#endif

	SetTimer(0, 33, NULL);
}


void CMultiFLDlg::OnBnClickedBtnStopVideo()
{
	// TODO: 在此添加控件通知处理程序代码;
	m_bStopVideo = true;
	KillTimer(0);
	KillTimer(1);
#ifdef CAM_DS
	m_camera1.CloseCamera();
	m_camera2.CloseCamera();
#else
	m_camera1.release();
	m_camera2.release();
#endif
	GetDlgItem(IDC_BTN_OPEN_CAM)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_STOP_VIDEO)->EnableWindow(FALSE);
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
	CvvImage CvvImage1, CvvImage2, CvvImage3;
	cv::Mat tmpFrame1, tmpFrame2;

	IplImage frame1, frame2, frame3;
	std::vector<cv::Point2f> corner1, corner2;
	std::stringstream filename1, filename2;
	std::stringstream calibfilesave, calibfileread;
	char txt[16];
	uchar* disparity;
	cv::Mat disp;
	cv::Mat pointCloud;
	cv::Point3f point;
	std::ofstream fp;

#ifdef CAM_DS
	tmpFrame1 = cv::Mat(m_camera1.QueryFrame());
	tmpFrame2 = cv::Mat(m_camera2.QueryFrame());
#else
	m_camera1 >> tmpFrame1;
	m_camera2 >> tmpFrame2;
#endif

	switch(nIDEvent)
	{
	case 0:
		KillTimer(0);

		if(m_bStereoCalibed)
		{
			cv::remap(tmpFrame1, tmpFrame1, m_mRemapX1, m_mRemapY1, CV_INTER_LINEAR);
			cv::remap(tmpFrame2, tmpFrame2, m_mRemapX2, m_mRemapY2, CV_INTER_LINEAR);

		//	cv::resize(tmpFrame1, tmpFrame1, cv::Size(320, 240), 0, 0, cv::INTER_LINEAR);
		//	cv::resize(tmpFrame2, tmpFrame2, cv::Size(320, 240), 0, 0, cv::INTER_LINEAR);

			cv::resize(tmpFrame1, tmpFrame1, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
			cv::resize(tmpFrame2, tmpFrame2, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);

			disparity = new uchar [tmpFrame1.rows * tmpFrame1.cols];
			process(tmpFrame1, tmpFrame2, disparity);
			disp = cv::Mat(tmpFrame1.rows, tmpFrame1.cols, CV_8UC1, disparity);

			frame3 = disp;
			CvvImage3.CopyOf(&frame3, frame3.nChannels);
			CvvImage3.DrawToHDC(m_hDc3,&m_rect3);
			delete disparity;

			for(int i = 50; i < tmpFrame1.rows; i += 50)
			{
				cv::line(tmpFrame1, cv::Point(0, i), cv::Point(tmpFrame1.cols, i), cv::Scalar(0, 0, 255));
				cv::line(tmpFrame2, cv::Point(0, i), cv::Point(tmpFrame2.cols, i), cv::Scalar(0, 0, 255));
			}

#if 0
			if (m_bSaveCur3D)
			{
				fp.open("pointclouds.txt", std::ios::out);
				/*
				cv::reprojectImageTo3D(disp, pointCloud, m_mQ, true);
				for (int y = 0; y < pointCloud.rows; ++y)
				{
					for (int x = 0; x < pointCloud.cols; ++x)
					{
						point = pointCloud.at<cv::Point3f>(y,x);
						fp << point.x << " " << point.y << " " << point.z << "\n";
					}
				}
				*/
				for (int i=0;i<tmpFrame1.rows * tmpFrame1.cols;i++)
				{
					fp << int(disparity[i]) << "\n";
				}
				fp.close();
				break;
			}
#endif
		}
		sprintf(txt, "%d / 20", m_iChessBoardFrame);
		cv::putText(tmpFrame1, txt, cv::Point(50, 20), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 255));

		frame1 = tmpFrame1;		frame2 = tmpFrame2;
		CvvImage1.CopyOf(&frame1,frame1.nChannels);		CvvImage1.DrawToHDC(m_hDc1,&m_rect1);
		CvvImage2.CopyOf(&frame2,frame2.nChannels);		CvvImage2.DrawToHDC(m_hDc2,&m_rect2);

		SetTimer(0, 33, NULL);

		break;

	case 1:
		if(m_iChessBoardFrame < 20)
		{
			KillTimer(0);
			KillTimer(1);

			corner1 = detectCorners(tmpFrame1, cv::Size(6, 4));
			if(corner1.size() > 0)
				corner2 = detectCorners(tmpFrame2, cv::Size(6, 4));

			if (corner1.size() == corner2.size() && corner1.size() == 24)
			{
				filename1 << "image/chessBoards/left/left_" << m_imageWide << "_" << m_imageHigh << "_" << m_iChessBoardFrame << ".jpg";
				filename2 << "image/chessBoards/right/right_" << m_imageWide << "_" << m_imageHigh << "_" <<m_iChessBoardFrame << ".jpg";
				cv::imwrite(filename1.str(), tmpFrame1);
				cv::imwrite(filename2.str(), tmpFrame2);
				
				cv::drawChessboardCorners(tmpFrame1, cv::Size(6, 4), corner1, true);
				cv::drawChessboardCorners(tmpFrame2, cv::Size(6, 4), corner2, true);

				cv::bitwise_not(tmpFrame1, tmpFrame1);
				cv::bitwise_not(tmpFrame2, tmpFrame2);
				m_vCorners1.push_back(corner1);
				m_vCorners2.push_back(corner2);
				m_iChessBoardFrame++;
			}

			frame1 = tmpFrame1;
			frame2 = tmpFrame2;

			CvvImage1.CopyOf(&frame1,frame1.nChannels);
			CvvImage1.DrawToHDC(m_hDc1,&m_rect1);

			CvvImage2.CopyOf(&frame2,frame2.nChannels);
			CvvImage2.DrawToHDC(m_hDc2,&m_rect2);

			SetTimer(0, 400, NULL);
			SetTimer(1, 3000, NULL);
		}
		else
		{
			KillTimer(0);	KillTimer(1);
			MessageBox("start calibrating ...", "notice", MB_OK);
			calibrateStereoVision(m_vCorners1, m_vCorners2, cv::Size(6, 4), 35.0f, cv::Size(m_imageWide, m_imageHigh),
				m_mRemapX1, m_mRemapY1, m_mRemapX2, m_mRemapY2, m_mQ);

			calibfilesave << "calib_paras_" << m_imageWide << "_" << m_imageHigh << ".xml";
			if(m_fsCalib.open(calibfilesave.str(), cv::FileStorage::WRITE))
			{
				m_fsCalib << "remapX1" << m_mRemapX1;
				m_fsCalib << "remapY1" << m_mRemapY1;
				m_fsCalib << "remapX2" << m_mRemapX2;
				m_fsCalib << "remapY2" << m_mRemapY2;
				m_fsCalib << "QMatrix" << m_mQ;
			}
			m_fsCalib.release();

			m_bStereoCalibed = true;
			MessageBox("stereo calibration finished.", "notice", MB_OK);
			SetTimer(0, 33, NULL);

			GetDlgItem(IDC_BTN_STEREO_CALIB)->EnableWindow(TRUE);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CMultiFLDlg::OnBnClickedBtnStereoCalib()
{
	// TODO: 在此添加控件通知处理程序代码;
	SetTimer(1, 3000, NULL);

	GetDlgItem(IDC_BTN_STEREO_CALIB)->EnableWindow(FALSE);
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
		cv::resize(frame1, frame1, cv::Size(), 1.0, 1.0, cv::INTER_LINEAR);
		cv::resize(frame2, frame2, cv::Size(), 1.0, 1.0, cv::INTER_LINEAR);

		int64 start = cv::getTickCount();
		uchar* disparity = new uchar [frame1.rows * frame1.cols];
		process(frame1, frame2, disparity);
		cv::Mat disp(frame1.rows, frame1.cols, CV_8UC1, disparity);
		double fps = cv::getTickFrequency() / double(cv::getTickCount() - start);
		char tfps[16], tsize[16];
		sprintf(tfps, "fps : %f", fps);
		cv::putText(disp, tfps, cv::Point(10, 50), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255));
																		   
	//	sprintf(tsize, "size : %d * %d", frame1.cols, frame1.rows);		   
	//	cv::putText(disp, tsize, cv::Point(10, 20), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255));

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


void CMultiFLDlg::OnBnClickedBtn3dSave()
{
	m_bSaveCur3D = true;
}


void CMultiFLDlg::OnCbnSelchangeComboImageSize()
{
	CComboBox* combox = (CComboBox*)GetDlgItem(IDC_COMBO_IMAGE_SIZE);
	int seletedIndex = combox->GetCurSel();
	CString selectedString;
	combox->GetLBText(seletedIndex, selectedString);

	if(selectedString == "1920 * 1080")
	{
		m_imageWide = 1920;	m_imageHigh = 1080;
	}
	if(selectedString == "1280 * 1024")
	{
		m_imageWide = 1280;	m_imageHigh = 1024;
	}
	if(selectedString == "1024 * 768")
	{
		m_imageWide = 1024;	m_imageHigh = 768;
	}
	if(selectedString == "640 * 480")
	{
		m_imageWide = 640;	m_imageHigh = 480;
	}
	if(selectedString == "320 * 240")
	{
		m_imageWide = 320;	m_imageHigh = 240;
	}
}


void CMultiFLDlg::OnBnClickedRadioLoadCalibParas()
{
	if(m_bLoadCalibParas)
	{
		((CButton*)GetDlgItem(IDC_RADIO_LOAD_CALIB_PARAS))->SetCheck(FALSE);
		m_bLoadCalibParas = false;
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_LOAD_CALIB_PARAS))->SetCheck(TRUE);
		m_bLoadCalibParas = true;
	}
}
