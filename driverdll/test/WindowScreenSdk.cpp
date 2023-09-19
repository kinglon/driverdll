#include "stdafx.h"
#include "WindowScreenSdk.h"
#include "..\Utility\ImPath.h"
#include "..\third-party\tinyxml\tinyxml.h"
#include "..\Utility\ImCharset.h"

using namespace std;

CWindowScreenSdk::CWindowScreenSdk()
{
    LoadModule();
}

CWindowScreenSdk::~CWindowScreenSdk()
{
}

CWindowScreenSdk* CWindowScreenSdk::GetInstance()
{
    static CWindowScreenSdk* pInstance = new CWindowScreenSdk();
    return pInstance;
}

void CWindowScreenSdk::Init()
{
    //
}

void CWindowScreenSdk::LoadModule()
{
    wstring strDllPath = CImPath::GetSoftInstallPath() + L"BERR.dll";
    HMODULE hModule = LoadLibraryEx(strDllPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (hModule == NULL)
    {
        MessageBox(NULL, L"加载窗口条屏DLL失败", L"错误", MB_OK);
        return;
    }

    m_pInitializeFunction = (PBERRInitialize)GetProcAddress(hModule, "BERR_Initialize");
    if (m_pInitializeFunction == NULL)
    {
        MessageBox(NULL, L"获取BERR_Initialize函数地址失败", L"错误", MB_OK);
        return;
    }

    m_pShowFunction = (PBERRShow)GetProcAddress(hModule, "BERR_Show");
    if (m_pShowFunction == NULL)
    {
        MessageBox(NULL, L"获取BERR_Show函数地址失败", L"错误", MB_OK);
        return;
    }

    m_pGetStatusFunction = (PBERRGetStatus)GetProcAddress(hModule, "BERR_GetStatus");
    if (m_pGetStatusFunction == NULL)
    {
        MessageBox(NULL, L"获取BERR_GetStatus函数地址失败", L"错误", MB_OK);
        return;
    }

    m_bLoadSuccess = true;

    string strXmlValue = "<?xml version=\"1.0\" encoding=\"utf - 8\"?><Device>  <DeviceId>Window Screen One</DeviceId>  <LogLevel>3</LogLevel></Device>";
    PB_RESULT result = m_pInitializeFunction(strXmlValue.c_str());
    if (result != PB_OK)
    {
        MessageBox(NULL, L"窗口条屏初始化失败", L"错误", MB_OK);
    }
}

PB_RESULT CWindowScreenSdk::BERRShow(const wchar_t* szWindowNumber, const wchar_t* szText)
{
    if (m_bLoadSuccess && m_pShowFunction)
    {
        TiXmlDocument document;
        document.Parse("<?xml version=\"1.0\" encoding=\"utf-8\"?>");

        TiXmlElement rootNode("Command");
        TiXmlElement modeNode("Mode");
        modeNode.InsertEndChild(TiXmlText("TEXT"));
        rootNode.InsertEndChild(modeNode);

        TiXmlElement dataNode("Data");
        dataNode.InsertEndChild(TiXmlText(CImCharset::UnicodeToUTF8(szText).c_str()));
        rootNode.InsertEndChild(dataNode);
        document.InsertEndChild(rootNode);

        TiXmlPrinter printer;
        document.Accept(&printer);
        string strXmlValue = printer.CStr();
        return m_pShowFunction(CImCharset::UnicodeToUTF8(szWindowNumber).c_str(), strXmlValue.c_str());
    }
    else
    {
        return PB_BERR_INTERNAL_ERROR;
    }

}