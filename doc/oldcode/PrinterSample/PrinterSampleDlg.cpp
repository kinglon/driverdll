// PrinterSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PrinterSample.h"
#include "PrinterSampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrinterSampleDlg dialog

CPrinterSampleDlg::CPrinterSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrinterSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrinterSampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	m_pPrinter = NULL;
}

void CPrinterSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrinterSampleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPrinterSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CPrinterSampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_PRINTTICKET,OnPrintTicket)	
	ON_BN_CLICKED(IDC_BUTTON_Test, On_BUTTON_Test)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrinterSampleDlg message handlers

BOOL CPrinterSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here


	if(false == Printer_Init())
	{
		AfxMessageBox("请检查打印机!");
		exit(0);
	}


	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPrinterSampleDlg::OnPaint() 
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
HCURSOR CPrinterSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

bool CPrinterSampleDlg::Printer_Init()
{
	if(NULL != this->m_pPrinter)
	{
		m_pPrinter->Close();
		delete m_pPrinter;
		m_pPrinter = NULL;
	}

	m_pPrinter = new CCoptr;


	HWND hWnd = ::GetDesktopWindow();
	
	m_pPrinter->Create(NULL,WS_CHILD,CRect(0,0,0,0),CWnd::FromHandle(hWnd),1);

	//m_pPrinter->Create(NULL,WS_CHILD/*|WS_VISIBLE*/,CRect(0,0,0,0),m_pQueueManager->m_pQSDlg,1);
	


	if(m_pPrinter->Open("EPSON") != OPOS_SUCCESS)
	{
		return false;
	}

	if(m_pPrinter->Claim(200) != OPOS_SUCCESS)
	{
		return false;
	}
	
	m_pPrinter->SetDeviceEnabled(TRUE);
	if(m_pPrinter->GetResultCode() != OPOS_SUCCESS)
	{
		return false;
	}

	m_pPrinter->ClearOutput();	


	/*
	//设置Logo
	CString strLogoFile;
	strLogoFile.Format("D:\\QmDriver\\Config\\Logo.bmp");

	CString strData;
	if(m_pPrinter->GetCapRecPresent() == TRUE)
	{
		m_pPrinter->SetRecLetterQuality(TRUE);
		
		strData.Format("%c|rA",ESC);
		if(m_pPrinter->ValidateData(PTR_S_RECEIPT,strData.GetBuffer(0)) == OPOS_SUCCESS)
		{
			m_pPrinter->SetBitmap(1,PTR_S_RECEIPT, strLogoFile,m_pPrinter->GetRecLineWidth(),PTR_BM_CENTER);
		}
		else
		{
			m_pPrinter->SetBitmap(1,PTR_S_RECEIPT,strLogoFile,m_pPrinter->GetRecLineWidth(),PTR_BM_LEFT);
		}

		strData.Format("%c|cA%s",ESC,"网点LOGO");
		m_pPrinter->SetLogo(PTR_L_TOP,strData.GetBuffer(0));
	}
	*/


	return true;
}



