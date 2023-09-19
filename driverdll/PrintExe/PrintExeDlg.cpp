
// PrintExeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PrintExe.h"
#include "PrintExeDlg.h"
#include "afxdialogex.h"
#include "..\third-party\tinyxml\tinyxml.h"
#include "..\Utility\XmlUtil.h"
#include "..\Utility\StringUtil.h"
#include "..\Utility\ImCharset.h"
#include "AppSetting.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define EVENT_NAME_PRNT_DLL L"7a8b533f-546b-4dea-83f7-9858a77488f2"
#define TIMERID_CHECK_PRNT_DLL_EXIT 1234

CPrintExeDlg::CPrintExeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrintExeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrintExeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPrintExeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CPrintExeDlg 消息处理程序

BOOL CPrintExeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();    

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetTimer(TIMERID_CHECK_PRNT_DLL_EXIT, 1000, nullptr);
    LOG_DEBUG(L"set timer for checking print dll exit");

    CRequestReceiver::GetInstance()->Init(m_hWnd);
    
    HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, false, EVENT_NAME_PRNT_DLL);
    if (hEvent != NULL)
    {
        LOG_DEBUG(L"notify already ok");
        SetEvent(hEvent);
        CloseHandle(hEvent); 
    }
    else
    {
        LOG_ERROR(L"failed to open event, error=%d", GetLastError());
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPrintExeDlg::OnPaint()
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
HCURSOR CPrintExeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPrintExeDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == TIMERID_CHECK_PRNT_DLL_EXIT)
    {
        HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, false, EVENT_NAME_PRNT_DLL);
        if (hEvent == NULL)
        {
            LOG_DEBUG(L"print dll is already exit");
            exit(0);
        }
        else
        {
            CloseHandle(hEvent);
        }
    }
}

void CPrintExeDlg::ProcessRequest(CRequest* pRequest)
{
    LOG_DEBUG(L"start to process request : %d", pRequest->m_dwMethodIndex);
    if (pRequest->m_dwMethodIndex == METHODINDEX_INIT)
    {
        pRequest->m_pbResult = ProcessInitRequest();        
    }
    else if (pRequest->m_dwMethodIndex == METHODINDEX_SETUP)
    {
        pRequest->m_pbResult = ProcessSetupRequest(pRequest->m_strMethodParam.c_str());        
    }
    else if (pRequest->m_dwMethodIndex == METHODINDEX_PRINT)
    {
        pRequest->m_pbResult = ProcessPrintRequest(pRequest->m_strMethodParam.c_str());        
    }
    else if (pRequest->m_dwMethodIndex == METHODINDEX_CUT)
    {
        pRequest->m_pbResult = ProcessCutRequest();        
    }
    
    if (pRequest->m_hSyncEvent != NULL)
    {
        SetEvent(pRequest->m_hSyncEvent);
    }
    LOG_DEBUG(L"finish to process request : %d, result=%d", pRequest->m_dwMethodIndex, pRequest->m_pbResult);
}

PB_RESULT CPrintExeDlg::ProcessInitRequest()
{
    if (m_bAlreadyInit)
    {
        LOG_INFO(L"already init");
        return PB_OK;
    }    

    PB_RESULT result = CPrintApiFactory::GetPrintApi()->Init();
    if (result != PB_OK)
    {
        return result;
    }
   
    LOG_INFO(L"successful to init printer");
    m_bAlreadyInit = true;
    return PB_OK;
}

PB_RESULT CPrintExeDlg::ProcessSetupRequest(const char* lpszSetupXml)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_PRNT_NOT_INITIALIZED;
    }
    
    TiXmlDocument document;
    document.Parse(lpszSetupXml, nullptr, TIXML_ENCODING_UTF8);

    TiXmlNode* pSetupNode = document.RootElement();
    if (pSetupNode == nullptr)
    {
        LOG_ERROR(L"failed to parse xml, error=%S, path=%S", document.ErrorDesc(), lpszSetupXml);
        return PB_PRNT_INVALID_SETUP_XML;
    }

    TiXmlNode* pLogoNode = pSetupNode->FirstChildElement("Logo");
    m_strLogoFilePath = CXmlUtil::GetNodeText(pLogoNode);
    LOG_DEBUG(L"set logo : %s", m_strLogoFilePath.c_str());

    TiXmlNode* pMarginNode = pSetupNode->FirstChildElement("Margin");
    if (pMarginNode)
    {
        m_nLeftMargin = CStringUtil(CXmlUtil::GetNodeText(pMarginNode->FirstChildElement("Left")).c_str()).ToInt();
        m_nRightMargin = CStringUtil(CXmlUtil::GetNodeText(pMarginNode->FirstChildElement("Right")).c_str()).ToInt();
    }
    LOG_DEBUG(L"set margin, left = %d, right=%d", m_nLeftMargin, m_nRightMargin); 
    
    return PB_OK;
}

