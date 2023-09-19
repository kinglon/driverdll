// PrinterSampleDlg.h : header file
//

#if !defined(AFX_PRINTERSAMPLEDLG_H__65772F5F_3BE9_4001_B7D0_15400EF42662__INCLUDED_)
#define AFX_PRINTERSAMPLEDLG_H__65772F5F_3BE9_4001_B7D0_15400EF42662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OposPrinter\\coptr.h"


#define OPOS_SUCCESS	0
#define PTR_S_RECEIPT	2
#define ESC				27

#define PTR_BM_LEFT		-1
#define PTR_BM_CENTER	-2
#define PTR_BM_RIGHT	-3

#define PTR_L_TOP		1
#define PTR_L_BOTTOM	2

#define	WM_PRINTTICKET	WM_USER+101


/////////////////////////////////////////////////////////////////////////////
// CPrinterSampleDlg dialog

class CPrinterSampleDlg : public CDialog
{
// Construction
public:
	CPrinterSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPrinterSampleDlg)
	enum { IDD = IDD_PRINTERSAMPLE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrinterSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPrinterSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnPrintTicket(WPARAM wParam,LPARAM lParam);
	afx_msg void On_BUTTON_Test();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CCoptr	*m_pPrinter;

public:
	bool Printer_Init();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTERSAMPLEDLG_H__65772F5F_3BE9_4001_B7D0_15400EF42662__INCLUDED_)
