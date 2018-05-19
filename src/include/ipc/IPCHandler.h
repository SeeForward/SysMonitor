#ifndef __IPC_HANDLER_H__
#define __IPC_HANDLER_H__

#include <map>
#include <string>
#include "IPCCmd.h"
#include "IPCMessage.h"

class IPCHandler
{
public:
    IPCHandler();
    virtual ~IPCHandler() {} 

    bool OnMessage(IPCMsgPtr pMsg);

protected:
    bool Date(IPCMsgPtr pMsg);

protected:
    typedef bool (IPCHandler::*CALLBACK)(IPCMsgPtr pMsg);
    std::map<std::string, CALLBACK> m_callback;
};

#endif //__IPC_HANDLER_H__
