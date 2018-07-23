#include "ServerCenter.h"
 
void ServerCenter::Add(ServerPtr pServer)
{
    m_servers[m_id] = pServer;
    ++m_id;
}

void ServerCenter::Del(ServerPtr pServer)
{
    
}
