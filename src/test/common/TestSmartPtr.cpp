#include <iostream>
#include "SmartPtr.h"

using namespace std;

int main(int argc, const char * argv[])
{

    SmartPtr<int> sp(new int(1));

    cout << sp.Value() << endl;

    {
        SmartPtr<int> sp2(sp);

        cout << sp.Value() << endl;
        cout << sp2.Value() << endl;
    }

    cout << sp.Value() << endl;

    {
        SmartPtr<int> sp3;
        cout << sp3.Value() << endl;

        sp3 = sp;

        cout << sp.Value() << endl;
        cout << sp3.Value() << endl;
    }

    cout << sp.Value() << endl;

    cout << *sp << endl;

    *sp = 2;

    ++(*sp);

    cout << *sp << endl;

    return 0;
}
