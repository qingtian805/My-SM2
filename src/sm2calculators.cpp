extern "C"
{
#include "miracl.h"
}
#include "sm2calculators.h"
#include "sm2parameter.h"
#include "sm2StreamString.h"
#include <time.h>
#include <memory.h>
#include "sm3.h"

using namespace SM2;

void SM2::gen_Message(char* ZAn,byte* message,int mlen,byte* _message)
{
    byte ZA[32];
    stringToStream(ZAn,64,ZA);
    memcpy(_message,ZAn,32);//偏移0-31复制为ZA
    memcpy(_message+32,message,mlen);//偏移32-++复制为message
}

void SM2::calE(byte* _message,int _mlen,big e)
{
    char en[65];
    sm3(_message,_mlen,(byte*)en);
    streamToString((byte*)en,32,en);
    cinstr(e,en);
}

void SM2::genRandom(big k)
{
    char nn[65] = __nn__; 
    big n;
    n = mirvar(0);

    cinstr(n,nn);

    do
    {
    irand(time(NULL)+SEED);
    bigrand(n,k);// 0 <= k < n
    }while(is_zero(k));//是0则重新生成

    mirkill(n);
}

void SM2::genRandom2(big k)
{
    char nn[65] = __nn__;
    big n;
    n = mirvar(0);
    
    stringsub1(nn,65,nn);//n=n-1

    cinstr(n,nn);
    
    do
    {
    irand(time(NULL)+SEED);
    bigrand(n,k);// 0 <= k < n, n=n-1
    }while(is_zero(k));//是0则重新生成

    mirkill(n);
}

void SM2::calrt(big er,big x1s,big rt)
{
    char nn[65] = __nn__;
    big n,tmp;

    n = mirvar(0);
    tmp = mirvar(0);
    cinstr(n,nn);

    add(x1s,er,tmp);//tmp = x1 + e
    divide(tmp,n,n);//tmp = tmp mod n
    copy(tmp,rt);//r = tmp

    mirkill(n);
    mirkill(tmp);
}

void SM2::cals(big k,big dA,big r,big s)
{
    char nn[65] = __nn__;
    big tmp,und;
    big n;

    tmp = mirvar(0);
    und = mirvar(0);
    n = mirvar(0);
    cinstr(n,nn);
    
    incr(dA,1,und);//und = 1+dA
    xgcd(und,n,und,und,und);//und = und^-1 mod n
    mad(r,dA,dA,n,n,tmp);//tmp = (r * dA) mod n
    if(mr_compare(k,tmp) >= 0)//tmp = k - tmp
    {
        subtract(k,tmp,tmp);
    }
    else
    {
        subtract(n,tmp,tmp);
        add(k,tmp,tmp);
    }
    mad(tmp,und,und,n,n,tmp);//tmp = (tmp * und) mod n
    copy(tmp,s);
    mirkill(tmp);
    mirkill(und);
    mirkill(n);
}

void SM2::calP1(big k,big x1,big y1)
{
    char xGn[65] = __xGn__;
    char yGn[65] = __yGn__;
    big xG,yG;//椭圆曲线基点
    epoint *g,*p1;
    bool yisnull = false;
    

    if(y1 == NULL)
    {
        y1 = mirvar(0);
        yisnull = true;
    }

    xG = mirvar(0);//初始化参数
    yG = mirvar(0);
    cinstr(xG,xGn);
    cinstr(yG,yGn);

    g = epoint_init();
    p1 = epoint_init();
    epoint_set(xG,yG,0,g);

    ecurve_mult(k,g,p1);//p=[k]g
    epoint_get(p1,x1,y1);//取出参数

    if(yisnull){
        mirkill(y1);
    }
    mirkill(xG);//函数清理
    mirkill(yG);

    epoint_free(g);
    epoint_free(p1);
}

void SM2::cal_P1(big s,big t,big xA,big yA,big x1,big y1)
{
    char xGn[65] = __xGn__;
    char yGn[65] = __yGn__;
    big xG,yG;//椭圆曲线基点
    epoint *g,*pA,*p1;
    bool yisnull = false;

    if(y1 == NULL)
    {
        yisnull = true;
        y1 = mirvar(0);
    }

    xG = mirvar(0);//初始化参数
    yG = mirvar(0);
    cinstr(xG,xGn);
    cinstr(yG,yGn);

    g = epoint_init();
    pA = epoint_init();
    p1 = epoint_init();
    epoint_set(xG,yG,0,g);//g = (xG,yG)
    epoint_set(xA,yA,0,pA);//pA = (xA,yA)
    
    ecurve_mult2(s,g,t,pA,p1);//p1 = [s]g + [t]pA
    epoint_get(p1,x1,y1);

    if(yisnull)
    {
        mirkill(y1);
    }
    mirkill(xG);//函数清理
    mirkill(yG);

    epoint_free(g);
    epoint_free(pA);
    epoint_free(p1);
}

void SM2::calP2(big k,big xB,big yB,big x2,big y2)
{
    epoint *pB,*p2;

    pB = epoint_init();
    p2 = epoint_init();

    epoint_set(xB,yB,0,pB);
    ecurve_mult(k,pB,p2);

    epoint_get(p2,x2,y2);

    epoint_free(pB);
    epoint_free(p2);
}

bool SM2::in_1n1(big __x)
{
    char nn[65] = __nn__;
    big cmp0;//0
    big cmpn;//n
    bool res1,res2;
    cmp0 = mirvar(0);
    cmpn = mirvar(0);
    cinstr(cmpn,nn);
    res1 = (mr_compare(__x,cmp0) > 0);//x > 0
    res2 = (mr_compare(__x,cmpn) < 0);//x < n
    mirkill(cmp0);
    mirkill(cmpn);
    return res1&&res2; // >=0 && <n
}

bool SM2::is_n(big __x)
{
    char nn[65] = __nn__;
    big cmpn;
    bool res;
    cmpn = mirvar(0);
    cinstr(cmpn,nn);
    res = (mr_compare(__x,cmpn) == 0);// == n
    mirkill(cmpn);
    return res;// == n
}

bool SM2::is_zero(big __x)
{
    big cmp0;
    bool res;
    cmp0 = mirvar(0);
    res = (mr_compare(__x,cmp0) == 0);// == 0
    mirkill(cmp0);
    return res;
}

bool SM2::is_equal(big __x,big __y)
{
    return (mr_compare(__x,__y) == 0);
}

bool SM2::is_allzero(byte* Bs,int lenB)
{
    for(int i = 0;i<lenB;i++)
    {
        if(*(Bs+i) == 0){//如果当前为0,则检查下一位
            continue;
        }
        else//如果当前位不是0,则返回false并退出函数
        {
            return false;
        }
    }
    return true;//如果检查结束，则全部为0
}