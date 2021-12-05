#ifndef __TOTP_H__
#define __TOTP_H__
//#define DEBUG

//参数设置 
    //密钥长度(字节)
#define TOTP_MAX_KLEN 64
    //初始时间
#define TOTP_T0 0
    //时间间隔
#define TOTP_T1 30
    //返回数字位数（DEC）
#define TOTP_DIGEST 6
    //信息摘要长度
#define TOTP_HLEN 32

namespace TOTP{
    void __TC(char *ct);// void -> ct
    void __HMAC(char *K,int Klen,char *M,int Mlen,char *res);// K,TC -> res
    void __Truncate(char *HMAC,char *cotp);//HMAC -> cotp
    int __pow();
    bool TOTP(char *K,char * cotp);
};
#endif