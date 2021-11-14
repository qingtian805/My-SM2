extern "C"{
#include"miracl.h"
}
#define DEBUG true

//随机数生成器赋值
#define SEED 0x1BD8C95A

namespace SM2{ 
    typedef unsigned char byte;  

    //椭圆曲线参数赋值
    char pn[65] =  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
    char an[65] =  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
    char bn[65] =  "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
    char nn[65] =  "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
    char xGn[65] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
    char yGn[65] = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";

    #if DEBUG
    //标准第五章中的一些测试用参数
    char xAn[65] = "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020";
    char yAn[65] = "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13";
    char dAn[65] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";
    char en[65] =  "F0B43E94BA45ACCAACE692ED534382EB17E6AB5A19CE7B31F4486FDFC0D28640";
    char kn[65] =  "59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21";
    char x1n[65] = "04EBFC718E8D1798620432268E77FEB6415E2EDE0E073C0F4F640ECD2E149A73";
    char y1n[65] = "E858F9D81E5430A57B36DAAB8F950A3C64E6EE6A63094D99283AFF767E124DF0";
    char rn[65] =  "F5A03B0648D2C4630EEAC513E1BB81A15944DA3827D5B74143AC7EACEEE720B3";
    char sn[65] =  "B1B6AA29DF212FD8763182BC0D421CA1BB9038FD1F7F42D4840B69C485BBC1AA";
    #endif

    //sm3补足函数，将比特流转换为字符串
    //stream,streamlength -> string
    //转换后的字符串存于相同字符串，请确保字符串长度为原长度的两倍+1
    //长度为32字节的流需要65字节长的字符串
    void streamToString(byte* stream,int streamlength, byte* string);

    //计算_M = ZA||M
    void gen_Message(char* ZA,char* message,char* _message);

    //计算e = H256(_M)，并转化整数
    void calE(char* _message,big e);

    //生成随机数k in [1,n-1]
    void genRandom(big k);

    //计算r = (e + x1)mod n
    //计算R = (e`+ x1`)mod n
    //计算t = (r`+ s`)mod n
    void calrt(big er,big x1s,big rt);

    //计算s==((1 + dA)^ -1·(k - r·dA))mod n
    void cals(big k,big dA,big r,big s);

    //计算签名公钥 P1(x1,y1) = [k]G
    void calP1(big k,big x1);

    //计算椭圆曲线点P1`(x1`,y1`)=[s`]G+[t]PA
    void cal_P1(big s,big t,big xA,big yA,big x1);

    //判断输入给出数值是否处于[1,n-1]，如果不在则返回真
    bool notIn_1n(big __x);

    //判断输入是否等于n
    bool is_n(big __x);

    //判断给出数值是否为0，为0返回真
    bool is_zero(big __x);

    //判断输入是否相等，相等返回真
    bool is_equal(big __x,big __y);

    #if DEBUG
    //用于打印测试结果
    void printDebugResult(big tmp,char* shouldbe);
    #endif
}
    
//计算用户其他信息函数（ZA）
//输入：ID 发送者ID
//  xA yA 发送者公钥坐标
//输出：ZA[65] 用户其他信息
void genZA(char *ID,char *xA,char *yA,char* ZA);

//SM2椭圆曲线算法签名生成函数
//输入：ZA[65] 发送者其他信息
//  xAn[65] yAn[65] 发送者公钥坐标
//  dAn[65] 发送者私钥
//  message 签名信息
//  messagelen 信息长度
//输出：rn[65] sn[65] 签名密钥对
void genSignment(char* ZA,char* dAn,char* message,int messagelen,char* rn,char* sn);

//SM2椭圆曲线算法签名验证函数
//输入：ZA 发送者其他信息
//  xAn yAn 发送者的公钥坐标
//  message 签名信息
//  messagelen 信息长度
//  rn sn 签名密钥对
//返回：签验证结果
bool verifySignment(char* ZA,char* xAn,char* yAn,char* message,int messagelen,char* rn,char* sn);