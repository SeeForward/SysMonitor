#include <iostream>
#include <boost/asio.hpp>
#include "IPCHandler.h"
#include "IPCMessage.h"

using namespace std;
using namespace boost::asio;

int main(int argc, const char* argv[])
{
    io_service service;
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 5000));
    
    bool stop = false;
    while(!stop)
    {
        ip::tcp::socket sock(service);
        acceptor.accept(sock);

        cout << sock.remote_endpoint().address() << endl;

        boost::system::error_code rc;
        char buf[512];
        size_t read = sock.read_some(buffer(buf));
        IPCMsgPtr pMsg = new IPCMessage(buf);

        IPCHandler ipc;
        ipc.OnMessage(pMsg);
        std::string str;
        pMsg->ToStr(str);

        sock.write_some(buffer(str), rc);
        if (rc)
        {
            cout << boost::system::system_error(rc).what() << endl;
            break;
        }
    }

    return 0;
}
