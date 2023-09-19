#include "stdafx.h"
#include "XmlUtil.h"
#include "ImCharset.h"

CXmlUtil::CXmlUtil()
{
}

CXmlUtil::~CXmlUtil()
{
}

std::wstring CXmlUtil::GetAttributeValueString(TiXmlNode* pNode, const char * attrName)
{
    if (pNode == nullptr)
    {
        return L"";
    }

    TiXmlElement* element = pNode->ToElement();
    if (element && element->Attribute(attrName))
    {
        return CImCharset::UTF8ToUnicode(element->Attribute(attrName));
    }
    else
    {        
        return L"";
    }
}

int CXmlUtil::GetAttributeValueInt(TiXmlNode* pNode, const char * attrName)
{
    if (pNode == nullptr)
    {
        return INVALID_INTEGER;
    }

    TiXmlElement* element = pNode->ToElement();
    if (element && element->Attribute(attrName))
    {
        int nValue = INVALID_INTEGER;
        element->Attribute(attrName, &nValue);
        return nValue;
    }
    else
    {        
        return INVALID_INTEGER;
    }
}

std::wstring CXmlUtil::GetNodeText(TiXmlNode* pNode)
{
    if (pNode == nullptr)
    {
        return L"";
    }

    TiXmlElement* element = pNode->ToElement();
    if (element)
    {
        return CImCharset::UTF8ToUnicode(element->GetText());
    }
    else
    {
        return L"";
    }
}