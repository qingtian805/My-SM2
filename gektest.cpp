#include "sm2keymgr.h"
#include "sm2config.h"
#include <string.h>
#include <iostream>

using namespace std;
//标准第四章测试参数

char pn[] = "8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3";
char an[] = "787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498";
char bn[] = "63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A";
char nn[] = "8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7";
char Gxn[] = "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D";
char Gyn[] = "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2";


int main(void)
{
    char d[65];
    char Pxn[65], Pyn[65];

    sm2cfg *sm2p;
    sm2p = sm2init();
    if( sm2p == NULL ){
        cout << "init failed!" << endl;
        return 0;
    }

    strcpy(sm2p->A,an);
    strcpy(sm2p->B,bn);
    strcpy(sm2p->P,pn);
    strcpy(sm2p->N,nn);
    strcpy(sm2p->Gx,Gxn);
    strcpy(sm2p->Gy,Gyn);

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

    sm2exit(sm2p);

    return 0;
}