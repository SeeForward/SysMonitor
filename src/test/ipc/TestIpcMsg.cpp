#include <iostream>
#include "IPCHandler.h"
#include "IPCMessage.h"

using namespace std;

int main(int argc, const char* argv[])
{
    IPCMsgPtr pMsg = new IPCMessage("IPC_DATE");
    cout << "cmd\t: " << pMsg->Cmd() << endl;
    cout << "reply\t: " << pMsg->Reply() << endl;

    IPCHandler ipc;
    ipc.OnMessage(pMsg);


    cout << "cmd\t: " << pMsg->Cmd() << endl;
    cout << "reply\t: " << pMsg->Reply() << endl;

    return 0;
}