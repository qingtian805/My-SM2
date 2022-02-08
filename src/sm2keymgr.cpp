extern "C"
{
#include "miracl.h"
}
#include "sm2keymgr.h"
#include "sm2calculators.h"
#include "sm2init.h"
#include <iostream>
#include "sm2StreamString.h"
#include "sm2type.h"

using namespace std;
using namespace SM2;

void SM2::gen_keypair(big d,big Px,big Py)
{
    genRandom2(d);//生成处于[1,n-2]的随机数

    calP1(d,Px,Py);
}

bool gen_keypair(char* dn, char*Pxn, char* Pyn)
{
    miracl *mip;
    if(! init_miracl(mip))
    {
        cout << "MIRACL INIT FALTAL" << endl;
        return false;
    }
    init_ecruve();

    big d;
    big Px;
    big Py;
    d = mirvar(0);
    Px = mirvar(0);
    Py = mirvar(0);

    SM2::gen_keypair(d,Px,Py);

    big_to_bytes(0,d,dn,FALSE);
    big_to_bytes(0,Px,Pxn,FALSE);
    big_to_bytes(0,Py,Pyn,FALSE);
    streamToString((byte*)dn,32,dn);
    streamToString((byte*)Pxn,32,Pxn);
    streamToString((byte*)Pyn,32,Pyn);

EXIT_F:
    mirkill(d);
    mirkill(Px);
    mirkill(Py);
    mirexit();
    return true;
}

bool SM2::verify_pubkey(big Px, big Py)
{
    
}