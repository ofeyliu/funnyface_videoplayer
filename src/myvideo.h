
// myvideo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CmyvideoApp: 
// �йش����ʵ�֣������ myvideo.cpp
//

class CmyvideoApp : public CWinApp
{
public:
	CmyvideoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CmyvideoApp theApp;