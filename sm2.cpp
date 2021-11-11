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

void SM2::streamToString(unsigned char* stream,int streamlength, unsigned char* string){
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

void SM2::calOtherInfo(char *ID,char *xA,char *yA,unsigned char* ZA){
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
    sm3((unsigned char*)info,IDlen+387,ZA);
    streamToString(ZA,32,ZA);
}

void SM2::gen_Message(char* ID,char* xAn,char* yAn,char* message,char* _message){ 
    char ZA[65];
    calOtherInfo(ID, xAn, yAn, (unsigned char*)ZA);
    strcpy(_message,ZA);
    strcat(_message,(char*)message);
}

void SM2::calE(char* _message,char* en){
    sm3((unsigned char*)_message,strlen(_message),(unsigned char*)en);
    streamToString((unsigned char*)en,32,(unsigned char*)en);
}

void SM2::genRandom(big k){
    big n;

    n = mirvar(0);
    cinstr(n,nn);

    irand(time(NULL)+SEED);
    bigrand(n,k);

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

void SM2::calr(big e,big x1,big r){
    big n,tmp;

    n = mirvar(0);
    tmp = mirvar(0);
    cinstr(n,nn);

    add(x1,e,tmp);//tmp = x1+e
    divide(tmp,n,n);//tmp = tmp mod n
    copy(tmp,r);//r = tmp

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

void genSignment(char* ID,char* xAn,char* yAn,char* dAn,unsigned char* message,int messagelen,char* rn,char* sn){
    //init MIRACL
    miracl *mip;
    mip = mirsys(64,16);//16位
    if(mip == NULL){
        cout << "MIRACL INIT FALTAL" << endl;
        exit(0);
    }
    big ZERO;//0，方便后续对比判断
    big rk;//r+k
    big e;
    big k;
    big x1;
    big r;//
    big s;
    big n;//参数n用于判断
    ZERO = mirvar(0);
    rk = mirvar(0);
    e = mirvar(0);
    k = mirvar(0);
    x1 = mirvar(0);
    r = mirvar(0);
    s = mirvar(0);
    n = mirvar(0);
    cinstr(n,nn);

    //step 1
    char _message[messagelen + 65];
    gen_Message(ID,xAn,yAn,(char*)message,_message);

    //step 2
    char en[65];
    calE(_message,en);
    cinstr(e,en);

    do{
        do{
            //step 3
            genRandom(k);

            //step 4
            calP1(k,x1);

            //step 5
            calr(e,x1,r);

            add(r,k,rk);//rk = r + k
        }while(mr_compare(ZERO,r) == 0 || mr_compare(rk,n) == 0);
        //step 6
        big dA;
        dA = mirvar(0);
        cinstr(dA,dAn);
        cals(k,dA,r,s);
    }while(mr_compare(s,ZERO) == 0);

    big_to_bytes(0,r,rn,FALSE);
    streamToString((unsigned char*)rn,32,(unsigned char*)rn);
    big_to_bytes(0,s,sn,FALSE);
    streamToString((unsigned char*)sn,32,(unsigned char*)sn);
    
    mirkill(ZERO);
}

void verifySignment(){

}

#if DEBUG
int main(){
    
}
#endif