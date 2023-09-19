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
    @name ת��Ϊ����
    */
    int ToInt();

    /**
    @name ���ַ����еĿ����ַ�ת��16�����ַ�
    */
    std::wstring CtrlSymbolToHexChar();

    /**
    @name ���ڴ��ÿ���ֽ���2��16���Ƶ��ַ����
    @param splitCharÿ2��16�����ַ��ķָ�����Ĭ��\0��ʾ����Ҫ�ָ���
    */
    static std::wstring ToHexChar(const unsigned char* pData, int nDataLength, const wchar_t splitChar = L'\0');

    /**
    @name ��16���Ƶ��ı���ת��Ascii�ַ���
    */
    static std::string ToAsciiChar(const std::wstring& input);
    
    /**
    @name ���ڴ��ת��Ϊ������ÿ���ֽ����޷���ʮ�����������
    */
    static std::wstring ToUnsignedIntString(const unsigned char* pData, int nDataLength);

    /**
    @name �ж��Ƿ�Ϊ����
    */
    bool IsAllDigital();

    /**
    @name ���ַ�������chOriginal�ַ��滻ΪchWillReplace
    */
    std::string ReplaceAll(char chOriginal, char chWillReplace);

    /**
    @name ȥ����β�ո�
    */
    std::wstring TrimSpace();

	/**
	@name ȥ�����ɼ��ַ�
	*/
	std::wstring TrimInvisibleChar();

    /**
    @name ���ַ�����ָ�����ַ�ȫ��ɾ��
    @param chWillRemove ָ��Ҫɾ�����ַ�
    */
    std::wstring RemoveChar(char chWillRemove);

    /**
    @name У���ַ������ַ��Ƿ�Ϊָ�����ַ�
    @param chWillCompare ָ���Ƚϵ��ַ�
    */
    bool IsAllChar(char chWillCompare);

private:
    std::wstring m_strData;
};
