// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

#include "LogMacro.h"

#define DEVICE_NAME     L"CRDR"

#ifdef _DEBUG
#define CRDR_ASSERT(condition) if (!(condition)) { DebugBreak(); }
#else
#define CRDR_ASSERT(condition) ((void)0)
#endif