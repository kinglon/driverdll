
// PrintExe.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPrintExeApp: 
// �йش����ʵ�֣������ PrintExe.cpp
//

class CPrintExeApp : public CWinApp
{
public:
	CPrintExeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPrintExeApp theApp;