extern "C"
{
#include "miracl.h"
}
#include <iostream>
#include <memory.h>
#include "sm3.h"
#include "sm2sigment.h"
#include "sm2type.h"
#include "sm2init.h"
#include "sm2calculators.h"
#include "sm2parameter.h"
#include "sm2StreamString.h"

using namespace std;
using namespace SM2;

void genZA(char *ID,int IDlen,char *xAn,char *yAn,char* ZA)
{
    //椭圆曲线算法参数赋值
    char an[65] = __an__;
    char bn[65] = __bn__;
    char xGn[65] = __xGn__;
    char yGn[65] = __yGn__;
    //函数内部数值
    char ENTLA[2];//可否使用union替代？
    char info[IDlen + 194] = "";//32 * 6 + 2 = 194
    int entlena = IDlen * 8;
    //将数据转换为字节串
    stringToStream(an,64,(byte*)an);
    stringToStream(bn,64,(byte*)bn);
    stringToStream(xGn,64,(byte*)xGn);
    stringToStream(yGn,64,(byte*)yGn);
    stringToStream(xAn,64,(byte*)xAn);
    stringToStream(yAn,64,(byte*)yAn);
    //计算ZA
    ENTLA[0] = entlena & 0xFF;
    ENTLA[1] = (entlena & 0xFF00) >> 8;
    memcpy(info,ENTLA,2);
    memcpy(info+2,ID,IDlen);
    memcpy(info+IDlen+2,an,32);
    memcpy(info+IDlen+34,bn,32);
    memcpy(info+IDlen+66,xGn,32);
    memcpy(info+IDlen+98,yGn,32);
    memcpy(info+IDlen+130,xAn,32);
    memcpy(info+IDlen+162,yAn,32);
    sm3((byte*)info,IDlen+194,(byte*)ZA);
    streamToString((byte*)ZA,32,ZA);
}

int genSignment(char* ZAn,char* dAn,unsigned char* message,int messagelen,char* rn,char* sn)
{
    //init MIRACL
    miracl *mip;
    if(! init_miracl(mip))
    {
        cout << "MIRACL INIT FALTAL" << endl;
        return -1;
    }
    init_ecruve();

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
    gen_Message(ZAn,message,messagelen,_message);

    //step 2
    calE(_message,messagelen+32,e);

    do
    {
        do
        {
            //step 3
            genRandom(k);

            //step 4
            calP1(k,x1,NULL);

            //step 5
            calrt(e,x1,r);

            add(r,k,rk);//rk = r + k
        }while( is_zero(r) || is_n(rk));
        //step 6
        cals(k,dA,r,s);
    }while(is_zero(s));

    big_to_bytes(0,r,rn,FALSE);
    big_to_bytes(0,s,sn,FALSE);
    streamToString((byte*)rn,32,rn);
    streamToString((byte*)sn,32,sn);

EXIT_FS:
    mirkill(rk);
    mirkill(e);
    mirkill(k);
    mirkill(x1);
    mirkill(r);
    mirkill(s);
    mirkill(dA);
    mirexit();
    return 0;
}

bool verifySignment(char* ZAn,char* xAn,char* yAn,unsigned char* message,int messagelen,char* rn,char* sn)
{
    miracl *mip;
    if(! init_miracl(mip))
    {
        cout << "MIRACL INIT FALTAL" << endl;
        return -1;
    }
    init_ecruve();

    int ret = 0;
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
    if(! in_1n1(r))//r 不在 [1,n-1] 则不通过
    {
        goto EXIT_FV;
    }

    //step2
    if(! in_1n1(s))//s 不在 [1,n-1] 则不通过
    {
        goto EXIT_FV;
    }

    //step3
    gen_Message(ZAn,message,messagelen,_message);

    //step4
    calE(_message,messagelen+32,e);

    //step5
    calrt(r,s,t);
    if(is_zero(t))//t = 0，则不通过
    {
        goto EXIT_FV;
    }

    //step6
    cal_P1(s,t,xA,yA,x1,NULL);

    //step7
    calrt(e,x1,R);
    if(! is_equal(R,r))//R != r，则不通过
    {
        goto EXIT_FV;
    }

    ret = 1;
EXIT_FV:
    mirkill(r);
    mirkill(s);
    mirkill(e);
    mirkill(t);
    mirkill(xA);
    mirkill(yA);
    mirkill(x1);
    mirkill(R);
    mirexit();
    return ret;
}