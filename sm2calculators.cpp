extern "C"
{
#include"miracl.h"
}
#include"sm2.hpp"
#include<time.h>
#include<iostream>
#include<memory.h>
#include"sm3.c"

using namespace SM2;
using namespace std;

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

void SM2::calP1(big k,big x1,big y1)
{
    big xG,yG;//椭圆曲线基点
    epoint *g,*p1;
    bool yisnull = false;

    if(y1 == NULL){
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

void SM2::cal_P1(big s,big t,big xA,big yA,big x1)
{
    big xG,yG;//椭圆曲线基点
    big y1;//无用的_y1
    epoint *g,*pA,*p1;

    xG = mirvar(0);//初始化参数
    yG = mirvar(0);
    y1 = mirvar(0);
    cinstr(xG,xGn);
    cinstr(yG,yGn);

    g = epoint_init();
    pA = epoint_init();
    p1 = epoint_init();
    epoint_set(xG,yG,0,g);//g = (xG,yG)
    epoint_set(xA,yA,0,pA);//pA = (xA,yA)
    
    ecurve_mult2(s,g,t,pA,p1);//p1 = [s]g + [t]pA
    epoint_get(p1,x1,y1);

    mirkill(xG);//函数清理
    mirkill(yG);
    mirkill(y1);
    epoint_free(g);
    epoint_free(pA);
    epoint_free(p1);
}

bool SM2::notIn_1n(big __x)
{
    big cmp0;//0
    big cmpn;//n
    bool res1,res2;
    cmp0 = mirvar(0);
    cmpn = mirvar(0);
    cinstr(cmpn,nn);
    res1 = (mr_compare(__x,cmp0) == 0);// == 0
    res2 = (mr_compare(__x,cmpn) >= 0);// >= n
    mirkill(cmp0);
    mirkill(cmpn);
    return res1||res2; // ==0 || >=n
}

bool SM2::is_n(big __x)
{
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



//DEBUG 在sm2.hpp中启用
#if DEBUG
void SM2::printDebugResult(big tmp,char* shouldbe){
    char out[65];
    big_to_bytes(0,tmp,out,FALSE);
    streamToString((byte*)out,32,out);
    std::cout << "Your value:" << out << "\nShould be: " << shouldbe << endl;
}
int main(){
    miracl *mip = mirsys(64,16);
    if(mip == NULL){
        cout << "MIRACL init Faltal" << endl;
        exit(0);
    }
    mip -> IOBASE = 16;

    big k,x1;
    k = mirvar(0);
    x1 = mirvar(0);
    big e,r,rk;
    e = mirvar(0);
    r = mirvar(0);
    rk = mirvar(0);
    big dA,s;
    dA = mirvar(0);
    s = mirvar(0);

    do{
        do{
            //step 4
            cinstr(k,kn);
            calP1(k,x1,NULL);
            printDebugResult(x1,x1n);

            //step 5
            cinstr(e,en);
            calrt(e,x1,r);

            add(r,k,rk);//rk = r + k
        }while( is_zero(r) || is_n(rk));
        printDebugResult(r,rn);
        //step 6
        cinstr(dA,dAn);
        cals(k,dA,r,s);
    }while(is_zero(s));
    printDebugResult(s,sn);

    mirkill(rk);
    mirkill(k);
    mirkill(dA);

    bool res;
    if(notIn_1n(r)){
        res = false;
        cout << res << endl;
    }
    if(notIn_1n(s)){
        res = false;
        cout << res << endl;
    }
    big t;
    t = mirvar(0);
    calrt(r,s,t);
    if(is_zero(t)){
        res = false;
        cout << res << endl;
    }
    big xA,yA;
    xA = mirvar(0);
    yA = mirvar(0);
    cinstr(xA,xAn);
    cinstr(yA,yAn);
    cal_P1(s,t,xA,yA,x1);
    big R;
    R = mirvar(0);
    calrt(e,x1,R);
    if(is_equal(R,r)){
        res = true;
        cout << res << endl;
    }
    else{
        return false;
        cout << res << endl;
    }

    return 0;
}
#endif