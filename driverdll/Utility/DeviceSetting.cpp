#include "stdafx.h"
#include "DeviceSetting.h"
#include "tinyxml\tinyxml.h"
#include "XmlUtil.h"

using namespace std;

CDeviceSetting::CDeviceSetting()
{
}

CDeviceSetting::~CDeviceSetting()
{
}

bool CDeviceSetting::Parse(const std::string& strSettingXml)
{
    TiXmlDocument document;
    document.Parse(strSettingXml.c_str(), nullptr, TIXML_ENCODING_UTF8);    

    TiXmlNode* pDeviceNode = document.RootElement();
    if (pDeviceNode == nullptr)
    {
        LOG_ERROR(L"failed to parse xml, error=%S, path=%S", document.ErrorDesc(), strSettingXml.c_str());
        return false;
    }

    TiXmlNode* pDeviceIdNode = pDeviceNode->FirstChildElement("DeviceId");
    m_strDeviceId = CXmlUtil::GetNodeText(pDeviceIdNode);

    TiXmlNode* pLogLevelNode = pDeviceNode->FirstChildElement("LogLevel");
    wstring strLogLevel = CXmlUtil::GetNodeText(pLogLevelNode);
    if (strLogLevel == L"0")
    {
        m_logLevel = ELogLevel::LOG_LEVEL_FATAL;
    }
    else if (strLogLevel == L"1")
    {
        m_logLevel = ELogLevel::LOG_LEVEL_ERROR;
    }
    else if (strLogLevel == L"2")
    {
        m_logLevel = ELogLevel::LOG_LEVEL_INFO;
    }
    else if (strLogLevel == L"3")
    {
        m_logLevel = ELogLevel::LOG_LEVEL_DEBUG;
    }
    else
    {
        UTILITY_ASSERT(false);
        m_logLevel = ELogLevel::LOG_LEVEL_FATAL;
    }

    return true;
}