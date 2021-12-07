extern "C"
{
#include"miracl.h"
}
#include "sm2init.h"
#include "sm2parameter.h"

using namespace SM2;

bool init_mircal(miracl *mip)
{
    mip = mirsys(64,16);
    if(mip == NULL)
    {
        return false;
    }
    mip->IOBASE = 16;
    return true;
}

void init_ecruve(void)
{
    char an[65] = __an__;
    char bn[65] = __bn__;
    char pn[65] = __pn__;
    big a,b,p;//椭圆曲线参数
    a = mirvar(0);
    b = mirvar(0);
    p = mirvar(0);
    cinstr(a,an);
    cinstr(b,bn);
    cinstr(p,pn);

    ecurve_init(a,b,p,MR_PROJECTIVE);//初始化椭圆曲线

    mirkill(a);
    mirkill(b);
    mirkill(p);
}