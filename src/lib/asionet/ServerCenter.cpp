#include "ServerCenter.h"


ServerPtr ServerCenter::Get(int serverType, SocketPtr spSock)
{
    std::map<int, CREATEFUNC>::iterator iter = m_entrance.find(serverType);
    if (m_entrance.end() != iter)
    {
        cout << "ServerFactory:" << spSock->remote_endpoint().address() << endl;

        ServerPtr spServer((iter->second)(spSock));
        if (spServer)
        {
            Add(spServer);
            return spServer;
        }
    }
    return NULL;
}    

bool ServerCenter::Register(int serverType, CREATEFUNC create)
{
	m_entrance[serverType] = create;
	return true;
}
