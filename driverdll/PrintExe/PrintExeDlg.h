#pragma once
#include "IPrintApi.h"
#include "RequestReceiver.h"

#define FONT_SIZE_SMALL     L"SIZE-SMALL"
#define FONT_SIZE_NORMAL  L"SIZE-NORMAL"
#define FONT_SIZE_LARGE       L"SIZE-LARGE"

#define TEXT_ALIGN_LEFT    L"ALIGN-LEFT" 
#define TEXT_ALIGN_CENTER    L"ALIGN-CENTER" 
#define TEXT_ALIGN_RIGHT    L"ALIGN-RIGHT" 

class CPrintExeDlg : public CDialogEx
{
private:
    class CPrintSetting
    {
    public:
        bool m_bFontBold = false;  //�Ӵ�
        bool m_bFontSlant = false; //б��
        std::wstring m_strFontSize = FONT_SIZE_NORMAL;  //��С
        std::wstring m_strTextAlign = TEXT_ALIGN_LEFT;  //�ı�����

    public:
        void Init()
        {
            m_bFontBold = false;
            m_bFontSlant = false;
            m_strFontSize = FONT_SIZE_NORMAL;
            m_strTextAlign = TEXT_ALIGN_LEFT;
        }
    };
    
public:
	CPrintExeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PRINTEXE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��    

protected:
    bool m_bAlreadyInit = false;
    std::wstring m_strLogoFilePath;
    int m_nLeftMargin = 0;
    int m_nRightMargin = 0;
    
protected:
    void ProcessRequest(CRequest* pRequest);
    PB_RESULT ProcessInitRequest();
    PB_RESULT ProcessSetupRequest(const char* lpszSetupXml);
    PB_RESULT ProcessPrintRequest(const char* lpszData);
    PB_RESULT ProcessCutRequest();
    
    PB_RESULT Print(const CPrintSetting& printSetting, const std::wstring& strText, int& nCurrentLineCharNum);
    PB_RESULT Print(const std::wstring& strPrintContent);
    PB_RESULT Print(CString& strPrintContent);
    PB_RESULT ProcessControlCommand(const std::wstring& strControlCommand, CPrintSetting& printSetting, int& nCurrentLineCharNum);
    
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};