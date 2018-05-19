#include "IPCHandler.h"
#include "Date.h"
#include "DateFormat.h"

using std::map;
using std::string;

IPCHandler::IPCHandler()
{
    m_callback[IPC_DATE] = (IPCHandler::CALLBACK)&IPCHandler::Date;
}

bool IPCHandler::OnMessage(IPCMsgPtr pMsg)
{
    map<string, CALLBACK>::iterator iter = m_callback.find(pMsg->Cmd());
    if (m_callback.end() != iter)
    {
        (this->*(iter->second))(pMsg);
        return true;
    }
    return false;
}

bool IPCHandler::Date(IPCMsgPtr pMsg)
{
    string dateStr = Date::Now().Format();
    pMsg->SetReply(dateStr);
    return true;
}
