// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

#include "LogMacro.h"

#define DEVICE_NAME     L"CRDR"

#ifdef _DEBUG
#define CRDR_ASSERT(condition) if (!(condition)) { DebugBreak(); }
#else
#define CRDR_ASSERT(condition) ((void)0)
#endif