#include "Level.h"

#include <iostream>

using namespace std;

void test_log();

int main(int argc, const char* argv[])
{
	test_log();
	return 0;
}

void test_log()
{
    Level level;
    cout << level.Str() << endl;
}
