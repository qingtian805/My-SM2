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

bool encryptMessage(char* message,int messagelen,char* xBn,char* yBn,char* emessage)
{
    miracl *mip;
    if(! init_miracl(mip))
    {
        cout << "MIRACL INIT FALTAL" << endl;
    }
    init_ecruve();

    byte x1b[32],y1b[32];//p1的比特串形态
    byte x2b[32],y2b[32];//p2的比特串形态
    byte key[messagelen];//KDF生成的密钥，也作为加密后的M
    byte hash[64+messagelen];//等待hash的数据，用于计算C3
    byte C3[32];
    big k;//随机数
    big x1,y1;
    big xB,yB;//B的公钥
    big x2,y2;
    epoint* pB;//B的公钥
    k = mirvar(0);
    x1 = mirvar(0);
    y1 = mirvar(0);
    xB = mirvar(0);
    yB = mirvar(0);
    x2 = mirvar(0);
    y2 = mirvar(0);

    cinstr(xB,xBn);//初始化pB用于判断是否位于无限远点
    cinstr(yB,yBn);

    do
    {
        //step 1
        genRandom(k);
        //step 2
        calP1(k,x1,y1);
        big_to_bytes(0,x1,(char*)x1b,FALSE);
        big_to_bytes(0,y1,(char*)y1b,FALSE);
        //step 3
        epoint_set(xB,yB,0,pB);
        if(point_at_infinity(pB))
        {
            cout << "ERROR！ B`s public key is invalid!" << endl;
            return false;
        }
        //step 4
        calP1(k,x2,y2);
        big_to_bytes(0,x2,(char*)x2b,FALSE);
        big_to_bytes(0,y2,(char*)y2b,FALSE);
        //step 5
        memcpy(hash,x2b,32);
        memcpy(hash+32,y2b,32);
        KDF((char*)hash,64,messagelen*8,(char*)key);
    }while(is_allzero(key,messagelen));
    //step 6
    for(int i=0;i<messagelen;i++)
    {
        key[i] = message[i] ^ key[i];
    }
    //step 7
    memcpy(hash,x2b,32);
    memcpy(hash+32,message,messagelen);
    memcpy(hash+32+messagelen,y2b,32);
    sm3(hash,messagelen+64,C3);
    //step 8
    emessage[0] = 4;
    memcpy(emessage+1,x1b,32);
    memcpy(emessage+33,y1b,32);

    memcpy(emessage+65,key,messagelen);

    memcpy(emessage+messagelen+65,C3,32);
}

bool decryptMessage(char* emessage,int emessagelen,char* dBn,char* message)
{
    miracl *mip;
    if(! init_miracl(mip))
    {
        cout << "MIRACL INIT FALTAL" << endl;
    }
    init_ecruve();

}