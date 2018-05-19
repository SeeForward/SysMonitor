#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;

int main(int argc, const char* argv[])
{
    io_service service;
    ip::tcp::socket sock(service);

    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 5000);
    sock.connect(ep);

    boost::system::error_code rc;
    sock.write_some(buffer("client msg\n"), rc);
    if (rc)
    {
       cout << "client send msg error" << endl;
       cout << boost::system::system_error(rc).what() << endl;
       return -1;
    }

    char buf[512];
    size_t read = sock.read_some(buffer(buf)); 

    cout << buf << endl;

    return 0;
}
