// QueueDllSample.h : main header file for the QUEUEDLLSAMPLE application
//

#if !defined(AFX_QUEUEDLLSAMPLE_H__A3B86A5F_7CEE_476A_9C9C_3ABD283FABED__INCLUDED_)
#define AFX_QUEUEDLLSAMPLE_H__A3B86A5F_7CEE_476A_9C9C_3ABD283FABED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQueueDllSampleApp:
// See QueueDllSample.cpp for the implementation of this class
//

class CQueueDllSampleApp : public CWinApp
{
public:
	CQueueDllSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueueDllSampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQueueDllSampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUEUEDLLSAMPLE_H__A3B86A5F_7CEE_476A_9C9C_3ABD283FABED__INCLUDED_)
