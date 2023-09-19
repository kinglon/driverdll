#pragma once

#include <string>
#include "Utility.h"

#pragma warning(disable:4251)
class UTILITY_API CStringUtil
{
public:
    CStringUtil(const char* szData);
    CStringUtil(const char* szData, int nLength);
    CStringUtil(const wchar_t* szData);
    ~CStringUtil();

public:
    /**
    @name 转化为整数
    */
    int ToInt();

    /**
    @name 将字符串中的控制字符转成16进制字符
    */
    std::wstring CtrlSymbolToHexChar();

    /**
    @name 将内存块每个字节以2个16进制的字符输出
    @param splitChar每2个16进制字符的分隔符，默认\0表示不需要分隔符
    */
    static std::wstring ToHexChar(const unsigned char* pData, int nDataLength, const wchar_t splitChar = L'\0');

    /**
    @name 将16进制的文本串转成Ascii字符串
    */
    static std::string ToAsciiChar(const std::wstring& input);
    
    /**
    @name 将内存块转化为整数，每个字节以无符号十进制数字输出
    */
    static std::wstring ToUnsignedIntString(const unsigned char* pData, int nDataLength);

    /**
    @name 判断是否都为数字
    */
    bool IsAllDigital();

    /**
    @name 将字符串所有chOriginal字符替换为chWillReplace
    */
    std::string ReplaceAll(char chOriginal, char chWillReplace);

    /**
    @name 去除首尾空格
    */
    std::wstring TrimSpace();

	/**
	@name 去除不可见字符
	*/
	std::wstring TrimInvisibleChar();

    /**
    @name 将字符串中指定的字符全部删除
    @param chWillRemove 指定要删除的字符
    */
    std::wstring RemoveChar(char chWillRemove);

    /**
    @name 校验字符串的字符是否都为指定的字符
    @param chWillCompare 指定比较的字符
    */
    bool IsAllChar(char chWillCompare);

private:
    std::wstring m_strData;
};

