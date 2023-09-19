// QueueDllSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QueueDllSample.h"
#include "QueueDllSampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQueueDllSampleDlg dialog

CQueueDllSampleDlg::CQueueDllSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueueDllSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQueueDllSampleDlg)
	m_iWsComNum = 1;
	m_strWsInfo = _T("A-1234");
	m_strWsStyle = _T("暂无");
	m_iWsWinNum = 1;
	m_iPComNum = 0;
	m_strPPicFile = _T("D:\\QmDriver\\Config\\Logo.bmp");
	m_strPStyle = _T("1|1|1");
	m_strPTextInfo = _T("北京农商银行");
	m_strCsInfo = _T("A-1234 -> [1]");
	m_iCsBlockNum = 1;
	m_iCsComNum = 2;
	m_strCsStyle = _T("暂无");
	m_iKComNum = 3;
	m_strKInfo = _T("1234");
	m_iKWinNum = 1;
	m_iMcComNum = 4;
	m_iIcComNum = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQueueDllSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueueDllSampleDlg)
	DDX_Control(pDX, IDC_LIST_Log, m_listLog);
	DDX_Text(pDX, IDC_EDIT_WS_ComNum, m_iWsComNum);
	DDX_Text(pDX, IDC_EDIT_WS_Info, m_strWsInfo);
	DDX_Text(pDX, IDC_EDIT_WS_Style, m_strWsStyle);
	DDX_Text(pDX, IDC_EDIT_WS_WinNum, m_iWsWinNum);
	DDX_Text(pDX, IDC_EDIT_P_ComNum, m_iPComNum);
	DDX_Text(pDX, IDC_EDIT_P_PicFile, m_strPPicFile);
	DDX_Text(pDX, IDC_EDIT_P_Style, m_strPStyle);
	DDX_Text(pDX, IDC_EDIT_P_TextInfo, m_strPTextInfo);
	DDX_Text(pDX, IDC_EDIT_CS_Info, m_strCsInfo);
	DDX_Text(pDX, IDC_EDIT_CS_BlockNum, m_iCsBlockNum);
	DDX_Text(pDX, IDC_EDIT_CS_ComNum, m_iCsComNum);
	DDX_Text(pDX, IDC_EDIT_CS_Style, m_strCsStyle);
	DDX_Text(pDX, IDC_EDIT_K_ComNum, m_iKComNum);
	DDX_Text(pDX, IDC_EDIT_K_Info, m_strKInfo);
	DDX_Text(pDX, IDC_EDIT_K_WinNum, m_iKWinNum);
	DDX_Text(pDX, IDC_EDIT_MC_ComNum, m_iMcComNum);
	DDX_Text(pDX, IDC_EDIT_IC_ComNum, m_iIcComNum);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQueueDllSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CQueueDllSampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_WS_Open, OnBUTTON_WS_Open)
	ON_BN_CLICKED(IDC_BUTTON_LoadDLL, OnBUTTON_LoadDLL)
	ON_BN_CLICKED(IDC_BUTTON_WS_ShowInfo, OnBUTTON_WS_ShowInfo)
	ON_BN_CLICKED(IDC_BUTTON_WS_Close, OnBUTTON_WS_Close)
	ON_BN_CLICKED(IDC_BUTTON_P_Open, OnBUTTON_P_Open)
	ON_BN_CLICKED(IDC_BUTTON_P_FillPicInfo, OnBUTTON_P_FillPicInfo)
	ON_BN_CLICKED(IDC_BUTTON_P_FillTextInfo, OnBUTTON_P_FillTextInfo)
	ON_BN_CLICKED(IDC_BUTTON_P_Print, OnBUTTON_P_Print)
	ON_BN_CLICKED(IDC_BUTTON_P_Close, OnBUTTON_P_Close)
	ON_BN_CLICKED(IDC_BUTTON_CS_Open, OnBUTTON_CS_Open)
	ON_BN_CLICKED(IDC_BUTTON_CS_ShowInfo, OnBUTTON_CS_ShowInfo)
	ON_BN_CLICKED(IDC_BUTTON_CS_Close, OnBUTTON_CS_Close)
	ON_BN_CLICKED(IDC_BUTTON_K_Open, OnBUTTON_K_Open)
	ON_BN_CLICKED(IDC_BUTTON_K_ShowInfo, OnBUTTON_K_ShowInfo)
	ON_BN_CLICKED(IDC_BUTTON_K_SetCallBack, OnBUTTON_K_SetCallBack)
	ON_BN_CLICKED(IDC_BUTTON_K_Close, OnBUTTON_K_Close)
	ON_BN_CLICKED(IDC_BUTTON_MC_Open, OnBUTTON_MC_Open)
	ON_BN_CLICKED(IDC_BUTTON_MC_SetCallBack, OnBUTTON_MC_SetCallBack)
	ON_BN_CLICKED(IDC_BUTTON_MC_Close, OnBUTTON_MC_Close)
	ON_BN_CLICKED(IDC_BUTTON_IC_Open, OnBUTTON_IC_Open)
	ON_BN_CLICKED(IDC_BUTTON_IC_SetCallBack, OnBUTTON_IC_SetCallBack)
	ON_BN_CLICKED(IDC_BUTTON_IC_Close, OnBUTTON_IC_Close)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueueDllSampleDlg message handlers

