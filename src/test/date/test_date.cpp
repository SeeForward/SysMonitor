
#include "Date.h"
#include "system_tool.h"

#include <iostream>

using namespace std;

void test_date();

int main(int argc, const char* argv[])
{
	test_date();
	return 0;
}

void test_date()
{
	Date date = Date::Now();
	cout << date.Format() << endl;

	SleepMs(1000 * 2);

	Date date2 = Date::Now();
	cout << date2.Format() << endl;
}
