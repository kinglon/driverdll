#include "stdafx.h"
#include "StringUtil.h"
#include "ImCharset.h"

using namespace std;

CStringUtil::CStringUtil(const char* szData)
{
    m_strData = CImCharset::UTF8ToUnicode(szData);
}

CStringUtil::CStringUtil(const char* szData, int nLength)
{
    for (int i = 0; i < nLength; ++i)
    {
        m_strData += szData[i];
    }
}

CStringUtil::CStringUtil(const wchar_t* szData)
{
    m_strData = szData;
}

CStringUtil::~CStringUtil()
{
}

int CStringUtil::ToInt()
{
    return _wtoi(m_strData.c_str());
}

std::wstring CStringUtil::CtrlSymbolToHexChar()
{
    wstring strResult;
    for (auto ch : m_strData)
    {
        if (ch < L'!' || ch >L'~')
        {
            wchar_t szBuf[10];
            memset(szBuf, 0, sizeof(szBuf));
            _itow_s((int)ch, szBuf, 10, 16);
            strResult.append(L"\\x");
            strResult.append(szBuf);
        }
        else
        {
            strResult.append(1, ch);
        }
    }

    return strResult;
}

std::wstring CStringUtil::ToHexChar(const unsigned char* pData, int nDataLength, const wchar_t splitChar)
{
    wstring strResult;
    for (int i = 0; i < nDataLength; i++)
    {
        if (splitChar != L'\0' && i != 0) {
            strResult += splitChar;
        }

        unsigned char ch = pData[i];
        wchar_t szBuf[10];
        memset(szBuf, 0, sizeof(szBuf));
        wsprintf(szBuf, L"%02x", (int)ch);
        strResult += szBuf;
    }

    return strResult;
}

std::string CStringUtil::ToAsciiChar(const std::wstring& input)
{
    if (input.length() % 2 != 0) {
        return "";
    }

    std::string strResult;
    for (unsigned int i = 0; i + 1 < input.length(); i += 2)
    {
        int first = 0;
        int second = 0;
        if (input[i] >= L'0' && input[i] <= L'9') {
            first = input[i] - L'0';
        }
        else if (input[i] >= L'A' && input[i] <= L'F') {
            first = input[i] - L'A' + 10;
        }

        if (input[i+1] >= L'0' && input[i+1] <= L'9') {
            second = input[i+1] - L'0';
        }
        else if (input[i+1] >= L'A' && input[i+1] <= L'F') {
            second = input[i+1] - L'A' + 10;
        }

        int val = first * 16 + second;
        strResult += (char)val;
    }

    return strResult;
}

std::wstring CStringUtil::ToUnsignedIntString(const unsigned char* pData, int nDataLength)
{
    wstring strContent;
    for (int i = 0; i < nDataLength; ++i)
    {
        strContent += to_wstring(pData[i]);
        strContent += L" ";
    }

    return strContent;
}

bool CStringUtil::IsAllDigital()
{
    if (m_strData.empty())
    {
        return false;
    }

    for (auto it = m_strData.begin(); it != m_strData.end(); ++it)
    {
        if (*it < L'0' && *it > L'9')
        {
            return false;
        }
    }

    return true;
}

std::string CStringUtil::ReplaceAll(char chOriginal, char chWillReplace)
{
    string strResult;
    string strTemp = CImCharset::UnicodeToUTF8(m_strData.c_str());
    for (auto ch : strTemp)
    {
        if (ch == chOriginal)
        {
            strResult += chWillReplace;
        }
        else
        {
            strResult += ch;
        }
    }

    return strResult;
}

std::wstring CStringUtil::TrimSpace()
{
    int nFirstNoSpaceIndex = 0;
    int nLastNoSpaceIndex = m_strData.length();
    for (; nFirstNoSpaceIndex < nLastNoSpaceIndex; nFirstNoSpaceIndex++)
    {
        if (m_strData[nFirstNoSpaceIndex] != L' ')
        {
            break;
        }
    }

    for (--nLastNoSpaceIndex; nLastNoSpaceIndex > nFirstNoSpaceIndex; nLastNoSpaceIndex--)
    {
        if (m_strData[nLastNoSpaceIndex] != L' ')
        {
            break;
        }
    }

    if (nFirstNoSpaceIndex > nLastNoSpaceIndex)
    {
        return L"";
    }
    else
    {
        return m_strData.substr(nFirstNoSpaceIndex, nLastNoSpaceIndex-nFirstNoSpaceIndex+1);
    }
}

std::wstring CStringUtil::TrimNoNumberChar()
{
	wstring strResult;
	for (auto ch : m_strData)
	{
		if (ch >= L'0' && ch <= L'9')
		{
			strResult += ch;
		}
	}

	return strResult;
}

std::wstring CStringUtil::RemoveChar(char chWillRemove)
{
    wstring strResult;
    for (auto ch : m_strData)
    {
        if (ch != chWillRemove)
        {
            strResult.append(1, ch);
        }
    }

    return strResult;
}

bool CStringUtil::IsAllChar(char chWillCompare)
{
    if (m_strData.empty())
    {
        return false;
    }

    for (auto ch : m_strData)
    {
        if (ch != chWillCompare)
        {
            return false;
        }
    }

    return true;
}