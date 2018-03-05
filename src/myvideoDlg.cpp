
// myvideoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myvideo.h"
#include "myvideoDlg.h"
#include "afxdialogex.h"

//#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <algorithm>
#include "facemorph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct Attribute{
	int age;
	int gender;
	Rect r;
};
// CmyvideoDlg 对话框
int bsize;
int hong;


CmyvideoDlg::CmyvideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmyvideoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmyvideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_VIDEO, m_inputVideo);
	DDX_Control(pDX, IDC_LOG, m_log);
	DDX_Control(pDX, IDC_FACE, m_faceDetect);
	DDX_Control(pDX, IDC_ATTRIBUTE, m_faceAttribute);
	DDX_Control(pDX, IDC_QUALITY, m_faceQuality);
	DDX_Control(pDX, IDC_COMBO1, m_noselist);
	DDX_Control(pDX, IDC_COMBO2, m_eyelist);
	DDX_Control(pDX, IDC_POSE, m_poseEstimation);
	DDX_Control(pDX, IDC_BROW, m_brow);
	DDX_Control(pDX, IDC_LIPS, m_lips);
	DDX_Control(pDX, IDC_COMBO3, m_browlist);
	DDX_Control(pDX, IDC_COMBO4, m_lipslist);
	DDX_Control(pDX, IDC_CHECK5, m_saveVideo);
	DDX_Control(pDX, IDC_CHECK1, m_auto);
	DDX_Control(pDX, IDC_FRAMENUM, m_frameNum);
	DDX_Control(pDX, IDC_FACE_CHANGE, m_faceChange);
}

BEGIN_MESSAGE_MAP(CmyvideoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDSTART, &CmyvideoDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDSTOP, &CmyvideoDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PLAY, &CmyvideoDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_SUSPEND, &CmyvideoDlg::OnBnClickedSuspend)
//	ON_BN_CLICKED(IDC_QUALITY, &CmyvideoDlg::OnBnClickedQuality)
//ON_BN_CLICKED(IDC_ATTRIBUTE, &CmyvideoDlg::OnBnClickedAttribute)
ON_BN_CLICKED(IDC_MIAOMEI, &CmyvideoDlg::OnBnClickedMiaomei)
ON_BN_CLICKED(IDC_TUCHU, &CmyvideoDlg::OnBnClickedTuchu)
ON_BN_CLICKED(IDC_EYE, &CmyvideoDlg::OnBnClickedEye)
ON_BN_CLICKED(IDC_NOSE, &CmyvideoDlg::OnBnClickedNose)
ON_BN_CLICKED(IDC_BUTTON1, &CmyvideoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


void CmyvideoDlg::addnoselist()
{
	m_noselist.AddString(L"nose1");
	m_noselist.AddString(L"nose2");
}

void CmyvideoDlg::addeyelist()
{
	m_eyelist.AddString(L"eye1");
	m_eyelist.AddString(L"eye2");
	m_eyelist.AddString(L"eye3");
	m_eyelist.AddString(L"eye4");
}

void CmyvideoDlg::addbrowlist()
{
	m_browlist.AddString(L"brow1");
	m_browlist.AddString(L"brow2");
	m_browlist.AddString(L"brow3");
}
void CmyvideoDlg::addlipslist()
{
	m_lipslist.AddString(L"lips1");
	m_lipslist.AddString(L"lips2");
	m_lipslist.AddString(L"lips3");
	m_lipslist.AddString(L"lips4");
	m_lipslist.AddString(L"mouth1");
	m_lipslist.AddString(L"lips5");
}

// CmyvideoDlg 消息处理程序

BOOL CmyvideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	OneFrameProcess = 0;
	m_inputVideo.SetWindowTextW(L"my3.mp4");
	started = false;
	addnoselist();
	addeyelist();
	addbrowlist();
	addlipslist();

	m_noselist.SelectString(0, L"nose1");
	m_eyelist.SelectString(0, L"eye1");
	m_browlist.SelectString(0, L"brow1");
	m_lipslist.SelectString(0, L"lips1");

	m_noselist.SetWindowTextW(L"nose1");
	m_eyelist.SetWindowTextW(L"eye1");
	m_browlist.SetWindowTextW(L"brow1");
	m_lipslist.SetWindowTextW(L"lips1");

	if (started == false)
	{
		cv::namedWindow("INPUT", 1);
		HWND hWnd = (HWND)cvGetWindowHandle("INPUT");
		HWND hParent = ::GetParent(hWnd);
		::SetParent(hWnd, GetDlgItem(IDC_FRAME_INPUT)->m_hWnd);
		::ShowWindow(hParent, SW_HIDE);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmyvideoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmyvideoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CmyvideoDlg::getInputVideoFilePath()
{
	m_inputVideo.GetWindowTextW(filePath);

	if (filePath.GetLength() > 0)
		return true;
	else
	{
		AfxMessageBox(L"Please, select one video file or image sequence folder!");
		return false;
	}
}


void CmyvideoDlg::OnBnClickedStart()
{
	int nIndex = m_noselist.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_noselist.GetLBText(nIndex, noseName);
	}
	else noseName = "nose1";

	nIndex = m_eyelist.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_eyelist.GetLBText(nIndex, eyeName);
	}
	else eyeName = "eye1";

	nIndex = m_browlist.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_browlist.GetLBText(nIndex, browName);
	}
	else browName = "brow1";

	nIndex = m_lipslist.GetCurSel();
	if (nIndex != CB_ERR)
	{
		m_lipslist.GetLBText(nIndex, lipsName);
	}
	else lipsName = "lips1";

	if (started == false)
	{
		started = true;
		stopped = false;
		m_log.SetWindowTextW(L"Checking...");

		if (!getInputVideoFilePath())
		{
			m_log.SetWindowTextW(L"Stopped...");
			return;
		}
		m_log.SetWindowTextW(L"Starting...");

		// Convert a TCHAR string to a LPCSTR
		CT2CA filePath2(filePath);
		// construct a std::string using the LPCSTR input
		str_filePath = filePath2;
		thread = AfxBeginThread((AFX_THREADPROC)CmyvideoDlg::Thread, (LPVOID) this);
	}
	else
	{
		AfxMessageBox(L"Thread is already initialized!");
		return;
	}
}

