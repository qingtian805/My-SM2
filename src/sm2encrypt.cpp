extern "C"
{
#include "miracl.h"
}
#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include "sm3.h"
#include "sm2encrypt.h"
#include "sm2type.h"
#include "sm2init.h"
#include "sm2calculators.h"
#include "sm2KDF.h"
#include "sm2StreamString.h"
//#include "sm2parameter.h"

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

    bool ret = true;
    byte x1b[32],y1b[32];//p1的比特串形态
    byte x2b[32],y2b[32];//p2的比特串形态
    byte key[messagelen];//KDF生成的密钥t，也作为加密后的C2
    byte hash[64+messagelen];//等待hash的数据，用于计算C3和KDF
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
            ret = false;
            goto EXIT_FE;
        }
        //step 4
        calP2(k,xB,yB,x2,y2);
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
    memcpy(hash+32,message,messagelen);
    memcpy(hash+32+messagelen,y2b,32);
    sm3(hash,messagelen+64,C3);
    //step 8
    emessage[0] = 4;
    memcpy(emessage+1,x1b,32);
    memcpy(emessage+33,y1b,32);

    memcpy(emessage+65,key,messagelen);

    memcpy(emessage+messagelen+65,C3,32);

EXIT_FE:
    mirkill(k);
    mirkill(x1);
    mirkill(y1);
    mirkill(xB);
    mirkill(yB);
    mirkill(x2);
    mirkill(y2);
    mirexit();
    return ret;
}

bool decryptMessage(char* emessage,int emessagelen,char* dBn,char* message)
{
    miracl *mip;
    if(! init_miracl(mip))
    {
        cout << "MIRACL INIT FALTAL" << endl;
    }
    init_ecruve();

    int messagelen = emessagelen-97;//原信息长度
    bool ret = true;
    char x1n[65];
    char y1n[65];
    byte x2b[32];
    byte y2b[32];
    byte hash[64+messagelen];//等待hash计算的数值
    byte key[messagelen];//KDF生成的密钥t，也作为解密后的C2
    byte u[32];
    big x1,y1;
    big dB;
    big x2,y2;
    epoint* p1;
    x1 = mirvar(0);
    y1 = mirvar(0);
    dB = mirvar(0);
    x2 = mirvar(0);
    y2 = mirvar(0);
    p1 = epoint_init();

    cinstr(dB,dBn);

    //step 1 功能：取出曲线点并验证有效性 等待被替换
    switch (emessage[0])
    {
    case 4:
        streamToString((byte*)emessage+1,32,x1n);
        streamToString((byte*)emessage+33,32,y1n);
        cinstr(x1,x1n);
        cinstr(y1,y1n);
        break;
    
    default:
        cout << "unsupported point format!" << endl;
        ret = false;
        goto EXIT_FD;
        break;
    }

    if(! epoint_set(x1,y1,0,p1))
    {
        cout << "ERROR! Point is not on your ecruve!" << endl;
        ret = false;
        goto EXIT_FD;
    }

    //step 2
    if(point_at_infinity(p1))
    {
        cout << "ERROR! Using invalid public key!" << endl;
        ret = false;
        goto EXIT_FD;
    }

    //step 3
    calP2(dB,x1,y1,x2,y2);
    big_to_bytes(0,x2,(char*)x2b,FALSE);
    big_to_bytes(0,y2,(char*)y2b,FALSE);
    //step 4
    memcpy(hash,x2b,32);
    memcpy(hash+32,y2b,32);
    KDF((char*)hash,64,messagelen,(char*)key);
    //step 5 and some of step 6
    for(int i = 0;i<messagelen;i++)//hash[32~messagelen+32] = message
    {
        hash[32+i] = emessage[65+i] ^ key[i];
    }
    //step 6
    //memcpy(hash+32,emessage,emessagelen);
    memcpy(hash+messagelen+32,y2b,32);//hash + 32 + emessagelen - 97
    sm3(hash,messagelen+64,u);
    if(memcmp(u,emessage+emessagelen-32,32) != 0)
    {
        cout << "ERROR! Message may be modified!" << endl;
        ret = false;
        goto EXIT_FD;
    }
    //step 7
    memcpy(message,hash+32,messagelen);

EXIT_FD:
    mirkill(x1);
    mirkill(y1);
    mirkill(dB);
    mirkill(x2);
    mirkill(y2);
    epoint_free(p1);
    return ret;
}