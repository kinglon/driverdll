#include "stdafx.h"
#include "RequestReceiver.h"
#include "..\Utility\ImPath.h"
#include "..\Utility\ImCharset.h"
#include <future>

using namespace std;

extern HMODULE g_hDllModule;

CRequestReceiver::CRequestReceiver()
{
}

CRequestReceiver::~CRequestReceiver()
{
}

CRequestReceiver* CRequestReceiver::GetInstance()
{
    static CRequestReceiver* instacne = new CRequestReceiver();
    return instacne;
}

void CRequestReceiver::ComThread(HANDLE hEvent)
{
    LOG_DEBUG(L"com thread start");
    CRequestReceiver::GetInstance()->InitInComThread(hEvent);
    LOG_DEBUG(L"com thread exit");
}

void CRequestReceiver::InitInComThread(HANDLE hEvent)
{
    WNDCLASS wc = { 0 };
    wc.lpszClassName = L"driversddd";
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = MsgWndWindowProc;
    wc.hInstance = g_hDllModule;
    RegisterClass(&wc);
    HWND hReceiveRequestWnd = CreateWindowExW(WS_EX_LAYERED, L"driversddd", L"driversddd", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, GetDesktopWindow(), 0, g_hDllModule, 0);
    if (hReceiveRequestWnd == NULL)
    {
        LOG_ERROR(L"failed to create window for receiving request, error=%d", GetLastError());
        SetEvent(hEvent);
        return;
    }    
    else
    {
        LOG_DEBUG(L"succeed to init window for receiving request");
    }
    
    SetEvent(hEvent);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        DispatchMessage(&msg);
    }
    
    CloseHandle(hReceiveRequestWnd);
}

LRESULT CALLBACK CRequestReceiver::MsgWndWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if  (message == WM_COPYDATA)
    {
        PCOPYDATASTRUCT pCopyDataStruct = (PCOPYDATASTRUCT)lParam;
        return CRequestReceiver::GetInstance()->ProcessRequest(pCopyDataStruct);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

PB_RESULT CRequestReceiver::ProcessRequest(PCOPYDATASTRUCT pCopyDataStruct)
{
    CRequest* pRequest = new CRequest();
    pRequest->m_dwMethodIndex = pCopyDataStruct->dwData;
    pRequest->m_strMethodParam = string((char*)pCopyDataStruct->lpData, pCopyDataStruct->cbData);
    pRequest->m_hSyncEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (pRequest->m_hSyncEvent == NULL)
    {
        delete pRequest;
        LOG_ERROR(L"failed to create sync event, error=%d", GetLastError());
        return PB_PRNT_INTERNAL_ERROR;
    }

    LOG_DEBUG(L"start to post request(%d) to main window", pRequest->m_dwMethodIndex);
    PostMessage(m_hMainWnd, WM_PROCESS_REQUEST, 0, (LPARAM)pRequest);
    if (WaitForSingleObject(pRequest->m_hSyncEvent, INFINITE) != WAIT_OBJECT_0)
    {
        CloseHandle(pRequest->m_hSyncEvent);
        LOG_ERROR(L"failed to wait sync event, error=%d", GetLastError());
        return PB_PRNT_INTERNAL_ERROR;
    }
    
    PB_RESULT pbResult = pRequest->m_pbResult;
    LOG_DEBUG(L"suceed to process request(%d), result=%d", pRequest->m_dwMethodIndex, pbResult);
    CloseHandle(pRequest->m_hSyncEvent);  
    pRequest->m_hSyncEvent = NULL;    
    delete pRequest;
    pRequest = nullptr;
    return pbResult;
}

void CRequestReceiver::Init(HWND hWnd)
{
    m_hMainWnd = hWnd;
    HANDLE hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    std::async(std::launch::async, ComThread, hEvent);
    if (WaitForSingleObject(hEvent, 20000) != WAIT_OBJECT_0)
    {
        LOG_ERROR(L"failed to wait com thread, error=%d", GetLastError());
    }
    CloseHandle(hEvent);
}