BOOL CQueueDllSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	Ctrl_Init();

	Ctrl_ShowDebugInfo("程序启动");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQueueDllSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQueueDllSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQueueDllSampleDlg::Ctrl_Init()
{
	m_listLog.InsertColumn(0,	"日期时间",	LVCFMT_LEFT,	120);
	m_listLog.InsertColumn(1,	"事件",		LVCFMT_LEFT,	300);

	m_listLog.SetExtendedStyle (m_listLog.GetExtendedStyle()  | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

}

void CQueueDllSampleDlg::Ctrl_ShowDebugInfo(char *pLog)
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	char cDateTime[50];
	sprintf(cDateTime, "%4.4ld-%2.2ld-%2.2ld %2.2ld:%2.2ld:%2.2ld", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		
	m_listLog.InsertItem (0, cDateTime);
	m_listLog.SetItemText(0, 1, pLog);
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//窗口条屏模块

//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pStyle, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *WS_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *WS_SHOWINFO)(int iWinNum, char *pInfo, char *pStyle, char *pMsg);
typedef int (_stdcall *WS_CLOSE)(char *pMsg);

WS_OPEN		ws_open		= NULL;
WS_SHOWINFO ws_showinfo	= NULL;
WS_CLOSE	ws_close	= NULL;


void CQueueDllSampleDlg::OnBUTTON_WS_Open() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = ws_open(this->m_iWsComNum, cMsg);
	
	CString strLog;
	strLog.Format("[WindowScreen.dll]-[Open]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}


void CQueueDllSampleDlg::OnBUTTON_WS_ShowInfo() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = ws_showinfo(this->m_iWsWinNum, this->m_strWsInfo.GetBuffer(0), this->m_strWsStyle.GetBuffer(0), cMsg);
	
	CString strLog;
	strLog.Format("[WindowScreen.dll]-[ShowInfo]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

void CQueueDllSampleDlg::OnBUTTON_WS_Close() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = ws_close(cMsg);
	
	CString strLog;
	strLog.Format("[WindowScreen.dll]-[Close]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//打印机模块

//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall FillPicInfo(char *pPicFile, char *pMsg)
//int _declspec(dllexport) __stdcall FillTextInfo(char *pTextInfo, char *pStyle, char *pMsg)
//int _declspec(dllexport) __stdcall Print(char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *P_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *P_FILLPICINFO)(char *pPicFile, char *pMsg);
typedef int (_stdcall *P_FILLTEXTINFO)(char *pTextInfo, char *pStyle, char *pMsg);
typedef int (_stdcall *P_PRINT)(char *pMsg);
typedef int (_stdcall *P_CLOSE)(char *pMsg);

P_OPEN			p_open			= NULL;
P_FILLPICINFO	p_fillpicinfo	= NULL;
P_FILLTEXTINFO	p_filltextinfo	= NULL;
P_PRINT			p_print			= NULL;
P_CLOSE			p_close			= NULL;

void CQueueDllSampleDlg::OnBUTTON_P_Open() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = p_open(this->m_iPComNum, cMsg);
	
	CString strLog;
	strLog.Format("[Printer.dll]-[Open]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}

void CQueueDllSampleDlg::OnBUTTON_P_FillPicInfo() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = p_fillpicinfo(this->m_strPPicFile.GetBuffer(0), cMsg);
	
	CString strLog;
	strLog.Format("[Printer.dll]-[FillPicInfo]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
	
}

void CQueueDllSampleDlg::OnBUTTON_P_FillTextInfo() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = p_filltextinfo(this->m_strPTextInfo.GetBuffer(0), this->m_strPStyle.GetBuffer(0), cMsg);
	
	CString strLog;
	strLog.Format("[Printer.dll]-[FillTextInfo]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}

void CQueueDllSampleDlg::OnBUTTON_P_Print() 
{	
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = p_print(cMsg);
	
	CString strLog;
	strLog.Format("[Printer.dll]-[Print]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}

void CQueueDllSampleDlg::OnBUTTON_P_Close() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = p_close(cMsg);
	
	CString strLog;
	strLog.Format("[Printer.dll]-[Close]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//集中显示屏模块

//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall ShowInfo(int iBlockNum, char *pInfo, char *pStyle, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *CS_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *CS_SHOWINFO)(int iBlockNum, char *pInfo, char *pStyle, char *pMsg);
typedef int (_stdcall *CS_CLOSE)(char *pMsg);

CS_OPEN		cs_open		= NULL;
CS_SHOWINFO cs_showinfo	= NULL;
CS_CLOSE	cs_close	= NULL;


void CQueueDllSampleDlg::OnBUTTON_CS_Open() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = cs_open(this->m_iCsComNum, cMsg);
	
	CString strLog;
	strLog.Format("[CenterScreen.dll]-[Open]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}

void CQueueDllSampleDlg::OnBUTTON_CS_ShowInfo() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = cs_showinfo(this->m_iCsBlockNum, this->m_strCsInfo.GetBuffer(0), this->m_strCsStyle.GetBuffer(0), cMsg);
	
	CString strLog;
	strLog.Format("[CenterScreen.dll]-[ShowInfo]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

void CQueueDllSampleDlg::OnBUTTON_CS_Close() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = cs_close(cMsg);
	
	CString strLog;
	strLog.Format("[CenterScreen.dll]-[Close]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//键盘模块

//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pMsg)
//int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *K_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *K_SHOWINFO)(int iWinNum, char *pInfo, char *pMsg);
typedef int (_stdcall *K_SETCALLBACK)(void *pCallBack, char *pMsg);
typedef int (_stdcall *K_CLOSE)(char *pMsg);

K_OPEN			k_open			= NULL;
K_SHOWINFO		k_showinfo		= NULL;
K_SETCALLBACK	k_setcallback	= NULL;
K_CLOSE			k_close			= NULL;

int _stdcall Key_CallBack(int iWinNum, int iAct)
{
	char cMsg[200] = {0};
	sprintf(cMsg, "iWinNum=%d iAct=%d", iWinNum, iAct);
	::MessageBox(NULL, cMsg, "键盘回调函数", MB_OK);
	return 1;
}


void CQueueDllSampleDlg::OnBUTTON_K_Open() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = k_open(this->m_iKComNum, cMsg);
	
	CString strLog;
	strLog.Format("[Key.dll]-[Open]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}

void CQueueDllSampleDlg::OnBUTTON_K_ShowInfo() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = k_showinfo(this->m_iKWinNum, this->m_strKInfo.GetBuffer(0), cMsg);
	
	CString strLog;
	strLog.Format("[Key.dll]-[ShowInfo]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

void CQueueDllSampleDlg::OnBUTTON_K_SetCallBack() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	//int iRet = k_setcallback((void*)0x11111111, cMsg);
	int iRet = k_setcallback((void*)Key_CallBack, cMsg);
	
	CString strLog;
	strLog.Format("[Key.dll]-[SetCallBack]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

void CQueueDllSampleDlg::OnBUTTON_K_Close() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = k_close(cMsg);
	
	CString strLog;
	strLog.Format("[Key.dll]-[Close]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//磁条刷卡器模块

//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *MC_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *MC_SETCALLBACK)(void *pCallBack, char *pMsg);
typedef int (_stdcall *MC_CLOSE)(char *pMsg);

MC_OPEN			mc_open			= NULL;
MC_SETCALLBACK	mc_setcallback	= NULL;
MC_CLOSE		mc_close		= NULL;


int _stdcall Mn_CallBack(char *pCardInfo)
{
	char cMsg[1000] = {0};
	sprintf(cMsg, "pCardInfo = %s", pCardInfo);
	::MessageBox(NULL, cMsg, "磁条卡回调函数", MB_OK);
	return 1;
}

void CQueueDllSampleDlg::OnBUTTON_MC_Open() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = mc_open(this->m_iMcComNum, cMsg);
	
	CString strLog;
	strLog.Format("[MnCard.dll]-[Open]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}



void CQueueDllSampleDlg::OnBUTTON_MC_SetCallBack() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	//int iRet = mc_setcallback((void*)0x22222222, cMsg);
	int iRet = mc_setcallback((void*)Mn_CallBack, cMsg);
	
	CString strLog;
	strLog.Format("[MnCard.dll]-[SetCallBack]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

void CQueueDllSampleDlg::OnBUTTON_MC_Close() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = mc_close(cMsg);
	
	CString strLog;
	strLog.Format("[MnCard.dll]-[Close]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//IC刷卡器模块

//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *IC_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *IC_SETCALLBACK)(void *pCallBack, char *pMsg);
typedef int (_stdcall *IC_CLOSE)(char *pMsg);

IC_OPEN			ic_open			= NULL;
IC_SETCALLBACK	ic_setcallback	= NULL;
IC_CLOSE		ic_close		= NULL;



int _stdcall Ic_CallBack(char *pCardInfo)
{
	char cMsg[1000] = {0};
	sprintf(cMsg, "pCardInfo = %s", pCardInfo);
	::MessageBox(NULL, cMsg, "IC卡回调函数", MB_OK);
	return 1;
}

void CQueueDllSampleDlg::OnBUTTON_IC_Open() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = ic_open(this->m_iIcComNum, cMsg);
	
	CString strLog;
	strLog.Format("[IcCard.dll]-[Open]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));
}

void CQueueDllSampleDlg::OnBUTTON_IC_SetCallBack() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	//int iRet = ic_setcallback((void*)0x33333333, cMsg);
	int iRet = ic_setcallback((void*)Ic_CallBack, cMsg);
	
	CString strLog;
	strLog.Format("[IcCard.dll]-[SetCallBack]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

void CQueueDllSampleDlg::OnBUTTON_IC_Close() 
{
	UpdateData(TRUE);
	char cMsg[1024] = {0};

	// TODO: Add your control notification handler code here

	int iRet = ic_close(cMsg);
	
	CString strLog;
	strLog.Format("[IcCard.dll]-[Close]-[%d]-[%s]", iRet, cMsg);
	this->Ctrl_ShowDebugInfo(strLog.GetBuffer(0));	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//加载DLL
void CQueueDllSampleDlg::OnBUTTON_LoadDLL() 
{
	// TODO: Add your control notification handler code here

	//1 窗口条屏模块
	HINSTANCE hInstWS = ::LoadLibrary("WindowScreen.dll");
	if(hInstWS)
	{
		this->Ctrl_ShowDebugInfo("加载[WindowScreen.dll]成功");

		ws_open = (WS_OPEN) GetProcAddress(hInstWS, "Open");
		if(ws_open)	{	this->Ctrl_ShowDebugInfo("从[WindowScreen.dll]获取接口[Open]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[WindowScreen.dll]获取接口[Open]失败");	}

		ws_showinfo = (WS_SHOWINFO) GetProcAddress(hInstWS, "ShowInfo");
		if(ws_showinfo)	{	this->Ctrl_ShowDebugInfo("从[WindowScreen.dll]获取接口[ShowInfo]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[WindowScreen.dll]获取接口[ShowInfo]失败");	}

		ws_close = (WS_CLOSE) GetProcAddress(hInstWS, "Close");
		if(ws_close)	{	this->Ctrl_ShowDebugInfo("从[WindowScreen.dll]获取接口[Close]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[WindowScreen.dll]获取接口[Close]失败");	}
	}
	else
	{
		this->Ctrl_ShowDebugInfo("加载[WindowScreen.dll]失败");
	}	
	

	/*
//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall FillPicInfo(char *pPicFile, char *pMsg)
//int _declspec(dllexport) __stdcall FillTextInfo(char *pTextInfo, char *pStyle, char *pMsg)
//int _declspec(dllexport) __stdcall Print(char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *P_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *P_FILLPICINFO)(char *pPicFile, char *pMsg);
typedef int (_stdcall *P_FILLTEXTINFO)(char *pTextInfo, char *pStyle, char *pMsg);
typedef int (_stdcall *P_PRINT)(char *pMsg);
typedef int (_stdcall *P_CLOSE)(char *pMsg);

P_OPEN			p_open			= NULL;
P_FILLPICINFO	p_fillpicinfo	= NULL;
P_FILLTEXTINFO	p_filltextinfo	= NULL;
P_PRINT			p_print			= NULL;
P_CLOSE			p_close			= NULL;	
	*/


	//2 打印机模块
	HINSTANCE hInstP = ::LoadLibrary("Printer.dll");
	if(hInstP)
	{
		this->Ctrl_ShowDebugInfo("加载[Printer.dll]成功");

		p_open = (P_OPEN) GetProcAddress(hInstP, "Open");
		if(p_open)	{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[Open]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[Open]失败");	}

		p_fillpicinfo = (P_FILLPICINFO) GetProcAddress(hInstP, "FillPicInfo");
		if(p_fillpicinfo)	{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[FillPicInfo]成功");	}
		else				{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[FillPicInfo]失败");	}

		p_filltextinfo = (P_FILLTEXTINFO) GetProcAddress(hInstP, "FillTextInfo");
		if(p_filltextinfo)	{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[FillTextInfo]成功");	}
		else				{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[FillTextInfo]失败");	}

		p_print = (P_PRINT) GetProcAddress(hInstP, "Print");
		if(p_print)		{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[Print]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[Print]失败");	}

		p_close = (P_CLOSE) GetProcAddress(hInstP, "Close");
		if(p_close)	{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[Close]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[Printer.dll]获取接口[Close]失败");	}
	}
	else
	{
		this->Ctrl_ShowDebugInfo("加载[Printer.dll]失败");
	}	


	//3 集中显示屏模块
	HINSTANCE hInstCS = ::LoadLibrary("CenterScreen.dll");
	if(hInstCS)
	{
		this->Ctrl_ShowDebugInfo("加载[CenterScreen.dll]成功");

		cs_open = (CS_OPEN) GetProcAddress(hInstCS, "Open");
		if(cs_open)	{	this->Ctrl_ShowDebugInfo("从[CenterScreen.dll]获取接口[Open]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[CenterScreen.dll]获取接口[Open]失败");	}

		cs_showinfo = (CS_SHOWINFO) GetProcAddress(hInstCS, "ShowInfo");
		if(cs_showinfo)	{	this->Ctrl_ShowDebugInfo("从[CenterScreen.dll]获取接口[ShowInfo]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[CenterScreen.dll]获取接口[ShowInfo]失败");	}

		cs_close = (CS_CLOSE) GetProcAddress(hInstCS, "Close");
		if(cs_close)	{	this->Ctrl_ShowDebugInfo("从[CenterScreen.dll]获取接口[Close]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[CenterScreen.dll]获取接口[Close]失败");	}
	}
	else
	{
		this->Ctrl_ShowDebugInfo("加载[CenterScreen.dll]失败");
	}	

	//4 键盘模块
/*
//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall ShowInfo(int iWinNum, char *pInfo, char *pMsg)
//int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *K_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *K_SHOWINFO)(int iWinNum, char *pInfo, char *pMsg);
typedef int (_stdcall *K_SETCALLBACK)(void *pCallBack, char *pMsg);
typedef int (_stdcall *K_CLOSE)(char *pMsg);

K_OPEN			k_open			= NULL;
K_SHOWINFO		k_showinfo		= NULL;
K_SETCALLBACK	k_setcallback	= NULL;
K_CLOSE			k_close			= NULL;
*/

	HINSTANCE hInstK = ::LoadLibrary("Key.dll");
	if(hInstK)
	{
		this->Ctrl_ShowDebugInfo("加载[Key.dll]成功");

		k_open = (K_OPEN) GetProcAddress(hInstK, "Open");
		if(k_open)	{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[Open]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[Open]失败");	}

		k_showinfo = (K_SHOWINFO) GetProcAddress(hInstK, "ShowInfo");
		if(k_showinfo)	{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[ShowInfo]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[ShowInfo]失败");	}
	
		k_setcallback = (K_SETCALLBACK) GetProcAddress(hInstK, "SetCallBack");
		if(k_setcallback)	{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[SetCallBack]成功");	}
		else				{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[SetCallBack]失败");	}

		k_close = (K_CLOSE) GetProcAddress(hInstK, "Close");
		if(k_close)		{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[Close]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[Key.dll]获取接口[Close]失败");	}
	}
	else
	{
		this->Ctrl_ShowDebugInfo("加载[Key.dll]失败");
	}	
	

	//5磁条刷卡器模块
/*
//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *MC_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *MC_SETCALLBACK)(void *pCallBack, char *pMsg);
typedef int (_stdcall *MC_CLOSE)(char *pMsg);

MC_OPEN			mc_open			= NULL;
MC_SETCALLBACK	mc_setcallback	= NULL;
MC_CLOSE		mc_close			= NULL;
*/

	HINSTANCE hInstMC = ::LoadLibrary("MnCard.dll");
	if(hInstMC)
	{
		this->Ctrl_ShowDebugInfo("加载[MnCard.dll]成功");

		mc_open = (MC_OPEN) GetProcAddress(hInstMC, "Open");
		if(mc_open)	{	this->Ctrl_ShowDebugInfo("从[MnCard.dll]获取接口[Open]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[MnCard.dll]获取接口[Open]失败");	}
	
		mc_setcallback = (MC_SETCALLBACK) GetProcAddress(hInstMC, "SetCallBack");
		if(mc_setcallback)	{	this->Ctrl_ShowDebugInfo("从[MnCard.dll]获取接口[SetCallBack]成功");	}
		else				{	this->Ctrl_ShowDebugInfo("从[MnCard.dll]获取接口[SetCallBack]失败");	}

		mc_close = (MC_CLOSE) GetProcAddress(hInstMC, "Close");
		if(mc_close)	{	this->Ctrl_ShowDebugInfo("从[MnCard.dll]获取接口[Close]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[MnCard.dll]获取接口[Close]失败");	}
	}
	else
	{
		this->Ctrl_ShowDebugInfo("加载[MnCard.dll]失败");
	}	

	//6 IC刷卡器模块
/*
//int _declspec(dllexport) __stdcall Open(int iPort, char *pMsg)
//int _declspec(dllexport) __stdcall SetCallBack(void *pCallBack, char *pMsg)
//int _declspec(dllexport) __stdcall Close(char *pMsg)

typedef int (_stdcall *IC_OPEN)(int iPort, char *pMsg);
typedef int (_stdcall *IC_SETCALLBACK)(void *pCallBack, char *pMsg);
typedef int (_stdcall *IC_CLOSE)(char *pMsg);

IC_OPEN			ic_open			= NULL;
IC_SETCALLBACK	ic_setcallback	= NULL;
IC_CLOSE		ic_close		= NULL;
*/

	HINSTANCE hInstIC = ::LoadLibrary("IcCard.dll");
	if(hInstIC)
	{
		this->Ctrl_ShowDebugInfo("加载[IcCard.dll]成功");

		ic_open = (IC_OPEN) GetProcAddress(hInstIC, "Open");
		if(ic_open)	{	this->Ctrl_ShowDebugInfo("从[IcCard.dll]获取接口[Open]成功");	}
		else		{	this->Ctrl_ShowDebugInfo("从[IcCard.dll]获取接口[Open]失败");	}
	
		ic_setcallback = (IC_SETCALLBACK) GetProcAddress(hInstIC, "SetCallBack");
		if(ic_setcallback)	{	this->Ctrl_ShowDebugInfo("从[IcCard.dll]获取接口[SetCallBack]成功");	}
		else				{	this->Ctrl_ShowDebugInfo("从[IcCard.dll]获取接口[SetCallBack]失败");	}

		ic_close = (IC_CLOSE) GetProcAddress(hInstIC, "Close");
		if(ic_close)	{	this->Ctrl_ShowDebugInfo("从[IcCard.dll]获取接口[Close]成功");	}
		else			{	this->Ctrl_ShowDebugInfo("从[IcCard.dll]获取接口[Close]失败");	}
	}
	else
	{
		this->Ctrl_ShowDebugInfo("加载[IcCard.dll]失败");
	}	


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