PB_RESULT CPrintExeDlg::ProcessPrintRequest(const char* lpszData)
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_PRNT_NOT_INITIALIZED;
    }
    
    //进入汉字模式
    CPrintApiFactory::GetPrintApi()->DoFunction(TX_CHINESE_MODE, TX_ON, 0);
    
    wstring strData = CImCharset::AnsiToUnicode(lpszData);
    CPrintSetting printSetting;
    int nCurrentLineCharNum = 0; //当前行显示的字符数
    int nCurrentIndex = 0;
    int nTotalLength = strData.length();
    while (nCurrentIndex < nTotalLength)
    {
        wstring strText;
        int nLessIndex = strData.find(L'<', nCurrentIndex);
        if (nLessIndex == -1)
        {
            strText = strData.substr(nCurrentIndex, string::npos);
        }
        else
        {
            strText = strData.substr(nCurrentIndex, nLessIndex - nCurrentIndex);
        }

        //删除字符串的换行字符，打印需要换行使用CTRL:CRLF
        strText = CStringUtil(strText.c_str()).RemoveChar(0x0d);
        strText = CStringUtil(strText.c_str()).RemoveChar(0x0a);
        if (!strText.empty())
        {
            PB_RESULT result = Print(printSetting, strText, nCurrentLineCharNum);
            if (result != PB_OK)
            {
                return result;
            }
        }

        if (nLessIndex == -1)
        {
            break;
        }

        nCurrentIndex = nLessIndex;
        wstring strControlCommand;
        int nGreatIndex = strData.find(L'>', nCurrentIndex);
        if (nGreatIndex == -1)
        {
            LOG_ERROR(L"can not find match > symbol");
            return PB_PRNT_DATA_FORMAT_ERROR;
        }
        else
        {
            strControlCommand = strData.substr(nCurrentIndex, nGreatIndex - nCurrentIndex + 1);
        }

        PB_RESULT result = ProcessControlCommand(strControlCommand, printSetting, nCurrentLineCharNum);
        if (result != PB_OK)
        {
            return result;
        }

        nCurrentIndex = nGreatIndex + 1;
    }

    //退出汉字模式
    CPrintApiFactory::GetPrintApi()->DoFunction(TX_CHINESE_MODE, TX_OFF, 0);

    //走纸
    CPrintApiFactory::GetPrintApi()->DoFunction(TX_FEED, 255, 0);

    return PB_OK;
}

