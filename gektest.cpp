#include "sm2keymgr.h"
#include <iostream>

using namespace std;

int main(void)
{
    char d[65];
    char Pxn[65], Pyn[65];

    cout << "-----------Key Pair Generating test------------" << endl;

    if(gen_keypair(d,Pxn,Pyn))
        cout << "Key pair generating failure" << endl;

    cout << "Private Key:\t" << d << endl;
    cout << "Public X Key:\t" << Pxn << endl;
    cout << "Public Y Key:\t" <<Pyn << endl;

    cout << "\n";
    cout << "-------------Key Pair Verrify test-------------" << endl;

    switch (verify_pubkey(Pxn, Pyn)){
    case true:
        cout << "verify succeed" << endl;
        break;
    case false:
        cout << "verify failure" << endl;
        break;
    case -1:
        cout << "ERROR!" << endl;
        break;

    default:
        break;
    }

    return 0;
}