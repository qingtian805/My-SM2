#include<iostream>
#include<time.h>
#include<string.h>
#include"sm3.c"

using namespace std;

#define DEBUG true

//参数设置 
#define __KLEN 64
    //密钥长度
#define __T0 0
    //初始时间
#define __T1 30
    //时间间隔
#define DIGEST 6
    //返回数字位数（DEC）

void streamToString(unsigned char* stream){
    int pm,pr;
    stream[64] = '\0';
    pr = 63;
    for(pm = 31; pm>=0; pm--)
    {
        stream[pr] = stream[pm] & 0b00001111;//低4位
        stream[pr] = stream[pr] + '0';
        if(stream[pr] > '9'){
            stream[pr] = stream[pr] + 7;
        }
        pr--;
        stream[pr] = stream[pm] >> 4;//高4位
        stream[pr] = stream[pr] + '0';
        if(stream[pr] > '9'){
            stream[pr] = stream[pr] + 7;
        }
        pr--;
    }
}

namespace TOTPnp{
    time_t T1 = __T1;//TOTP TC时间间隔设置
    time_t T0 = __T0;//TOTP TC起始时间设置
    time_t t;
    bool __TC(char *ct);// void -> ct
    bool __HMAC(char *K,char *TC,char *res);// K,TC -> res
    bool __Truncate(char *HMAC,char *cotp);//HMAC -> cotp
    int __pow();
};

int TOTPnp::__pow(void){
    int res = 10;
    for(int i=1;i<DIGEST;i++){
        res = res * 10;
    }
    return res;
}

bool TOTPnp::__TC(char* ct){
    char *tp;
    t = time(NULL);//获得当前时间戳
    t = (t-T0)/T1;
    tp = (char*)&t;
    for(int i=0; i<8;i++){
        ct[i] = *(tp+i);
    }
    return true;
}

bool TOTPnp::__HMAC(char *K,char *TC,char *res){
    char message[__KLEN + 8];
    strcpy(message,K);//构造连接K TC的字符串(字节串)
    strcat(message,TC);
    sm3((unsigned char*)message,__KLEN + 8,(unsigned char*)res);
    return true;
}

bool TOTPnp::__Truncate(char *HMAC,char *cotp){
    short offset;
    unsigned int otp;
    unsigned int binary = 0;
    unsigned int otpp;
    offset = *(HMAC + 31) & 0xf;//offset 取最后一个字节的低4位
    cout << offset << endl;
    char HMACout[65];
    strcpy(HMACout,HMAC);
    streamToString((unsigned char*)HMACout);
    cout << HMACout <<endl;
    binary = (HMAC[offset] & 0x7f) << 24;
    binary = binary | (HMAC[offset + 1]  << 16);
    binary = binary | (HMAC[offset + 2]  << 8);
    binary = binary | (HMAC[offset + 3] );
    cout << binary << endl;
    otp = binary % (TOTPnp::__pow());
    otpp = otp;
    for(int i=1;i<=DIGEST;i++){
        cotp[DIGEST - i] = otpp % 10 + 48;
        otpp = otpp / 10;
    }
    cotp[DIGEST]='\0';
    return true;
}

bool TOTP(char *K,char * cotp){//K -> cotp
    /*
    TOTP总调用函数，如果需要修改返回位数，请修改源码最上方设置。另外，请注意字符串结尾\0问题
    */
    char ct[8];
    char res[33];
    TOTPnp::__TC(ct);
    TOTPnp::__HMAC(K,ct,res);
    TOTPnp::__Truncate(res,cotp);
    return true;
}

#if DEBUG
int main(void){
    char key[] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
    char cotp[7];
    TOTP(key,cotp);
    cout << cotp << endl;
    return 0;
}
#endif