DWORD CmyvideoDlg::Thread(LPVOID *lpvParam)
{
	CmyvideoDlg *thr = (CmyvideoDlg*)lpvParam;
	thr->ThreadProcess();
	return NULL;
}

void CmyvideoDlg::ThreadProcess()
{
	CStringA file_path_aux(filePath);
	capture.open((const char *)file_path_aux);
	int filepos = file_path_aux.ReverseFind('\\');
	string allpath;
	allpath = file_path_aux.GetBuffer(0);
	string videoname(allpath.substr(filepos + 1));

	if (!capture.isOpened())
	{
		AfxMessageBox(L"ERROR: Cannot initialize video!");
		m_log.SetWindowTextW(L"Stopped...");
		return;
	}
	started = true;
	frameNum = 0;
	fps = capture.get(CV_CAP_PROP_FPS);
	cv::Size default_size;
	default_size.width = 640;
	default_size.height = 480;
	create = false;
	destroy = false;
	brow = false;
	lips = false;
	nose = false;
	eyes = false;
	bsize = 5;
	hong = 5;

	Mat img_original_aux;
	Mat img;
	Mat result;
	ifstream frameFile;
	ifstream attriFile;
	ifstream keyptFile;
	ifstream qualityFile;
	ofstream direction;
	string dirPath = "dirresult.txt";
	int len = videoname.length();
	string preName(videoname.substr(0, len - 4));

	string framePath= "frame_"+preName+".txt";                                                  //c_str()函数返回C语言的字符串，而不是String类型
	string attriPath = "attri_"+preName+".txt";
	string keyptPath = "keypt_"+preName+".txt";
	string qualityPath = "quality_"+preName+".txt";
	frameFile.open(framePath.c_str(), ios::in);
	attriFile.open(attriPath.c_str(), ios::in);
	keyptFile.open(keyptPath.c_str(), ios::in);
	qualityFile.open(qualityPath.c_str(), ios::in);
	direction.open(dirPath.c_str(), ios::out);

	char resultVideoName[100] = "result.avi";
	double temp1 = 0, temp2 = 0;
	int frame_time = 0;
	int flag = 0;
	int state = 0;

	int ch[] = { 0, 0 };
	int chS[] = { 1, 0 };
	int  chsi[] = { 0, 1 };
	int chV[] = { 2, 0 };
	int chvi[] = { 0, 2 };

	vector <Point2i> leftbrow;
	vector <Point2i> rightbrow;
	vector <Point2i> uplip;
	vector <Point2i> downlip;
	vector <Point2i> lefteye;
	vector <Point2i> righteye;
	vector <Point2i> pnose;
	vector <Point2i> plips;

	vector<Attribute> attributes;

	model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
	model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
	model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
	model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
	model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
	model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

	double focal_length = default_size.width; // Approximate focal length.
	Point2d center = cv::Point2d(default_size.width / 2, default_size.width / 2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion

	//cout << "Camera Matrix " << endl << camera_matrix << endl;
	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;
	Point2d dxdy;
	
	nose_end_point3D.push_back(Point3d(0, 0, 1000.0));

	std::deque<int> headPos;
	std::deque<int> tempPos;
	int currentPos=0;

	CString strFrameNum;
	bool autoFlag = 0;
	bool leftFlag = 0, rightFlag = 0, upFlag = 0, downFlag = 0;
	int facesNum = 0;


	do
	{
		temp1 = static_cast<double>(getTickCount());
		
		if (!OneFrameProcess)
		{
			m_log.SetWindowTextW(L"Running...");
			frameNum++;
			capture >> img;                       //从视频逐帧读入
			
			if (img.empty())
				break;
			resize(img, img_original, default_size);
			img_original.copyTo(result);
			
	
			int f;
			int x, y, width, height;
			facesNum=0;
			if (frameNum == 1)
			{
				frameFile >> f;
				f++;
			}
			while (f != 0 || flag==0)
			{
				frameFile >> x >> y >> width >> height;
				Rect r(x, y, width, height);
				faces.push_back(r);
				frameFile >> f;
				facesNum++;
				flag = 1;
			}
			flag = 0;   //下次可以读取数据
			int px, py;
			for (int i = 0; i < 68*facesNum ; i++)
			{
				keyptFile >> x >> y >> px >> py;
				Point p(px, py);
				keypts.push_back(p);
				if ((i >= 17) && (i <= 21))
					leftbrow.push_back(p);
				if ((i >= 22) && (i <= 26))
					rightbrow.push_back(p);
				if ((i >= 49) && (i <= 55))
					uplip.push_back(p);
				if ((i >= 56) && (i <= 61))
					downlip.push_back(p);
				if ((i >= 62) && (i <= 64))
					uplip.push_back(p);
				if ((i >= 65) && (i <= 68))
					downlip.push_back(p);
				if ((i >= 36) && (i <= 41))
					lefteye.push_back(p);
				if ((i >= 42) && (i <= 47))
					righteye.push_back(p);
				if ((i >= 27) && (i <= 35))
					pnose.push_back(p);
				if ((i >= 48) && (i <= 67))
					plips.push_back(p);

				if (i == 8) image_points.push_back(p);
				if (i == 30) image_points.push_back(p);
				if (i == 36) image_points.push_back(p);
				if (i == 45) image_points.push_back(p);
				if (i == 48) image_points.push_back(p);
				if (i == 54) image_points.push_back(p);

				if (i < 68)
				{
					faceOne.push_back(p);
					faceOnei.push_back(p);
				}
				else
				{
					faceTwo.push_back(p);
					faceTwoi.push_back(p);
				}
			}
			int z0,z1,z2;
			for (int i = 0; i < facesNum; i++)
			{
				Attribute attr;
				attriFile >> z0>>z1>>z2>>x >> y >> width>>height;
				attr.age = z1; attr.gender = z2;
				attr.r = Rect(x, y, width, height);
				attributes.push_back(attr);
			}
			float q;
			string str, str2, str3;
			for (int i = 0; i<facesNum; i++)
			{
				qualityFile >> q;
				qualityFile >> str >> str2 >> str3;
				quality.push_back(q);
			}

			if (m_faceDetect.GetCheck() == BST_CHECKED)
			{
				Rect r;
				Point p;
				vector<Rect>::iterator it;
				vector<Point>::iterator ite;
				for (it = faces.begin(); it < faces.end(); it++)
				{
					r = (*it);
					rectangle(result, r, Scalar(0, 0, 255), 1);
				}
				if (m_faceAttribute.GetCheck() != BST_CHECKED)
		    		  for (ite = keypts.begin(); ite < keypts.end(); ite++)
				      {
				        	p = (*ite);
					        circle(result, p,1, Scalar(0, 255, 255), 2);
				       }	
			}
			double fontScale = 0.5;
			int   thickness = 1;
			if (m_faceAttribute.GetCheck() == BST_CHECKED)
			{
				char str[20];
				int    fontFace = FONT_HERSHEY_SIMPLEX;
				int    baseline = 0;
				vector<Attribute>::iterator it;
				for (it = attributes.begin(); it < attributes.end(); it++)
				{
					string gen;
					if ((*it).gender == 1)
						gen = "Male";
					else if ((*it).gender == -1)
						gen = "Female";
					else
						gen = "";
					sprintf_s(str, "Age:%d%s",  (*it).age, gen.c_str() );
 //				Size TextSize = getTextSize(str, fontFace, fontScale, thickness, &baseline);
					Point p1((*it).r.x, (*it).r.y);
					putText(result, str, p1, fontFace, fontScale, Scalar(0, 255, 255), thickness, 8);
				}
			}
			if (m_faceQuality.GetCheck() == BST_CHECKED)
			{
				char str[20];
				vector<Rect>::iterator it;
				Rect temp; Rect max(0,0,0,0);
				for (it = faces.begin(); it < faces.end(); it++)
				{
					temp = (*it);
					if (temp.area()>max.area())
					{
						max = temp;
					}
				}
				rectangle(result, max, Scalar(0, 0, 255), 1);
				sprintf_s(str, "Quality:%5.3f", q);
				Point p(10, result.rows-10);
				putText(result, str, p, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 2, 8);
			}
		}
		if ((brow || downFlag) && (!OneFrameProcess))
		{
			Mat hsv;
			Mat hue, saturation, val;
			Mat temp_val;
			cvtColor(result, hsv, COLOR_BGR2HSV);
			hue.create(hsv.size(), hsv.depth());
			saturation.create(hsv.size(), hsv.depth());
			val.create(hsv.size(), hsv.depth());
			mixChannels(&hsv, 1, &hue, 1, ch, 1);
			mixChannels(&hsv, 1, &saturation, 1, chS, 1);
			mixChannels(&hsv, 1, &val, 1, chV, 1);
			val.copyTo(temp_val);
			Rect leftBrowBox = boundingRect(leftbrow);
			Rect rightBrowBox = boundingRect(rightbrow);
			Mat img_mask = Mat::zeros(result.size(), CV_8UC1);
			Mat tempImg_left = img_mask(leftBrowBox);
			Mat imgROI_left = result(leftBrowBox);
			Mat tempImg_right = img_mask(rightBrowBox);
			Mat imgROI_right = result(rightBrowBox);
			cvtColor(imgROI_left, imgROI_left, CV_RGB2GRAY);
			cvtColor(imgROI_right, imgROI_right, CV_RGB2GRAY);
			adaptiveThreshold(imgROI_left, imgROI_left, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 7, bsize);
			adaptiveThreshold(imgROI_right, imgROI_right, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 7, bsize);
			imgROI_left.copyTo(tempImg_left);
			imgROI_right.copyTo(tempImg_right);
			img_mask.convertTo(img_mask, CV_64FC1, 1);
			temp_val.convertTo(temp_val, CV_64FC1, 1);
			for (int i = 0; i < temp_val.rows; i++)
			for (int j = 0; j < temp_val.cols; j++)
			{
				if (img_mask.at<double>(i, j) == (255))
				{
					temp_val.at<double>(i, j) = (temp_val.at<double>(i, j)*0.65);
				}
			}
			temp_val.convertTo(temp_val, val.type(), 1);
			Mat temp;
			temp.create(hsv.size(), hsv.type());
			mixChannels(&hue, 1, &temp, 1, ch, 1);
			mixChannels(&saturation, 1, &temp, 1, chsi, 1);
			mixChannels(&temp_val, 1, &temp, 1, chvi, 1);
			cvtColor(temp, temp, CV_HSV2BGR);
			temp.copyTo(result);
/*			if (OneFrameProcess)
			{
				imshow("mask", img_mask);
				imshow("tempImg_left", tempImg_left);
				imshow("tempImg_right", tempImg_right);
			}
*/
		}

		if ((lips || downFlag) && (!OneFrameProcess))
		{
			Mat bgr[3];
			split(result, bgr);
			Mat tempbgr2;
			bgr[2].copyTo(tempbgr2);
			Rect upLipBox = boundingRect(uplip);
			Rect downLipBox = boundingRect(downlip);
			Mat mask_lips = Mat::zeros(result.size(), CV_8UC1);
			Mat tempUp = mask_lips(upLipBox);
			Mat imgROI_up = tempbgr2(upLipBox);
			Mat tempDown = mask_lips(downLipBox);
			Mat imgROI_down = tempbgr2(downLipBox);
			adaptiveThreshold(imgROI_up, imgROI_up, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 7, hong);
			adaptiveThreshold(imgROI_down, imgROI_down, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV,7, hong);
			imgROI_up.copyTo(tempUp);
			imgROI_down.copyTo(tempDown);
			mask_lips.convertTo(mask_lips, CV_64FC1, 1);
			bgr[2].convertTo(bgr[2], CV_64FC1, 1);
			bgr[0].convertTo(bgr[0], CV_64FC1, 1);
			bgr[1].convertTo(bgr[1], CV_64FC1, 1);
			for (int i = 0; i < bgr[2].rows; i++)
			for (int j = 0; j < bgr[2].cols; j++)
			{
				if (mask_lips.at<double>(i, j) == (255))
				{
					bgr[2].at<double>(i, j) = (bgr[2].at<double>(i, j)*1.3>255 ? 255 : bgr[2].at<double>(i, j)*1.3);
					bgr[0].at<double>(i, j) = (bgr[0].at<double>(i, j)*0.5);
					bgr[1].at<double>(i, j) = (bgr[1].at<double>(i, j)*0.5);
				}
			}
			bgr[2].convertTo(bgr[2], tempbgr2.type(), 1);
			bgr[0].convertTo(bgr[0], tempbgr2.type(), 1);
			bgr[1].convertTo(bgr[1], tempbgr2.type(), 1);
			Mat result2;
			result2.create(result.size(), result.type());
			mixChannels(&bgr[0], 1, &result2, 1, ch, 1);
			mixChannels(&bgr[1], 1, &result2, 1, chsi, 1);
			mixChannels(&bgr[2], 1, &result2, 1, chvi, 1);
			result2.copyTo(result);
//			imshow("r", mask_lips);
/*			if (OneFrameProcess)
			{
				imshow("mask", mask_lips);
				imshow("tempImg_left", tempUp);
				imshow("tempImg_right", tempDown);
				imshow("r", tempbgr2);
			}
*/
		}
		if (eyes || rightFlag)
		{
			Rect leftEyeBox = boundingRect(lefteye);
			Rect rightEyeBox = boundingRect(righteye);
			Rect tempLeftEye, tempRightEye;
			tempLeftEye.y = leftEyeBox.y - 3*leftEyeBox.height;
			tempLeftEye.height = leftEyeBox.height*5;
			tempLeftEye.x = leftEyeBox.x -int(0.5*leftEyeBox.width); //int(0.5*leftEyeBox.width);
			tempLeftEye.width = int(leftEyeBox.width*2);

			tempRightEye.y = rightEyeBox.y - 3*rightEyeBox.height;
			tempRightEye.height = rightEyeBox.height*5;
			tempRightEye.x = rightEyeBox.x;// int(0.5*rightEyeBox.width);// +int(0.5*rightEyeBox.width);
			tempRightEye.width =( rightEyeBox.width*2);

			Mat m_lefteye, mask_lefteye, m_righteye, mask_righteye;

			int nIndex = m_eyelist.GetCurSel();
			if (nIndex != CB_ERR)
			{
				m_eyelist.GetLBText(nIndex, eyeName);
			}
			else eyeName = "eye1";

			if (eyeName == "eye1")
			{
				 m_lefteye = imread("res\\left_eye1.jpg");
			     mask_lefteye = imread("res\\left_eye1mask.jpg", 0);
				 m_righteye = imread("res\\right_eye1.jpg");
				 mask_righteye = imread("res\\right_eye1mask.jpg", 0);
			}
			else if (eyeName == "eye2")
			{
				m_lefteye = imread("res\\left_eye2.jpg");
			    mask_lefteye = imread("res\\left_eye2mask.jpg", 0);
				m_righteye = imread("res\\right_eye2.jpg");
				mask_righteye = imread("res\\right_eye2mask.jpg", 0);
			}
			else if (eyeName == "eye3")
			{
				m_lefteye = imread("res\\left_eye3.jpg");
				mask_lefteye = imread("res\\left_eye3mask.jpg", 0);
				m_righteye = imread("res\\right_eye3.jpg");
				mask_righteye = imread("res\\right_eye3mask.jpg", 0);
			}
			else
			{
				m_lefteye = imread("res\\left_eye4.jpg");
				mask_lefteye = imread("res\\left_eye4mask.jpg", 0);
				m_righteye = imread("res\\right_eye4.jpg");
				mask_righteye = imread("res\\right_eye4mask.jpg", 0);
			}
			
//			imshow("maskright", mask_righteye);
			Mat mask_left, mask_right;
			threshold(mask_lefteye, mask_left, 20, 255, THRESH_BINARY_INV);
			threshold(mask_righteye, mask_right,20, 255, THRESH_BINARY_INV);
			resize(mask_left, mask_left, tempLeftEye.size());
			resize(mask_right, mask_right, tempRightEye.size());
			resize(m_lefteye, m_lefteye, tempLeftEye.size());
			resize(m_righteye, m_righteye, tempRightEye.size());
			Mat img_roi_lefteye = result(tempLeftEye);
			Mat img_roi_righteye = result(tempRightEye);
			m_lefteye.copyTo(img_roi_lefteye, mask_left);
			m_righteye.copyTo(img_roi_righteye, mask_right);
		}
		if (nose|| leftFlag)
		{
			Rect temp = boundingRect(pnose);
			Rect rectnose;
			rectnose.y = temp.y - int(0.2 * temp.height);
			rectnose.height =int( temp.height * 1.2);
			rectnose.x = temp.x - int(0.2*temp.width); //int(0.5*leftEyeBox.width);
			rectnose.width = int(temp.width * 1.2);
			Mat m_nose, mask_nose;

			int nIndex = m_noselist.GetCurSel();
			if (nIndex != CB_ERR)
			{
				m_noselist.GetLBText(nIndex, noseName);
			}
			else noseName = "nose1";
			if (noseName == "nose1")
			{
				m_nose = imread("res\\nose1.jpg");
			   mask_nose = imread("res\\nose1mask.jpg", 0);
			}
			else
			{
				m_nose = imread("res\\nose2.jpg");
				mask_nose = imread("res\\nose2mask.jpg", 0);
			}
			
			Mat mask;
			threshold(mask_nose, mask, 20, 255, THRESH_BINARY_INV);
			resize(mask, mask, rectnose.size());
			resize(m_nose, m_nose, rectnose.size());
			Mat img_roi = result(rectnose);
			m_nose.copyTo(img_roi, mask);
		}

		if ((m_poseEstimation.GetCheck() == BST_CHECKED) || (m_auto.GetCheck() == BST_CHECKED))
		{
		    char str[20];
			cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);
			// Project a 3D point (0, 0, 1000.0) onto the image plane.
			// We use this to draw a line sticking out of the nose
			projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);
			dxdy.x = nose_end_point2D[0].x - image_points[1].x;
			dxdy.y = nose_end_point2D[0].y - image_points[1].y;

/*			for (int i = 0; i < image_points.size(); i++)
			{
				circle(result, image_points[i], 3, Scalar(0, 0, 255), -1);
			}
			cv::line(result, image_points[1], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);
*/
			double alpha, beta;
			double z;
			z = sqrt(1000 * 1000 - ((dxdy.x*dxdy.x) + (dxdy.y*dxdy.y)));
			alpha = asin(dxdy.y / 1000.0);
			beta = atan(dxdy.x / z);
//			direction << frameNum << endl;
//			direction << alpha * 180 << '\t' << beta * 180 << endl;
/*			Mat rotation_matrix;
			Rodrigues(rotation_vector, rotation_matrix);
			double r11 = rotation_matrix.at<double>(0, 0);
			double r21 = rotation_matrix.at<double>(1, 0);
			double r31 = rotation_matrix.at<double>(2, 0);
			double r32 = rotation_matrix.at<double>(2, 1);
			double r33 = rotation_matrix.at<double>(2, 2);
			z = sqrt(r32*r32 + r33*r33);
			double alpha2, beta2, theta2;
			alpha2 = atan2(r32, r33);
			beta2 = atan2(-r31, z);
			theta2 = atan2(r21, r11);
	//		direction << alpha2 * 180 << '\t' << beta2 * 180 << '\t' << theta2 * 180 << endl;

			double alpha3, beta3, theta3;
			beta3 = -asin(r31);
			z = cos(beta3 * 180);
			alpha3 = atan2((r32 / z), (r33 / z));
			theta3 = atan2((r21 / z), (r11 / z));
	//		direction << alpha3 * 180 << '\t' << beta3 * 180 << '\t'<< theta3 * 180 << endl;
	*/

			if (alpha * 180 > 10)
				currentPos = 4;
			else if (beta * 180 < -25)
				currentPos = 2;
			else if (alpha * 180<-20)
				currentPos = 3;
			else if (beta * 180 > 25)
				currentPos = 1;
			else	
			  currentPos = 0;
			headPos.push_back(currentPos);
			if (headPos.size() > 5)
			{
				headPos.pop_front();
				tempPos = headPos;
				std::sort(tempPos.begin(), tempPos.end());
				currentPos = tempPos[2];
			}
			switch (currentPos)
			{
			    case(0):
			    {
					   sprintf_s(str, "Null");
					   break;
			    }
			   case(1):
			    {
						  sprintf_s(str, "Right");
						  if (m_auto.GetCheck() == BST_CHECKED)
							  rightFlag = 1;
						  break;
			     }
			   case(2):
			   {
						  sprintf_s(str, "Left");
						  if (m_auto.GetCheck() == BST_CHECKED)
						      leftFlag = 1;
						  break;
			   }
			   case(3):
			   {
						  sprintf_s(str, "Up");
						  if (m_auto.GetCheck() == BST_CHECKED)
						  {
							  upFlag = 1; rightFlag = 0; leftFlag = 0; downFlag = 0;
						  }
							  
						  break;
			   }
			   case(4):
			   {
						  sprintf_s(str, "Down");
						  if (m_auto.GetCheck() == BST_CHECKED)
							  downFlag = 1;
						  break;
			   }
			}
			Point p(10, result.rows - 40);
			putText(result, str, p, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 2, 8); 
		}

		if (m_lips.GetCheck() == BST_CHECKED)
		{
			Rect temp = boundingRect(plips);
			Rect rectlips;
			rectlips.y = temp.y - int(0.2 * temp.height);
			rectlips.height = int(temp.height * 1.2);
			rectlips.x = temp.x - int(0.2*temp.width); //int(0.5*leftEyeBox.width);
			rectlips.width = int(temp.width * 1.2);
			Mat img_lips, mask_lips;
			int nIndex = m_lipslist.GetCurSel();
			if (nIndex != CB_ERR)
			{
				m_lipslist.GetLBText(nIndex, lipsName);
			}
			else lipsName = "lips1";

			if (lipsName == "lips1")
			{
				img_lips = imread("res\\lips1.jpg");
				mask_lips = imread("res\\lips1mask.jpg", 0);
			}
			else if ((lipsName == "lips2"))
			{
				img_lips = imread("res\\lips2.jpg");
				mask_lips = imread("res\\lips2mask.jpg", 0);
			}
			else if ((lipsName == "lips3"))
			{
				img_lips = imread("res\\lips3.jpg");
				mask_lips = imread("res\\lips3mask.jpg", 0);
			}
			else if ((lipsName == "lips4"))
			{
				img_lips = imread("res\\lips4.jpg");
				mask_lips = imread("res\\lips4mask.jpg", 0);
			}
			else if ((lipsName == "mouth1"))
			{
				img_lips = imread("res\\mouth1.jpg");
				mask_lips = imread("res\\mouth1mask.jpg", 0);
			}
			else
			{
				img_lips = imread("res\\lips5.jpg");
				mask_lips = imread("res\\lips5mask.jpg", 0);
			}

			Mat mask;
			threshold(mask_lips, mask, 20, 255, THRESH_BINARY_INV);
			resize(mask, mask, rectlips.size());
			resize(img_lips, img_lips, rectlips.size());
			Mat img_roi = result(rectlips);
			Mat temp_img;
			img_roi.copyTo(temp_img);
			img_lips.copyTo(img_roi, mask);
			addWeighted(img_roi, 0.9, temp_img, 0.1, 0.0, img_roi);

		}

		if (m_brow.GetCheck() == BST_CHECKED)
		{
	/*		Rect leftEyeBox = boundingRect(lefteye);
			Rect rightEyeBox = boundingRect(righteye);
			Rect tempLeftEye, tempRightEye;
			tempLeftEye.y = leftEyeBox.y - 3 * leftEyeBox.height;
			tempLeftEye.height = leftEyeBox.height * 5;
			tempLeftEye.x = leftEyeBox.x - int(0.5*leftEyeBox.width); //int(0.5*leftEyeBox.width);
			tempLeftEye.width = int(leftEyeBox.width * 2);

			tempRightEye.y = rightEyeBox.y - 3 * rightEyeBox.height;
			tempRightEye.height = rightEyeBox.height * 5;
			tempRightEye.x = rightEyeBox.x;// int(0.5*rightEyeBox.width);// +int(0.5*rightEyeBox.width);
			tempRightEye.width = (rightEyeBox.width * 2);

			Mat m_lefteye, mask_lefteye, m_righteye, mask_righteye;

			int nIndex = m_eyelist.GetCurSel();
			if (nIndex != CB_ERR)
			{
				m_eyelist.GetLBText(nIndex, eyeName);
			}
			else eyeName = "eye1";

			if (eyeName == "eye1")
			{
				m_lefteye = imread("res\\left_eye1.jpg");
				mask_lefteye = imread("res\\left_eye1mask.jpg", 0);
				m_righteye = imread("res\\right_eye1.jpg");
				mask_righteye = imread("res\\right_eye1mask.jpg", 0);
			}
			else if (eyeName == "eye2")
			{
				m_lefteye = imread("res\\left_eye2.jpg");
				mask_lefteye = imread("res\\left_eye2mask.jpg", 0);
				m_righteye = imread("res\\right_eye2.jpg");
				mask_righteye = imread("res\\right_eye2mask.jpg", 0);
			}
			else if (eyeName == "eye3")
			{
				m_lefteye = imread("res\\left_eye3.jpg");
				mask_lefteye = imread("res\\left_eye3mask.jpg", 0);
				m_righteye = imread("res\\right_eye3.jpg");
				mask_righteye = imread("res\\right_eye3mask.jpg", 0);
			}
			else
			{
				m_lefteye = imread("res\\left_eye4.jpg");
				mask_lefteye = imread("res\\left_eye4mask.jpg", 0);
				m_righteye = imread("res\\right_eye4.jpg");
				mask_righteye = imread("res\\right_eye4mask.jpg", 0);
			}
			Mat mask_left, mask_right;
			threshold(mask_lefteye, mask_left, 20, 255, THRESH_BINARY_INV);
			threshold(mask_righteye, mask_right, 20, 255, THRESH_BINARY_INV);
			resize(mask_left, mask_left, tempLeftEye.size());
			resize(mask_right, mask_right, tempRightEye.size());
			resize(m_lefteye, m_lefteye, tempLeftEye.size());
			resize(m_righteye, m_righteye, tempRightEye.size());
			Mat img_roi_lefteye = result(tempLeftEye);
			Mat img_roi_righteye = result(tempRightEye);
			m_lefteye.copyTo(img_roi_lefteye, mask_left);
			m_righteye.copyTo(img_roi_righteye, mask_right);
	*/
		}

		if (m_faceChange.GetCheck() == BST_CHECKED && !OneFrameProcess)                                  //换脸
		{
			if (facesNum == 2)
			{
				Mat H = cv::findHomography(faceOne, faceTwo);
				Mat H2 = findHomography(faceTwo, faceOne);
				Mat test1(img_original.size(), CV_8UC1, Scalar(0, 0, 0));
				Mat test2(img_original.size(), CV_8UC1, Scalar(0, 0, 0));
				Mat test3(img_original.size(), CV_8UC1, Scalar(0, 0, 0));
				Mat test4(img_original.size(), CV_8UC1, Scalar(0, 0, 0));
				Mat test5;
				img_original.copyTo(test5);//(img.size(),img.type(), Scalar(0,0,0));
				Mat test6;
				img_original.copyTo(test6);//(img.size(),img.type(), Scalar(0,0,0));
				Mat mask_roi1 = test3(faces[0]);
				Mat mask_roi2 = test4(faces[1]);

				Mat roi1 = test5(faces[0]);              //用于存储用来合并的Face 
				Mat roi2 = test6(faces[1]);

				mask_roi1 = Scalar(255, 255, 255);
				mask_roi2 = Scalar(255, 255, 255);

				                                                                                                        //hullOne hullTwo 函数外定义
				convexHull(faceOnei, hullOne); convexHull(faceTwoi, hullTwo);
				fillConvexPoly(test1, hullOne, Scalar(255, 255, 255));
				fillConvexPoly(test2, hullTwo, Scalar(255, 255, 255));
		
				Mat face1 = img_original(faces[0]);
				Mat face2 = img_original(faces[1]);
				Mat tempFace1, tempFace2;
				//Mat resultFace1,resultFace2;
				face1.copyTo(tempFace1); face2.copyTo(tempFace2);

				IplImage* source;   source = &IplImage(tempFace2);//=cvLoadImage("face2.jpg", CV_LOAD_IMAGE_COLOR);
				IplImage* dst;     dst = &IplImage(tempFace1);              //  =cvLoadImage("face1.jpg", CV_LOAD_IMAGE_COLOR);
				IplImage* dstlab = cvCreateImage(cvGetSize(dst), dst->depth, dst->nChannels);
				IplImage* res = cvCreateImage(cvGetSize(dst), dst->depth, dst->nChannels);
				dstlab = cvCloneImage(dst);
				CvScalar avg_src, avg_dst, std_src, std_dst;
				cvAvgSdv(source, &avg_src, &std_src);
				cvAvgSdv(dstlab, &avg_dst, &std_dst);
				dstlab = Transform(dstlab, avg_src, avg_dst, std_src, std_dst);
				Mat temp(dstlab, 0);
				res = cvCloneImage(dstlab);
				Mat resultFace1(res, 0);
				//	resultFace1.copyTo(face1);  
				resultFace1.copyTo(roi1);
				Mat_<Vec3f> lc; img_original.convertTo(lc, CV_32F, 1.0 / 255.0);//Vec3f表示有三个通道，即 l[row][column][depth]
				Mat_<Vec3f> rc; test5.convertTo(rc, CV_32F, 1.0 / 255.0);
				Mat_<float> mc;
				test3.convertTo(mc, CV_32F, 1.0 / 255.0);
				Mat_<Vec3f> blend1c = LaplacianBlend(rc, lc, mc);

				//	cvReleaseImage(&source);
				//	cvReleaseImage(&dst);
				cvReleaseImage(&dstlab);
				cvReleaseImage(&res);

				source = &IplImage(tempFace1);         //=cvLoadImage("face2.jpg", CV_LOAD_IMAGE_COLOR);
				dst = &IplImage(tempFace2);              //  =cvLoadImage("face1.jpg", CV_LOAD_IMAGE_COLOR);
				dstlab = cvCloneImage(dst);
				cvAvgSdv(source, &avg_src, &std_src);
				cvAvgSdv(dstlab, &avg_dst, &std_dst);
				dstlab = Transform(dstlab, avg_src, avg_dst, std_src, std_dst);
				res = cvCloneImage(dstlab);
				Mat resultFace2(res, 0);
				//	cvReleaseImage(&source);
				//	cvReleaseImage(&dst);
				resultFace2.copyTo(roi2);
				//颜色转换结果
				blend1c.convertTo(lc, CV_32F, 1.0 / 255.0);
				test6.convertTo(rc, CV_32F, 1.0 / 255.0);
				test4.convertTo(mc, CV_32F, 1.0 / 255.0);
				Mat_<Vec3f> blend2c = LaplacianBlend(rc, blend1c, mc);
				//	resultFace2.copyTo(face2);
				cvReleaseImage(&dstlab);
				cvReleaseImage(&res);
				Mat resultImg;
				blend2c.convertTo(resultImg, CV_8UC3, 255);
				//	resultImg.copyTo(img);
				Mat warpIm1, warpMask;
				Mat warpIm2, warpMask2;
				warpPerspective(img_original, warpIm1, H, img_original.size());
				warpPerspective(test1, warpMask, H, img_original.size());
				warpPerspective(img_original, warpIm2, H2, img_original.size());
				warpPerspective(test2, warpMask2, H2, img_original.size());

				Mat_<Vec3f> l; img_original.convertTo(l, CV_32F, 1.0 / 255.0);//Vec3f表示有三个通道，即 l[row][column][depth]
				Mat_<Vec3f> r; warpIm1.convertTo(r, CV_32F, 1.0 / 255.0);
				Mat_<float> m;
				warpMask.convertTo(m, CV_32F, 1.0 / 255.0);
				Mat_<Vec3f> blend1 = LaplacianBlend(r, l, m);

				blend1.convertTo(l, CV_32F, 1.0 / 255.0);
				warpIm2.convertTo(r, CV_32F, 1.0 / 255.0);
				warpMask2.convertTo(m, CV_32F, 1.0 / 255.0);
				Mat_<Vec3f> blend2 = LaplacianBlend(r, blend1, m);
				blend2.convertTo(resultImg, CV_8UC3, 255);
				//	resultImg.copyTo(img);
				resultImg.copyTo(result);
			}
			
		}
		result.copyTo(img_original_aux);
		result.copyTo(img_result);

		if (!img_original_aux.empty())
			imshow("INPUT", img_original_aux);
		strFrameNum.Format(L"%d", frameNum);
		m_frameNum.SetWindowTextW(strFrameNum);
		if (OneFrameProcess)
		{
			if (create)
			{
				create = false;
				namedWindow("result", 1);
				
			}
	
			waitKey(1);
			imshow("LABEL", img_original);
			imshow("result", result);
			m_log.SetWindowTextW(L"Suspending...");
		}

		if (destroy)
		{
			destroy = false;
			destroyWindow("LABEL");
		}

		if (!img_original.empty())
		{
			if (m_saveVideo.GetCheck() == BST_CHECKED)
			{
				if (frameNum == 1)
				{
					Size S(img_original.cols, img_original.rows);
					outputVideo.open(resultVideoName, CV_FOURCC('X', 'V', 'I', 'D'), 15.0, S);
				}
				outputVideo << result;
			}
			
		}

		if (stopped)
		{
			m_log.SetWindowTextW(L"Stopping...");
			break;
		}
		temp2 = static_cast<double>(getTickCount());
		frame_time = int((1000-((temp2 - temp1) / cv::getTickFrequency()))/fps);
		waitKey(2*frame_time);
		if (!OneFrameProcess)
		{
			leftbrow.clear();
			rightbrow.clear();
			uplip.clear();
			downlip.clear();
			lefteye.clear();
			righteye.clear();
			pnose.clear();
			nose_end_point2D.clear();
			image_points.clear();
			plips.clear();
			faceOne.clear();
			faceTwo.clear();
			faceOnei.clear();
			faceTwoi.clear();
			faces.clear();
			keypts.clear();
			attributes.clear();
			quality.clear();
			hullOne.clear();
			hullTwo.clear();
		}
	} while (true);
	if (frameFile.is_open())  frameFile.close();
	if (keyptFile.is_open())  keyptFile.close();
	if (attriFile.is_open())  attriFile.close();
	if (qualityFile.is_open())  qualityFile.close();
	if (direction.is_open()) direction.close();
	if (outputVideo.isOpened()) outputVideo.release();

	if (stopped)	{ m_log.SetWindowTextW(L"Stopped!"); }
	else	{ m_log.SetWindowTextW(L"Finished!"); }
	started = false;
	AfxMessageBox(L"Thread is finished!");
}

