#include <iostream>
#include "json.h"

using namespace std;


int main(int argc, const char* argv[])
{
    int count = 5;
    Json::Value root;
    Json::Value item;
    root["array"] = Json::Value::nullRef;
    for (int i = 0; i < count; i++)
    {
        item["a"] = i;
        item["b"] = i;
        root["array"][i] = item;
    }

    string jvStr = root.toStyledString();

	Json::Reader reader;
	Json::Value v;

	try
	{
		if(!reader.parse(jvStr, v))
		{
            cerr << "parse error" << endl;
            return -1;
		}
	}
	catch (std::exception &ex)
	{
		cerr << ex.what() << endl;
	}

    cout << v.toStyledString() << endl;

	return 0;
}
