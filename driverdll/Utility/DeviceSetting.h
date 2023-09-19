#pragma once

class UTILITY_API CDeviceSetting
{
public:
    CDeviceSetting();
    ~CDeviceSetting();

public:
    bool Parse(const std::string& strSettingXml);

public:
    std::wstring m_strDeviceId;
    ELogLevel m_logLevel = ELogLevel::LOG_LEVEL_FATAL;
};

