#pragma once

#include <string>

class CWaveFile  
{
public:
	CWaveFile();
	virtual ~CWaveFile();

public:
    static bool BindFile(const std::wstring& strDestFile, const std::wstring& strBindFile);
};