LRESULT CPrinterSampleDlg::OnPrintTicket(WPARAM wParam,LPARAM lParam)
{
	//-----------------------------------------------
	//得到打印文件内容
	CStringArray arrContent;

	FILE *fp = fopen("D:\\QmDriver\\Log\\printer.prt", "rb");
	if(!fp)
	{
		return 0;
	}
	while(!feof(fp))
	{
		char cLine[1000] = {0};
		fgets(cLine, 1000, fp);

		CString strLine;
		strLine.Format("%s", cLine);
		strLine.TrimLeft();
		strLine.TrimRight();
		if(strLine.GetLength() > 0)
		{
			arrContent.Add(strLine);
		}
	}
	fclose(fp);

	//-----------------------------------------------
	//打印内容

	CString strEnter,strFont,strLineDis,strInChnMode,strOutChnMode,strPaperGo,strLogoFile;

	strEnter.Format("%c%c",13,10);
	strFont.Format("%c%c",29,33);	//GS !
	strLineDis.Format("%c%c%c",27,51,10);	//ESC 3 n
	strInChnMode.Format("%c%c",28,38);
	strOutChnMode.Format("%c%c",28,46);
	strPaperGo.Format("%c%c%c",27,51,50/*60*/);
	
	//-------------------------------

	CString strPrintContent = "";

	CString strAdd;
	
	
	strPrintContent.Format("%c|1B",ESC);	//标头

	strAdd.Format("%c|cA",ESC);				//居中对齐
	strPrintContent += strAdd;

	strPrintContent += strInChnMode;		//进入汉字模式
//	strPrintContent += strLineDis;			//设置行距


	for(int i=0; i<arrContent.GetSize(); i++)
	{
		CString strContent = arrContent.GetAt(i);
		CStringArray arrItem;

		char *p = strtok(strContent.GetBuffer(0), "#");
		while(p)
		{
			arrItem.Add(p);
			p = strtok(NULL, "#");
		}

		if(3 != arrItem.GetSize())
		{
			continue;
		}

		if(0 == arrItem.GetAt(0).Compare("1"))
		{//图片
			//设置Logo
			CString strLogoFile;
			strLogoFile.Format("%s", arrItem.GetAt(1).GetBuffer(0));
			
			CString strData;
			if(m_pPrinter->GetCapRecPresent() == TRUE)
			{
				m_pPrinter->SetRecLetterQuality(TRUE);
				
				strData.Format("%c|rA",ESC);
				if(m_pPrinter->ValidateData(PTR_S_RECEIPT,strData.GetBuffer(0)) == OPOS_SUCCESS)
				{
					m_pPrinter->SetBitmap(1,PTR_S_RECEIPT, strLogoFile,m_pPrinter->GetRecLineWidth(),PTR_BM_CENTER);
				}
				else
				{
					m_pPrinter->SetBitmap(1,PTR_S_RECEIPT,strLogoFile,m_pPrinter->GetRecLineWidth(),PTR_BM_LEFT);
				}
				
				strData.Format("%c|cA%s",ESC,"网点LOGO");
				m_pPrinter->SetLogo(PTR_L_TOP,strData.GetBuffer(0));
			}
		}

		if(0 == arrItem.GetAt(0).Compare("2"))
		{//文本
			CString strText		= arrItem.GetAt(1);
			CString strStyle	= arrItem.GetAt(2);

			int iFontSize		= 0;
			int iIsNextLine		= 0;
			int iLineDistance	= 1;

			p = strtok(strStyle.GetBuffer(0), "|");
			iFontSize = atoi(p);
			p = strtok(NULL, "|");
			iIsNextLine = atoi(p);
			p = strtok(NULL, "|");
			iLineDistance = atoi(p);

			if(1 == iIsNextLine)
			{
				switch(iLineDistance)
				{
				case 1:
					{
						strLineDis.Format("%c%c%c",27,51,40);
					}
					break;
				case 2:
					{
						strLineDis.Format("%c%c%c",27,51,80);
					}
					break;
				case 3:
					{
						strLineDis.Format("%c%c%c",27,51,120);
					}
					break;
				default:
					{
						strLineDis.Format("%c%c%c",27,51,40);
					}
					break;
				}
				strPrintContent += strLineDis;		//设置行距
			}

			switch(iFontSize)
			{
			case 1:
				{
					strAdd.Format("%s%c%s", strFont, 16, strText.GetBuffer(0));
				}
				break;
			case 2:
				{
					strAdd.Format("%s%c%s", strFont, 1, strText.GetBuffer(0));
				}
				break;
			case 3:
				{
					strAdd.Format("%s%c%s", strFont, 17, strText.GetBuffer(0));
				}
				break;
			default:
				{
					strAdd.Format("%s%c%s", strFont, 1, strText.GetBuffer(0));
				}
				break;
			}
			strPrintContent += strAdd;

			if(1 == iIsNextLine)
			{
				strPrintContent += strEnter;	//换行
			}
		}

		arrItem.RemoveAll();	
	}


	strPrintContent += strOutChnMode;	//退出汉字模式	
	strPrintContent += strPaperGo;		//走纸


	m_pPrinter->PrintNormal(PTR_S_RECEIPT, strPrintContent.GetBuffer(0));//打印
	
	if(m_pPrinter->GetResultCode() != OPOS_SUCCESS)
	{
		return 0;
	}


	//切纸
	CString strData;
	if(m_pPrinter->GetCapRecPapercut() == TRUE)
	{
		strData.Format("%c|fP",ESC);
		m_pPrinter->PrintNormal(PTR_S_RECEIPT,strData.GetBuffer(0));
		
		if(m_pPrinter->GetResultCode() != OPOS_SUCCESS)
		{
			return 0;
		}
	}
	else
	{
		strData.Format("%c|%dlF",ESC, m_pPrinter->GetRecLinesToPaperCut());
		m_pPrinter->PrintNormal(PTR_S_RECEIPT,strData.GetBuffer(0));
		if(m_pPrinter->GetResultCode() != OPOS_SUCCESS)
		{
			return 0;
		}
	}

	//-----------------------------------------------




	return 0;
}

void CPrinterSampleDlg::On_BUTTON_Test() 
{
	// TODO: Add your control notification handler code here

	::PostMessageA(m_hWnd, WM_PRINTTICKET, 0, 0);
	
}
