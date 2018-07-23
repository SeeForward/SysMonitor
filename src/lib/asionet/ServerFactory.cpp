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

    AsyncRecv(&sc);
}

void ServerFactory::AsyncRecv(SocketCtx *pSC)
{
    cout << "factory recv" << endl;
    cout << "Accept:" << pSC->m_spS->remote_endpoint().address() << endl;

    async_read(*(pSC->m_spS.Get()), AsioBuffer(&(pSC->m_len), sizeof(pSC->m_len)), 
                    boost::bind(&ServerFactory::HeadRecvHandler, this, pSC, ErrorPlaceholder));
}

void ServerFactory::HeadRecvHandler(SocketCtx *pSC, const ErrorCode &ec)
{
    cout << "factory recv head" << endl;
    if (ec)
    {
        OnError(ec);
        return ;
    }
    cout << "body len:" << pSC->m_len << endl;

    pSC->m_pData = new uint8_t[pSC->m_len + sizeof(pSC->m_len)];
    memcpy(pSC->m_pData + sizeof(pSC->m_len), &(pSC->m_len), sizeof(pSC->m_len));
    async_read(*(pSC->m_spS.Get()), AsioBuffer(pSC->m_pData + sizeof(pSC->m_len), pSC->m_len),
                    boost::bind(&ServerFactory::BodyRecvHandler, this, pSC, ErrorPlaceholder));
}

void ServerFactory::BodyRecvHandler(SocketCtx *pSC, const ErrorCode &ec)
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
    NetMessage negoMsg;
    negoMsg.FromStr(std::string((char*)(sc.m_pData + sizeof(uint32_t)), sc.m_len));

    std::string s;
    negoMsg.ToStr(s);
    cout << "recv:" << s << endl;

    cout << "Create Server" << endl;
    std::map<int, CREATEFUNC>::iterator iter = m_entrance.find(negoMsg.Code());
    if (m_entrance.end() != iter)
    {
        cout << "ServerFactory:" << sc.m_spS->remote_endpoint().address() << endl;

        ServerPtr spServer((iter->second)(sc.m_spS));
        if (spServer)
        {
            Singleton<ServerCenter>::Instance().Add(spServer);
        }
    }
    m_socks.erase(sc.m_id);
}

bool ServerFactory::Register(int serverType, CREATEFUNC create)
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