void CmyvideoDlg::StopThread()
{
	DWORD exit_code = NULL;

	if (thread != NULL)
	{
		GetExitCodeThread(thread->m_hThread, &exit_code);

		if (exit_code == STILL_ACTIVE)
		{
			::TerminateThread(thread->m_hThread, 0);
			CloseHandle(thread->m_hThread);
		}

		thread->m_hThread = NULL;
		thread = NULL;
	}
}

void CmyvideoDlg::OnBnClickedStop()
{
	if (started)
	{
		started = false;
		stopped = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Stop Error!");
		return;
	}
}


void CmyvideoDlg::OnBnClickedPlay()
{
	if (started&&OneFrameProcess == 1)
	{
		OneFrameProcess = 0;
		destroy = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Play Error");
		return;
	}
}


void CmyvideoDlg::OnBnClickedSuspend()
{
	if (started&&OneFrameProcess == 0)
	{
		OneFrameProcess = 1;
		namedWindow("LABEL", 1);
		createTrackbar("眉毛：", "LABEL", &bsize, 40);
		createTrackbar("嘴唇：", "LABEL", &hong, 40);
		create = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Suspend Error!");
		return;
	}
}



void CmyvideoDlg::OnBnClickedMiaomei()
{
	if (brow == false)
	{
		brow = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Already Brow!");
		return;
	}
}


void CmyvideoDlg::OnBnClickedTuchu()
{
	if (lips == false)
	{
		lips = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Already Lips!");
		return;
	}
}


void CmyvideoDlg::OnBnClickedEye()
{
	if (eyes == false)
	{
		eyes = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Already Eyes!");
		return;
	}
}


void CmyvideoDlg::OnBnClickedNose()
{
	if (nose == false)
	{
		nose = true;
		return;
	}
	else
	{
		AfxMessageBox(L"Already Nose!");
		return;
	}
}


void CmyvideoDlg::OnBnClickedButton1()
{
	imwrite("result.jpg", img_result);
}
