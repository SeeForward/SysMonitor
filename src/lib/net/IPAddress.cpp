#include "IPAddress.h"
#include "StringUtil.h"

IPAddress::IPAddress()
{
    m_vaild = false;
}

IPAddress::IPAddress(const std::string &ip)
{
    m_ip = ip;
    m_vaild = IsValid();
}

bool IPAddress::Valid() const
{
    return m_vaild;
}

const std::string& IPAddress::Str() const
{
    return m_ip;
}

bool IPAddress::IsValid()
{
    vector<string> strs;
    StringUtil::Split(m_ip, '.', strs);
    if (4 != strs.size())
    {
        return false;
    }

    string trimIp;
    for (size_t i = 0; i < strs.size(); ++i)
    {
        string trimStr = StringUtil::Trim(strs[i]);
        if (!StringUtil::IsNumber(trimStr))
        {
            return false;
        }

        int32_t n = StringUtil::ToInt32(trimStr);
        if (n < 0 || n > 255)
        {
            return false;
        }
        trimIp += trimStr;
        if (strs.size() - 1 != i)
        {
            trimIp += ".";
        }
    }

    if (trimIp != m_ip)
    {
        m_ip = trimIp;
    }
    return true;
}
