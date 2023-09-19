// QueueDllSampleDlg.h : header file
//

#if !defined(AFX_QUEUEDLLSAMPLEDLG_H__5C97FDD4_5600_48DB_92BF_2614BD9EA564__INCLUDED_)
#define AFX_QUEUEDLLSAMPLEDLG_H__5C97FDD4_5600_48DB_92BF_2614BD9EA564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQueueDllSampleDlg dialog

class CQueueDllSampleDlg : public CDialog
{
// Construction
public:
	CQueueDllSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQueueDllSampleDlg)
	enum { IDD = IDD_QUEUEDLLSAMPLE_DIALOG };
	CListCtrl	m_listLog;
	int		m_iWsComNum;
	CString	m_strWsInfo;
	CString	m_strWsStyle;
	int		m_iWsWinNum;
	int		m_iPComNum;
	CString	m_strPPicFile;
	CString	m_strPStyle;
	CString	m_strPTextInfo;
	CString	m_strCsInfo;
	int		m_iCsBlockNum;
	int		m_iCsComNum;
	CString	m_strCsStyle;
	int		m_iKComNum;
	CString	m_strKInfo;
	int		m_iKWinNum;
	int		m_iMcComNum;
	int		m_iIcComNum;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueueDllSampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQueueDllSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTTON_WS_Open();
	afx_msg void OnBUTTON_LoadDLL();
	afx_msg void OnBUTTON_WS_ShowInfo();
	afx_msg void OnBUTTON_WS_Close();
	afx_msg void OnBUTTON_P_Open();
	afx_msg void OnBUTTON_P_FillPicInfo();
	afx_msg void OnBUTTON_P_FillTextInfo();
	afx_msg void OnBUTTON_P_Print();
	afx_msg void OnBUTTON_P_Close();
	afx_msg void OnBUTTON_CS_Open();
	afx_msg void OnBUTTON_CS_ShowInfo();
	afx_msg void OnBUTTON_CS_Close();
	afx_msg void OnBUTTON_K_Open();
	afx_msg void OnBUTTON_K_ShowInfo();
	afx_msg void OnBUTTON_K_SetCallBack();
	afx_msg void OnBUTTON_K_Close();
	afx_msg void OnBUTTON_MC_Open();
	afx_msg void OnBUTTON_MC_SetCallBack();
	afx_msg void OnBUTTON_MC_Close();
	afx_msg void OnBUTTON_IC_Open();
	afx_msg void OnBUTTON_IC_SetCallBack();
	afx_msg void OnBUTTON_IC_Close();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	void Ctrl_ShowDebugInfo(char *pLog);
	void Ctrl_Init();

public:



};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUEUEDLLSAMPLEDLG_H__5C97FDD4_5600_48DB_92BF_2614BD9EA564__INCLUDED_)
