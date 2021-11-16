extern "C"
{
#include"miracl.h"
}
#include"sm2.hpp"
#include<time.h>
#include<math.h>
#include<iostream>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include"sm3.c"

using namespace SM2;

void SM2::calrt(big er,big x1s,big rt)
{
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

void SM2::calP1(big k,big x1)
{
    big xG,yG;//椭圆曲线基点
    big a,b,p;//椭圆曲线参数
    big y1;//无用的y1
    epoint *g,*p1;

    xG = mirvar(0);//初始化参数
    yG = mirvar(0);
    a = mirvar(0);
    b = mirvar(0);
    p = mirvar(0);
    y1 = mirvar(0);
    cinstr(xG,xGn);
    cinstr(yG,yGn);
    cinstr(a,an);
    cinstr(b,bn);
    cinstr(p,pn);

    ecurve_init(a,b,p,MR_PROJECTIVE);//初始化椭圆曲线
    g = epoint_init();
    p1 = epoint_init();
    epoint_set(xG,yG,0,g);

    ecurve_mult(k,g,p1);//p=[k]g
    epoint_get(p1,x1,y1);//取出参数

    mirkill(xG);//函数清理
    mirkill(yG);
    mirkill(a);
    mirkill(b);
    mirkill(p);
    mirkill(y1);
    epoint_free(g);
    epoint_free(p1);
}

void SM2::cal_P1(big s,big t,big xA,big yA,big x1)
{
    big xG,yG;//椭圆曲线基点
    big a,b,p;//椭圆曲线参数
    big y1;//无用的_y1
    epoint *g,*pA,*p1;

    xG = mirvar(0);//初始化参数
    yG = mirvar(0);
    a = mirvar(0);
    b = mirvar(0);
    p = mirvar(0);
    y1 = mirvar(0);
    cinstr(xG,xGn);
    cinstr(yG,yGn);
    cinstr(a,an);
    cinstr(b,bn);
    cinstr(p,pn);

    ecurve_init(a,b,p,MR_PROJECTIVE);//初始化椭圆曲线
    g = epoint_init();
    pA = epoint_init();
    p1 = epoint_init();
    epoint_set(xG,yG,0,g);//g = (xG,yG)
    epoint_set(xA,yA,0,pA);//pA = (xA,yA)
    
    ecurve_mult2(s,g,t,pA,p1);//p1 = [s]g + [t]pA
    epoint_get(p1,x1,y1);

    mirkill(xG);//函数清理
    mirkill(yG);
    mirkill(a);
    mirkill(b);
    mirkill(p);
    mirkill(y1);
    epoint_free(g);
    epoint_free(pA);
    epoint_free(p1);
}