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
    // void -> ct
    void __TC(char *ct);

    // K,TC -> res
    void __HMAC(char *K,int Klen,char *M,int Mlen,char *res);

    //HMAC -> cotp
    void __Truncate(char *HMAC,char *cotp);

    //10^TOTP_DIGEST
    int __pow();

    //TOTP主函数，输入K返回cotp
    bool TOTP(char *K,char * cotp);
};
#endif