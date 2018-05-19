#include <boost/asio.hpp>

#include <iostream>

using namespace std;
using namespace boost::asio;

int main(int argc, const char* argv[])
{
    ip::address addr = ip::address::from_string("127.0.0.1");

    cout << addr.to_string() << endl;


    return 0;
}