PB_RESULT CPrintExeDlg::Print(const CPrintSetting& printSetting, const std::wstring& strText, int& nCurrentLineCharNum)
{    
    // 设置粗体
    CPrintApiFactory::GetPrintApi()->DoFunction(TX_FONT_BOLD, printSetting.m_bFontBold ? TX_ON : TX_OFF, 0);

    // 设置字体大小    
    // 注意：客户反映字体太大，而且业务系统不能修改字体，于是在此强制降低字体大小
    if (printSetting.m_strFontSize == FONT_SIZE_SMALL)
    {
        CPrintApiFactory::GetPrintApi()->DoFunction(TX_FONT_SIZE, TX_SIZE_1X, TX_SIZE_1X);
    }
    else if (printSetting.m_strFontSize == FONT_SIZE_LARGE)
    {
        CPrintApiFactory::GetPrintApi()->DoFunction(TX_FONT_SIZE, TX_SIZE_2X, TX_SIZE_2X);
    }
    else  // normal font size
    {
        CPrintApiFactory::GetPrintApi()->DoFunction(TX_FONT_SIZE, TX_SIZE_1X, TX_SIZE_1X);
    }

    // 设置对齐
    if (printSetting.m_strTextAlign == TEXT_ALIGN_CENTER)
    {
        CPrintApiFactory::GetPrintApi()->DoFunction(TX_ALIGN, TX_ALIGN_CENTER, 0);
    }
    else if (printSetting.m_strTextAlign == TEXT_ALIGN_RIGHT)
    {
        CPrintApiFactory::GetPrintApi()->DoFunction(TX_ALIGN, TX_ALIGN_RIGHT, 0);
    }
    else
    {
        CPrintApiFactory::GetPrintApi()->DoFunction(TX_ALIGN, TX_ALIGN_LEFT, 0);
    }

    //分隔线----占用空间比较小，不能按一个字符算，特殊处理，不走自动计算规则
    if (CStringUtil(strText.c_str()).IsAllChar(L'-'))
    {
        return Print(strText);
    }

    wstring strPrintContent;
    if (nCurrentLineCharNum == 0) //当前行开头位置，需要加上左边距
    {
        strPrintContent.append(m_nLeftMargin, L' ');
        nCurrentLineCharNum += m_nLeftMargin;
    }    

    int nMaxCharNumPerLine = CAppSetting::GetInstance()->GetPrintMaxCharsPerLine();
    if (nCurrentLineCharNum + (int)strText.length() + m_nRightMargin <= nMaxCharNumPerLine)
    {
        nCurrentLineCharNum += strText.length();
        strPrintContent.append(strText);
        return Print(strPrintContent);
    }
    else //一行显示不下，要自动换行显示
    {
        int nCanPrintCharNum = nMaxCharNumPerLine - nCurrentLineCharNum - m_nRightMargin;
        if (nCanPrintCharNum < 0)
        {
            LOG_ERROR(L"error margin config or max_char_num_per_line config");
            return PB_PRNT_INTERNAL_ERROR;
        }

        strPrintContent.append(strText.substr(0, nCanPrintCharNum));
        strPrintContent.append(1, (wchar_t)0x0d);
        strPrintContent.append(1, (wchar_t)0x0a);
        nCurrentLineCharNum = 0;
        int nResult = Print(strPrintContent);
        if (nResult != PB_OK)
        {
            return nResult;
        }

        //递归打印下一行的内容
        return Print(printSetting, strText.substr(nCanPrintCharNum), nCurrentLineCharNum);
    }    
}

PB_RESULT CPrintExeDlg::Print(const wstring& strPrintContent)
{
    CString strData = strPrintContent.c_str();
    return Print(strData);
}

PB_RESULT CPrintExeDlg::Print(CString& strPrintContent)
{
    wstring strLogContent = CStringUtil((LPCWSTR)strPrintContent).CtrlSymbolToHexChar();    
    LOG_DEBUG(L"print length : %d, content : %s", strPrintContent.GetLength(), strLogContent.c_str());
    return CPrintApiFactory::GetPrintApi()->Write(strPrintContent);
}

