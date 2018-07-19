#include <boost/asio.hpp>

#include <iostream>

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
    	//char buf[512];
    	//size_t read = sock.read_some(buffer(buf)); 
    	//cout << buf << endl;

    	uint32_t num = 0;
    	size_t read = sock.read_some(buffer((char*)&num, sizeof(num))); 
	cout << num << endl;

        sock.write_some(buffer("server msg"), rc);
        if (rc)
        {
            cout << boost::system::system_error(rc).what() << endl;
            break;
        }
    }

    return 0;
}
