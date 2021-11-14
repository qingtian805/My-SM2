extern "C"{
#include"miracl.h"
}
#include"sm2.hpp"
#include<time.h>
#include<iostream>
#include<stdlib.h>
#include<memory.h>
#include<string.h>
#include"sm3.c"

using namespace std;
using namespace SM2;

void SM2::streamToString(byte* stream,int streamlength, byte* string){
    int pm,pr;
    stream[streamlength * 2] = '\0';
    pr = streamlength * 2 - 1;
    for(pm = streamlength - 1; pm>=0; pm--)
    {
        string[pr] = stream[pm] & 0b00001111;//低4位
        if(string[pr] > 9)
            stream[pr] = string[pr] + 55;//55='A'-10
        else
            string[pr] = string[pr] + '0';
        pr--;
        string[pr] = stream[pm] >> 4;//高4位
        if(string[pr] > 9)
            stream[pr] = string[pr] + 55;//55='A'-10
        else
            string[pr] = string[pr] + '0';
        pr--;
    }
}

void SM2::gen_Message(char* ZA,char* message,char* _message){ 
    strcpy(_message,ZA);
    strcat(_message,message);
}

void SM2::calE(char* _message,big e){
    char en[65];
    sm3((byte*)_message,strlen(_message),(byte*)en);
    streamToString((byte*)en,32,(byte*)en);
    cinstr(e,en);
}

void SM2::genRandom(big k){
    big n;

    n = mirvar(0);
    cinstr(n,nn);
    do{
    irand(time(NULL)+SEED);
    bigrand(n,k);// 0 <= k < n
    }while(is_zero(k));//是0则重新生成

    mirkill(n);
}

void SM2::calrt(big er,big x1s,big rt){
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

//计算s==((1+dA)^-1·(k-r·dA))mod n
void SM2::cals(big k,big dA,big r,big s){
    big tmp,und;
    big n;

    tmp = mirvar(0);
    und = mirvar(0);
    n = mirvar(0);
    cinstr(n,nn);
    
    incr(dA,1,und);//und = 1+dA
    xgcd(und,n,und,und,und);//und = und^-1 mod n
    mad(r,dA,dA,n,n,tmp);//tmp = (r * dA) mod n
    if(mr_compare(k,tmp) >= 0){//tmp = k - tmp
        subtract(k,tmp,tmp);
    }
    else{
        subtract(n,tmp,tmp);
        add(k,tmp,tmp);
    }
    mad(tmp,und,und,n,n,tmp);//tmp = (tmp * und) mod n
    copy(tmp,s);
    mirkill(tmp);
    mirkill(und);
    mirkill(n);
}

void SM2::calP1(big k,big x1){
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

void SM2::cal_P1(big s,big t,big xA,big yA,big x1){
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

bool SM2::notIn_1n(big __x){
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

bool SM2::is_n(big __x){
    big cmpn;
    bool res;
    cmpn = mirvar(0);
    cinstr(cmpn,nn);
    res = (mr_compare(__x,cmpn) == 0);// == n
    mirkill(cmpn);
    return res;// == n
}

bool SM2::is_zero(big __x){
    big cmp0;
    bool res;
    cmp0 = mirvar(0);
    res = (mr_compare(__x,cmp0) == 0);// == 0
    mirkill(cmp0);
    return res;
}

bool SM2::is_equal(big __x,big __y){
    return (mr_compare(__x,__y) == 0);
}

void genZA(char *ID,char *xA,char *yA,char* ZA){
    char ENTLA[3];
    int IDlen = strlen(ID);
    char info[IDlen + 387] = "";//64 * 6 + 2 + 1 = 387
    int entlena = IDlen * 8;
    ENTLA[0] = entlena & 0x0F;
    ENTLA[1] = (entlena & 0xF0) >> 4;
    ENTLA[2] = '\0';
    strcat(info,ENTLA);
    strcat(info,ID);
    strcat(info,an);
    strcat(info,bn);
    strcat(info,xGn);
    strcat(info,yGn);
    strcat(info,xA);
    strcat(info,yA);
    sm3((byte*)info,IDlen+387,(byte*)ZA);
    streamToString((byte*)ZA,32,(byte*)ZA);
}

void genSignment(char* ZA,char* dAn,char* message,int messagelen,char* rn,char* sn){
    //init MIRACL
    miracl *mip;
    mip = mirsys(64,16);//16位
    if(mip == NULL){
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
    char _message[messagelen + 65];

    //step 1
    SM2::gen_Message(ZA,(char*)message,_message);

    //step 2
    calE(_message,e);

    do{
        do{
            //step 3
            genRandom(k);

            //step 4
            calP1(k,x1);

            //step 5
            calrt(e,x1,r);

            add(r,k,rk);//rk = r + k
        }while( is_zero(r) || is_n(rk));
        //step 6
        cinstr(dA,dAn);
        cals(k,dA,r,s);
    }while(is_zero(s));

    big_to_bytes(0,r,rn,FALSE);
    streamToString((byte*)rn,32,(byte*)rn);
    big_to_bytes(0,s,sn,FALSE);
    streamToString((byte*)sn,32,(byte*)sn);

    mirkill(rk);
    mirkill(e);
    mirkill(k);
    mirkill(x1);
    mirkill(r);
    mirkill(s);
    mirkill(dA);
    mirexit();
}

bool verifySignment(char* ZA,char* xAn,char* yAn,char* message,int messagelen,char* rn,char* sn){
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
    char _message[65];

    //step1
    if(notIn_1n(r)){
        return false;
    }

    //step2
    if(notIn_1n(s)){
        return false;
    }

    //step3
    gen_Message(ZA,message,_message);

    //step4
    calE(_message,e);

    //step5
    calrt(r,s,t);
    if(is_zero(t)){
        return false;
    }

    //step6
    cal_P1(s,t,xA,yA,x1);

    //step7
    calrt(e,x1,R);
    if(is_equal(R,r)){
        return true;
    }
    else{
        return false;
    }
}





//DEBUG 在sm2.hpp中启用
#if DEBUG
void SM2::printDebugResult(big tmp,char* shouldbe){
    char out[65];
    big_to_bytes(0,tmp,out,FALSE);
    streamToString((byte*)out,32,(byte*)out);
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
            calP1(k,x1);
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