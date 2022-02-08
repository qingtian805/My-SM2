#include "sm2keymgr.h"
#include <iostream>

using namespace std;

int main(void)
{
    char d[65];
    char Pxn[65], Pyn[65];

    if(! gen_keypair(d,Pxn,Pyn))
    {
        cout << "Key pair generating failure" << endl;
    }

    cout << "Private Key:\t" << d << endl;
    cout << "Public X Key:\t" << Pxn << endl;
    cout << "Public Y Key:\t" <<Pyn << endl;

    return 0;
}