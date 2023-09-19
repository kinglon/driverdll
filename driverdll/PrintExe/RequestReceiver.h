#pragma once

#include <string>
#include "InterfaceSpec.h"
#include "PrintRequest.h"

#define WM_PROCESS_REQUEST      WM_USER+120

class CRequestReceiver
{
protected:
    CRequestReceiver();
    ~CRequestReceiver();

public:
    static CRequestReceiver* GetInstance();
    
public:
    void Init(HWND hWnd);
    
private:
    static void ComThread(HANDLE hEvent);
    void InitInComThread(HANDLE hEvent);
    static LRESULT CALLBACK MsgWndWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    PB_RESULT ProcessRequest(PCOPYDATASTRUCT pCopyDataStruct);
    
private:
    HWND m_hMainWnd = NULL;
};

