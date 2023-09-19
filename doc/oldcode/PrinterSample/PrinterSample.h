// PrinterSample.h : main header file for the PRINTERSAMPLE application
//

#if !defined(AFX_PRINTERSAMPLE_H__D20976FB_D672_418C_9A04_025E081108EA__INCLUDED_)
#define AFX_PRINTERSAMPLE_H__D20976FB_D672_418C_9A04_025E081108EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrinterSampleApp:
// See PrinterSample.cpp for the implementation of this class
//

class CPrinterSampleApp : public CWinApp
{
public:
	CPrinterSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinterSampleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPrinterSampleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTERSAMPLE_H__D20976FB_D672_418C_9A04_025E081108EA__INCLUDED_)
