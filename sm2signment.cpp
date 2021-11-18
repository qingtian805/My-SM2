extern "C"
{
#include"miracl.h"
}
#include<time.h>
#include<iostream>
#include<stdlib.h>
#include<memory.h>
#include"sm3.c"
#include"sm2.hpp"

using namespace std;
using namespace SM2;

void genZA(char *ID,int IDlen,char *xAn,char *yAn,char* ZA)
{
    char ENTLA[2];
    char info[IDlen + 387] = "";//64 * 6 + 2 + 1 = 387
    int entlena = IDlen * 8;
    ENTLA[0] = entlena & 0xFF;
    ENTLA[1] = (entlena & 0xFF00) >> 8;
    memcpy(info,ENTLA,2);
    memcpy(info+2,ID,IDlen);
    memcpy(info+IDlen+2,an,64);
    memcpy(info+IDlen+66,bn,64);
    memcpy(info+IDlen+130,xGn,64);
    memcpy(info+IDlen+194,yGn,64);
    memcpy(info+IDlen+258,xAn,64);
    memcpy(info+IDlen+322,yAn,64);
    sm3((byte*)info,IDlen+386,(byte*)ZA);
    streamToString((byte*)ZA,32,ZA);
}

void genSignment(char* ZAn,char* dAn,unsigned char* message,int messagelen,char* rn,char* sn)
{
    //init MIRACL
    miracl *mip;
    mip = mirsys(64,16);//16位
    if(mip == NULL)
    {
        cout << "MIRACL INIT FALTAL" << endl;
        exit(0);
    }
    big rk;//r+k
    big e;
    big k;
    big x1;
    big r;
    big s;
    big dA;
    rk = mirvar(0);
    e = mirvar(0);
    k = mirvar(0);
    x1 = mirvar(0);
    r = mirvar(0);
    s = mirvar(0);
    dA = mirvar(0);
    cinstr(dA,dAn);
    byte _message[messagelen + 32];

    //step 1
    SM2::gen_Message(ZAn,message,messagelen,_message);

    //step 2
    calE(_message,messagelen+32,e);

    do
    {
        do
        {
            //step 3
            genRandom(k);

            //step 4
            calP1(k,x1);

            //step 5
            calrt(e,x1,r);

            add(r,k,rk);//rk = r + k
        }while( is_zero(r) || is_n(rk));
        //step 6
        cals(k,dA,r,s);
    }while(is_zero(s));

    big_to_bytes(0,r,rn,FALSE);
    streamToString((byte*)rn,32,rn);
    big_to_bytes(0,s,sn,FALSE);
    streamToString((byte*)sn,32,sn);

    mirkill(rk);
    mirkill(e);
    mirkill(k);
    mirkill(x1);
    mirkill(r);
    mirkill(s);
    mirkill(dA);
    mirexit();
}

bool verifySignment(char* ZAn,char* xAn,char* yAn,unsigned char* message,int messagelen,char* rn,char* sn)
{
    big r,s;
    big e;
    big t;
    big xA,yA,x1;
    big R;
    r = mirvar(0);
    s = mirvar(0);
    e = mirvar(0);
    t = mirvar(0);
    xA = mirvar(0);
    yA = mirvar(0);
    x1 = mirvar(0);
    R = mirvar(0);
    cinstr(r,rn);
    cinstr(s,sn);
    cinstr(xA,xAn);
    cinstr(yA,yAn);
    byte _message[messagelen+32];

    //step1
    if(notIn_1n(r))
    {
        return false;
    }

    //step2
    if(notIn_1n(s))
    {
        return false;
    }

    //step3
    gen_Message(ZAn,message,messagelen,_message);

    //step4
    calE(_message,messagelen+32,e);

    //step5
    calrt(r,s,t);
    if(is_zero(t))
    {
        return false;
    }

    //step6
    cal_P1(s,t,xA,yA,x1);

    //step7
    calrt(e,x1,R);
    if(is_equal(R,r))
    {
        return true;
    }
    else
    {
        return false;
    }
}



