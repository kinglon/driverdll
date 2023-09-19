#pragma once

#include <string>
#include <shtypes.h>
#include "Utility.h"

#pragma comment(lib, "shlwapi.lib")
#pragma warning(disable:4215)
class UTILITY_API CImPath
{
public:
    /**
    @name ��ȡӦ������·������б�ܽ�β
    */
	static std::wstring GetDataPath();

    /**
    @name ��ȡ��������·������б�ܽ�β
    */
	static std::wstring GetCachePath();

    /**
    @name ��ȡ������װ·��Drivers\lianzhi����б�ܽ�β
    */
	static std::wstring GetSoftInstallPath(); 

    /**
    @name ��ȡϵͳ������ʱ�ļ���·��%AppData%\\Local
    */
	static std::wstring GetLocalAppDataPath();

	/**
	@name ��ȡϵͳ������ʱ�ļ���·��%AppData%\\Roaming
	*/
	static std::wstring GetAppDataRoamingPath();

    /**
    @name ��ȡ����·������б�ܽ�β
    */
    static std::wstring GetConfPath();

    /**
    @name ��ȡ��Դ·������б�ܽ�β
    */
    static std::wstring GetResourcePath();

    /**
    @name ��ȡ��־·������б�ܽ�β
    */
    static std::wstring GetLogPath();

    /**
    @name ��ȡdump·������б�ܽ�β
    */
    static std::wstring GetDumpPath();

private:	
	static std::wstring GetSystemTempPath();
};