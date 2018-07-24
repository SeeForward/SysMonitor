#include "ServerFactory.h"
#include "ServerCenter.h"
#include <boost/bind.hpp>

#include <iostream>

using namespace std;

bool ServerFactory::Add(SocketPtr spSock)
{
    SocketCtx &sc = m_socks[m_id];
    sc.m_spS = spSock;
    sc.m_id = m_id;
    ++m_id;

    AsyncRecvType(&sc);
}

void ServerFactory::AsyncRecvType(SocketCtx *pSC)
{
    cout << "factory recv" << endl;
    cout << "Accept:" << pSC->m_spS->remote_endpoint().address() << endl;

    async_read(*(pSC->m_spS.Get()), AsioBuffer(&(pSC->m_type), sizeof(pSC->m_type)), 
                    boost::bind(&ServerFactory::TypeRecvHandler, this, pSC, ErrorPlaceholder));
}

void ServerFactory::TypeRecvHandler(SocketCtx *pSC, const ErrorCode &ec)
{
    cout << "factory recv body" << endl;
    if (ec)
    {
        OnError(ec);
        return ;
    }

    CreateServer(*pSC);
}

void ServerFactory::CreateServer(SocketCtx &sc)
{
    cout << "Create Server" << endl;
    std::map<uint32_t, CREATEFUNC>::iterator iter = m_entrance.find(sc.m_type);
    if (m_entrance.end() != iter)
    {
        cout << "ServerFactory:" << sc.m_spS->remote_endpoint().address() << endl;

        ServerPtr spServer((iter->second)(sc.m_spS));
        if (spServer)
        {
            spServer->SetId(sc.m_id);
            Singleton<ServerCenter>::Instance().Add(spServer);
        }
    }
    m_socks.erase(sc.m_id);
}

bool ServerFactory::Register(uint32_t serverType, CREATEFUNC create)
{
	m_entrance[serverType] = create;
	return true;
}

int ServerFactory::OnError(const ErrorCode &ec)
{
    cout << ec.value() << endl;
    cout << ec.category().name() << endl;
    return -ec.value();
}