PB_RESULT CPrintExeDlg::ProcessControlCommand(const std::wstring& strControlCommand, CPrintSetting& printSetting, int& nCurrentLineCharNum)
{
    int nIndex = strControlCommand.find(L':');
    if (nIndex == string::npos)
    {
        LOG_ERROR(L"invalid control command : %s", strControlCommand.c_str());
        return PB_PRNT_DATA_FORMAT_ERROR;
    }
    
    int nTotalLength = strControlCommand.length();
    wstring strControlType = strControlCommand.substr(1, nIndex - 1);
    if (strControlType.find(L"FONT") != string::npos) //模糊匹配，前后有空格也认为正确格式
    {
        wstring strCommand;
        for (int i = nIndex + 1; i < nTotalLength;++i)
        {
            if (strControlCommand[i] != L';' && strControlCommand[i] != L'>')
            {
                strCommand.append(1, strControlCommand[i]);
            }
            else
            {
                if (strCommand.find(L"BOLD-ON") != string::npos)
                {
                    printSetting.m_bFontBold = true;
                }
                else if (strCommand.find(L"BOLD-OFF") != string::npos)
                {
                    printSetting.m_bFontBold = false;
                }
                else if (strCommand.find(L"SLANT-ON") != string::npos || strCommand.find(L"SLIANT-ON") != string::npos)
                {
                    printSetting.m_bFontSlant = true;
                }
                else if (strCommand.find(L"SLANT-OFF") != string::npos || strCommand.find(L"SLIANT-OFF") != string::npos)
                {
                    printSetting.m_bFontSlant = false;
                }
                else if (strCommand.find(FONT_SIZE_NORMAL) != string::npos)
                {
                    printSetting.m_strFontSize = FONT_SIZE_NORMAL;
                }
                else if (strCommand.find(FONT_SIZE_LARGE) != string::npos)
                {
                    printSetting.m_strFontSize = FONT_SIZE_LARGE;
                }
                else if (strCommand.find(FONT_SIZE_SMALL) != string::npos)
                {
                    // attention : the version does not support the font of small size
                    printSetting.m_strFontSize = FONT_SIZE_NORMAL;
                }
                else
                {
                    LOG_DEBUG(L"FONT command %s is not support", strCommand.c_str());
                }

                strCommand = L"";
            }
        }
    }
    else if (strControlType.find(L"TEXT") != string::npos)
    {
        wstring strCommand;
        for (int i = nIndex + 1; i < nTotalLength; ++i)
        {
            if (strControlCommand[i] != L';' && strControlCommand[i] != L'>')
            {
                strCommand.append(1, strControlCommand[i]);
            }
            else
            {
                if (strCommand.find(TEXT_ALIGN_LEFT) != string::npos)
                {
                    printSetting.m_strTextAlign = TEXT_ALIGN_LEFT;
                }
                else if (strCommand.find(TEXT_ALIGN_RIGHT) != string::npos)
                {
                    printSetting.m_strTextAlign = TEXT_ALIGN_RIGHT;
                }
                else if (strCommand.find(TEXT_ALIGN_CENTER) != string::npos)
                {
                    printSetting.m_strTextAlign = TEXT_ALIGN_CENTER;
                }
                else
                {
                    LOG_DEBUG(L"TEXT command %s is not support", strCommand.c_str());
                }

                strCommand = L"";
            }
        }
    }
    else if (strControlType.find(L"CTRL") != string::npos)
    {
        wstring strCommand;
        for (int i = nIndex + 1; i < nTotalLength; ++i)
        {
            if (strControlCommand[i] != L';' && strControlCommand[i] != L'>')
            {
                strCommand.append(1, strControlCommand[i]);
            }
            else
            {
                wstring strPrintContent;
                if (strCommand.find(L"CRLF") != string::npos)
                {                    
                    strPrintContent.append(1, (wchar_t)0x0d);
                    strPrintContent.append(1, (wchar_t)0x0a);
                    Print(strPrintContent);
                    nCurrentLineCharNum = 0;
                }
                else if (strCommand.find(L"LT") != string::npos)
                {
                    Print(L"<");
                }
                else if (strCommand.find(L"GT") != string::npos)
                {
                    Print(L">");
                }
                else if (strCommand.find(L"LOGO") != string::npos)
                {
                    if (m_strLogoFilePath.empty())
                    {
                        LOG_ERROR(L"the logo file is empty.");
                    }
                    else
                    {
                        CPrintApiFactory::GetPrintApi()->PrintImage(m_strLogoFilePath);
                    }
                }
                else if (strCommand.find(L"INIT") != string::npos)
                {
                    printSetting.Init();
                }
                else
                {
                    LOG_DEBUG(L"CTRL command %s is not support", strCommand.c_str());
                }

                strCommand = L"";
            }
        }
    }
    else
    {
        LOG_INFO(L"unsupoort control type : %s", strControlType.c_str());
    }

    return PB_OK;
}

PB_RESULT CPrintExeDlg::ProcessCutRequest()
{
    if (!m_bAlreadyInit)
    {
        LOG_ERROR(L"not init");
        return PB_PRNT_NOT_INITIALIZED;
    }

    return CPrintApiFactory::GetPrintApi()->CutPaper();
}

BOOL CPrintExeDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    if (message == WM_PROCESS_REQUEST)
    {
        ProcessRequest((CRequest*)lParam);
        *pResult = S_OK;
        return TRUE;
    }

    return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}
