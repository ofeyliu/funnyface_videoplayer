
// myvideoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxeditbrowsectrl.h"
#include "afxwin.h"

//#include <opencv/cvaux.h>
#include <opencv/highgui.h>
#include "stdafx.h" 
#include <fstream>
#include <string>// ע����<string>������<string.h>����.h����C�����е�ͷ�ļ�
#include <opencv2\video\video.hpp>
using  namespace std;
using namespace cv;


// CmyvideoDlg �Ի���
class CmyvideoDlg : public CDialogEx
{
// ����
public:
	CmyvideoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYVIDEO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
//
private:                          
	bool started;
	bool stopped;
	bool create;
	bool destroy;
	bool brow;
	bool lips;
	bool nose;
	bool eyes;

	CString filePath;
	string str_filePath;
	CWinThread* thread;
	VideoCapture capture;
	double fps;
	VideoWriter outputVideo;
	int frameNum;
	int OneFrameProcess;
	Mat img_original;
	Mat img_result;

	CString noseName;
	CString eyeName;
	CString browName;
	CString lipsName;


public:
	CMFCEditBrowseCtrl m_inputVideo;
	CStatic m_log;
	
	bool getInputVideoFilePath();
	afx_msg void OnBnClickedStart();

	afx_msg void ThreadProcess();
	static DWORD Thread(LPVOID *x);
	afx_msg void StopThread();
	
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedSuspend();
	CButton m_faceDetect;
	CButton m_faceAttribute;
	CButton m_faceQuality;
//	afx_msg void OnBnClickedQuality();
//	afx_msg void OnBnClickedAttribute();
	afx_msg void OnBnClickedMiaomei();
	afx_msg void OnBnClickedTuchu();
	afx_msg void OnBnClickedEye();
	afx_msg void OnBnClickedNose();
	CComboBox m_noselist;
	CComboBox m_eyelist;
	CComboBox m_browlist;
	CComboBox m_lipslist;

	void CmyvideoDlg::addnoselist();
	void CmyvideoDlg::addeyelist();
	void CmyvideoDlg::addbrowlist();
	void CmyvideoDlg::addlipslist();

	CButton m_poseEstimation;
	CButton m_brow;
	CButton m_lips;
	
	afx_msg void OnBnClickedButton1();
	CButton m_saveVideo;
	CButton m_auto;
	CStatic m_frameNum;
	CButton m_faceChange;

	std::vector<cv::Point2d> image_points;
	std::vector<cv::Point3d> model_points;
	vector<Point3d> nose_end_point3D;
	vector<Point2d> nose_end_point2D;

	vector<Point2f> faceOne;
	vector<Point2f> faceTwo;
	vector<Point> faceOnei;
	vector<Point> faceTwoi;

	vector<Rect> faces;
	vector<Point> keypts;
	
	vector<float> quality;

	vector<Point> hullOne; vector<Point> hullTwo;
};
