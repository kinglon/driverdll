#pragma once
#include <string>
#include "tinyxml\tinyxml.h"
#include "Utility.h"

#define INVALID_INTEGER     0xffffffff

class UTILITY_API CXmlUtil
{
public:
    CXmlUtil();
    ~CXmlUtil();

public:
    static std::wstring GetAttributeValueString(TiXmlNode* pNode, const char * attrName);
    static int GetAttributeValueInt(TiXmlNode* pNode, const char * attrName);
    static std::wstring GetNodeText(TiXmlNode* pNode);
};